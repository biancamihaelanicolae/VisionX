#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <limits>


class Film {
private:
    std::string titlu;
    std::string gen;
    int durata; // in minute

public:
    explicit Film(const std::string& t="", const std::string& g="", int d=0)
        :titlu(t),gen(g),durata(d){}

    Film(const Film& f)
        :titlu(f.titlu),gen(f.gen),durata(f.durata){}

    Film& operator=(const Film& f) {
        if (this != &f) {
            titlu = f.titlu;
            gen = f.gen;
            durata = f.durata;
        }
        return *this;
    }

    ~Film(){}

    [[nodiscard]]const std::string& getTitlu() const {  return titlu;  }

    friend std::ostream& operator<<(std::ostream& os, const Film& f) {
        os << f.titlu << " (" << f.gen << ", " << f.durata<<" min)";
        return os;
    }
};

class Sala {
    private:
    int numar;
    int capacitate;
    std::vector<bool> locuri_ocupate; //true = ocupat; false = liber;

    public:
    explicit Sala(int n = 0, int cap = 0)
        : numar(n), capacitate(cap), locuri_ocupate(cap,false){}

    Sala(const Sala& s)=default;

    Sala& operator=(const Sala& s) =default;

    ~Sala()=default;

    bool rezervare_loc(int l) {
        if (l>=1 && l<=capacitate && !locuri_ocupate[l-1]) {
            locuri_ocupate[l-1]=true;
            return true;
        }
        return false;
    }

    [[nodiscard]]bool sala_plina() const {
        return std::ranges::all_of(locuri_ocupate,[](bool ocupat){ return ocupat;});
    }

    [[nodiscard]]int locuri_libere() const {
        int cnt=0;
        for (bool ocupat : locuri_ocupate) if (!ocupat) cnt++;
        return cnt;
    }

    [[nodiscard]]int getCapacitate() const { return capacitate; }
    [[nodiscard]]int getNumar() const {return numar; }

    friend std::ostream& operator<<(std::ostream& os, const Sala& s) {
        os << "Sala " << s.numar << ", Capacitate: " << s.capacitate<< ", Locuri libere: "<< s.locuri_libere();
        return os;
    }
};

class Proiectie {
    private:
    Film film;
    Sala sala;
    std::string zi;
    std::string ora;
    std::string tip; //2D, 3D, IMAX

    public:
    Proiectie(const Film& f, const Sala& s, const std::string& z, const std::string& o, const std::string& t)
        :film(f),sala(s),zi(z),ora(o),tip(t){}

    void afisare() const {
        std::cout<< film.getTitlu()<< "|" << " " << sala.getNumar()<< " | " << zi<< " - ora: " << ora<< " | " << "Tip: "<< tip<<std::endl;
    }

    bool incearca_rezervare(int loc) {
        return sala.rezervare_loc(loc);
    }

    [[nodiscard]]const std::string& getZi() const {return zi;}
    [[nodiscard]]const std::string& getOra() const {return ora;}
    [[nodiscard]]const std::string& getTip() const {return tip;}
    [[nodiscard]]const Film& getFilm() const {return film;}
    [[nodiscard]]const Sala& getSala() const {return sala;}

    friend std::ostream& operator<<(std::ostream& os, const Proiectie& p) {
        os << p.film.getTitlu() << " (" << p.zi << "," << p.ora<< "," << p.tip<< ")";
        return os;
    }
};

class Utilizator {
    private:
    std::string username;
    std::string tip; // normal/ student/ elev

    public:
    Utilizator()=default;

    Utilizator(const std::string& u, const std::string& t)
        :username(u),tip(t){}

    [[nodiscard]]const std::string& getTip() const {return tip;}
    [[nodiscard]]const std::string& getUsername() const {return username;}

    void citire_utilizator() {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Introdu numele clientului: ";
        std::getline(std::cin, username);

        bool tip_valid = false;
        std::string temp_tip;

        while (!tip_valid) {
            std::cout<< "Introdu tipul biletului dorit (normal/student/elev): ";
            std::cin >> temp_tip;

            std::transform(temp_tip.begin(), temp_tip.end(), temp_tip.begin(), [](unsigned char c) {
                return std::tolower(c);
            });

            if (temp_tip == "normal" || temp_tip == "student" || temp_tip == "elev") {
                tip = temp_tip; // <--- Aici era problema! Atribuirea lipsea!
                tip_valid = true;
            } else {
                std::cout << "Tipul biletului este invalid! Te rog introdu 'normal', 'student' sau 'elev'\n";
            }
        }
    }
};

class Bilet {
    private:
    std::string nume_client;
    Film film;
    int loc;
    Sala sala;
    double pret;
    std::string ora;
    std::string zi;

    public:
    Bilet(const std::string& nume, const Film& f, const Sala& s, int l, double p, const std::string& o, const std::string& z)
        :nume_client(nume), film(f), loc(l), sala(s), pret(p), ora(o), zi(z){}

    Bilet(const Bilet& b)=default;

    Bilet& operator=(const Bilet& b)=default;

    ~Bilet()=default;


    void afisare_bilet() const {
        std::cout << "\n---BILET CINEMA---\n"
                  << "Client: " << nume_client << "\n"
                  << "Film: " << film.getTitlu() << "\n"
                  <<"Data/ Ora: "<< zi << "/ " << ora << "\n"
                  << "Sala: " << sala.getNumar() << "\n"
                  << "Loc: " << loc << "\n"
                  << "Pret: " << std::fixed << std::setprecision(2) << pret << "lei\n"
                  <<"---------------------\n";
    }
};

class Cinema {
    private:
    std::vector<Proiectie> proiectii;

    public:
    double calculeaza_pret(const std::string& tip_bilet) const {
        double pret_baza = 30.0;
        if (tip_bilet == "student") return pret_baza * 0.7;
        if (tip_bilet == "elev") return pret_baza * 0.5;
        return pret_baza;
    }

    void incarca_din_fisier(const std::string& nume_fisier) {
        std::ifstream fin(nume_fisier);
        if (!fin) {
            std::cerr << "Eroare la deschiderea fisierului " << nume_fisier << "\n";
            return;
        }

        std::string titlu, gen, zi, ora, tip;
        int durata, nr_sala, capacitate;

        while (fin >> titlu >> gen >> durata >> zi >> ora >> tip >> nr_sala >> capacitate) {
            Film f(titlu, gen, durata);
            Sala s(nr_sala, capacitate);
            proiectii.emplace_back(f, s, zi, ora, tip);
        }
        fin.close();
    }

    void afiseaza_program() const {
        if (proiectii.empty()) {
            std::cout << "Nu exista proiectii programate.\n";
            return;
        }

        std::vector<Proiectie> program_sortat = proiectii;
        std::ranges::sort(program_sortat, [](const Proiectie& a, const Proiectie& b) {
            if (a.getZi() != b.getZi()) return a.getZi() < b.getZi();
            return a.getOra() < b.getOra();
        });

        std::cout << "\n==== Programul cinematografului VisionX ====\n";
        std::string zi_curenta;

        for (size_t i = 0; i < proiectii.size(); ++i) {
            if (proiectii[i].getZi() != zi_curenta) {
                zi_curenta = proiectii[i].getZi();
                std::cout << "\n Ziua:  " << zi_curenta << "\n";
            }

            std::cout << i << ". ";
            proiectii[i].afisare();
            std::cout << "\n";
        }

        std::cout << "========================================\n";
    }

    Proiectie* get_proiectie(int index) {
        if (index >= 0 && index < (int)proiectii.size()) {
            return &proiectii[index];
        }
        return nullptr;
    }

    void vinde_bilet(const Utilizator& u, Proiectie& p, int loc) {
        if (p.getSala().getCapacitate() <= loc || loc <= 0) {
            std::cout << "Loc invalid!\n";
            return;
        }

        if (p.incearca_rezervare(loc)) {
            double pret = calculeaza_pret(u.getTip());
            Bilet b(u.getUsername(), p.getFilm(), p.getSala(), loc, pret, p.getOra(),p.getZi());
            b.afisare_bilet();
        }else {
            std::cout << "Locul " << loc << " este deja ocupat sau este invalid pentru aceasta sala!\n";
        }
    }
};

int main() {

    std::cout.precision(2);

    Cinema cinema;
    cinema.incarca_din_fisier("tastatura.txt");

    if (cinema.get_proiectie(0) == nullptr) {
        std::cout<< "Programul nu a putut fi incarcat!\n";
        return 1;
    }

    std::cout << "Bine ai venit pe site-ul VisionX\n";

    while (true) {
        std::string comanda;
        std::cout << "\nPentru a cumpara un bilet foloseste comanda 'cumpara', iar daca vrei sa inchizi pagina foloseste comanda 'exit page'!";
        if (!(std::cin >> comanda)) break;

        std::transform(comanda.begin(), comanda.end(), comanda.begin(), [](unsigned char c){return std::tolower(c); });

        if (comanda == "exit" || comanda == "exit page") {
            break;
        }

        Utilizator u;
        u.citire_utilizator();
        std::cout << "Client: " << u.getUsername() << ", Tip bilet: " << u.getTip() << "\n";

        cinema.afiseaza_program();

        int idx;
        std::cout << "Introdu numarul proiectiei dorite: ";
        if (!(std::cin >> idx)) {
            std::cout << "Intrare invalida. Vanzare anulata.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        Proiectie* proiectie_selectata = cinema.get_proiectie(idx);

        if (!proiectie_selectata) {
            std::cout << "Index de proiectie invalid!";
            continue;
        }

        if (proiectie_selectata->getSala().sala_plina()) {
            std::cout << "Sala este plina! Alege alta proiectie!\n";
            continue;
        }

        int loc;
        std::cout << "Proiectie aleasa: " << proiectie_selectata->getFilm().getTitlu() << "\n";
        std::cout << "Locuril libere: " << proiectie_selectata->getSala().locuri_libere() << " din " << proiectie_selectata->getSala().getCapacitate() << "\n";
        std::cout << "Alege locul dorit: ";
        if (!(std::cin >> loc)) {
            std::cout << "Intrare invalida! Vanzare anulata!\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        cinema.vinde_bilet(u, *proiectie_selectata, loc);
    }

    std::cout << "\nMultumim ca ati folosit VisionX!";

    return 0;
}