#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <limits>
#include <map>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
void enable_ansi_coloring() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
}
#endif

const std::string ANSI_RESET = "\033[0m";
const std::string ANSI_RED = "\033[31m";
const std::string ANSI_GREEN = "\033[32m";

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
    [[nodiscard]]const std::string& getGen() const {  return gen;  }

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

    bool rezervare_multipla(const std::vector<int>& locuri) {
        if (locuri.empty()) {
            return true;
        }

        for (int l : locuri) {
            if (l < 1 || l > capacitate || locuri_ocupate[l-1]) {
                return false;
            }
        }

        for (int l : locuri) {
            locuri_ocupate[l-1] = true;
        }

        return true;
    }

    [[nodiscard]]bool sala_plina() const {
        return std::ranges::all_of(locuri_ocupate,[](bool ocupat){ return ocupat;});
    }

    [[nodiscard]]int locuri_libere() const {
        int cnt=0;
        for (bool ocupat : locuri_ocupate) if (!ocupat) cnt++;
        return cnt;
    }

    void vizualizare_locuri() const {
        std::cout << "\n----- Harta Salii " << numar << " ( Locuri libere: " << locuri_libere() << " din " << capacitate << " )-----\n";

        std::cout << "=============================================================\n";
        std::cout << "                         E C R A N\n";
        std::cout << "=============================================================\n";

        const int locuri_pe_rand = 10;
        for (int i = 0; i< capacitate; i++) {
            std::cout << std::setw(4) << i+1;
            if (locuri_ocupate[i]) {
                std::cout << ANSI_RED << "[X]" << ANSI_RESET;
            }else {
                std::cout << ANSI_GREEN << "[O]" << ANSI_RESET;
            }

            std::cout << " ";

            if ((i+1) % locuri_pe_rand == 0) {
                std::cout << "\n";
            }
        }
        std::cout << "-----------------------------------------------------\n";
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

    bool rezervare_multipla(const std::vector<int>& locuri) {
        return sala.rezervare_multipla(locuri);
    }

    [[nodiscard]]const std::string& getZi() const {return zi;}
    [[nodiscard]]const std::string& getOra() const {return ora;}
    [[nodiscard]]const std::string& getTip() const {return tip;}
    [[nodiscard]]const Film& getFilm() const {return film;}
    [[nodiscard]]const Sala& getSala() const {return sala;}

    friend std::ostream& operator<<(std::ostream& os, const Proiectie& p) {
        os << "Proiectie: " << p.film << "\n"
           << "Sala: " << p.sala << "\n"
           << "Zi: " << p.zi << " - Ora: " << p.ora << " ( " << p.tip << " ) "<<"\n";
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
        std::cout << "Introdu numele clientului: ";
        std::getline(std::cin, username);

        std::string temp_tip;

        while (true) {
            std::cout<< "Introdu tipul biletului dorit (normal/student/elev): ";

            if (!(std::cin >> temp_tip)) {
                std::cin.clear();
                std::cout << "Citirea tipului de bilet a esuat!";
                tip="normal";
                break;
            }

            std::transform(temp_tip.begin(), temp_tip.end(), temp_tip.begin(), [](unsigned char c) {
                return std::tolower(c);
            });

            if (temp_tip == "normal" || temp_tip == "student" || temp_tip == "elev") {
                tip = temp_tip;
                break;
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


    friend std::ostream& operator<<(std::ostream& os, const Bilet& b) {
        os << "\n---BILET CINEMA---\n"
           << "Client: " << b.nume_client << "\n"
           << "Film: " << b.film << "\n"
           <<"Data/ Ora: "<< b.zi << "/ " << b.ora << "\n"
           << "Sala: " << b.sala << "\n"
           << "Loc: " << b.loc << "\n"
           << "Pret: " << std::fixed << std::setprecision(2) << b.pret << "lei\n"
           <<"---------------------\n";
    }
};

class Cinema {
    private:
    std::vector<Proiectie> proiectii;

    static int get_ordinea_zilei(const std::string& zi) {
        static const std::map<std::string, int > ordine ={
            {"Luni", 0}, {"Marti", 1}, {"Miercuri", 2}, {"Joi", 3},
            {"Vineri", 4}, {"Sambata", 5}, {"Duminica", 6}
        };

        auto it = ordine.find(zi);
        if (it != ordine.end()) {
            return it->second;
        }

        return 99;
    }

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
            int ordine_a = Cinema::get_ordinea_zilei(a.getZi());
            int ordine_b = Cinema::get_ordinea_zilei(b.getZi());

            if (ordine_a != ordine_b) {
                return ordine_a < ordine_b;
            }

            return a.getOra() < b.getOra();
        });

        std::cout << "\n==== Programul cinematografului VisionX ====\n";
        std::string zi_curenta;

        for (size_t i = 0; i < program_sortat.size(); ++i) {
            if (program_sortat[i].getZi() != zi_curenta) {
                zi_curenta = program_sortat[i].getZi();
                std::cout << "\n Ziua:  " << zi_curenta << "\n";
            }

            std::cout << i << ". ";
            program_sortat[i].afisare();
            std::cout << "\n";
        }

        std::cout << "========================================\n";
    }

    [[nodiscard]]std::vector<Proiectie> filtreaza_pe_gen(const std::string& gen_cautat) const {
        std::vector<Proiectie> lista_filtrata;
        for (const auto& p : proiectii) {
            if (p.getFilm().getGen() == gen_cautat) {
                lista_filtrata.push_back(p);
            }
        }

        std::ranges::sort(lista_filtrata, [](const Proiectie& a, const Proiectie& b) {
            int ordine_a = Cinema::get_ordinea_zilei(a.getZi());
            int ordine_b = Cinema::get_ordinea_zilei(b.getZi());

            if (ordine_a != ordine_b) {
                return ordine_a < ordine_b;
            }

            return a.getOra() < b.getOra();
        });

        return lista_filtrata;
    }

    Proiectie* get_proiectie(int index) {
        if (index >= 0 && index < (int)proiectii.size()) {
            return &proiectii[index];
        }
        return nullptr;
    }

    void vinde_bilet(const Utilizator& u, Proiectie& p, const std::vector<int>& locuri) {
        if (locuri.empty()) {
            std::cout << "Nu ati specificat niciun loc.\n";
            return;
        }

        if (p.rezervare_multipla(locuri)) {
            double pret_unitar = calculeaza_pret(u.getTip());

            std::cout << "\n----BILETE VANDUTE----\n";

            for (int loc : locuri) {
                Bilet b(u.getUsername(), p.getFilm(), p.getSala(), loc, pret_unitar, p.getOra(), p.getZi());
                std::cout << b;
            }
        }else {
            std::cout << "REZERVARE ESUATA: Cel putin unul dintre locurile selectate este invalid!";
        }
    }

    void actualizeaza_sala_originala(const Proiectie& proiectie_modificata) {
        for (auto& p_originala : proiectii) {
            if (p_originala.getFilm().getTitlu() == proiectie_modificata.getFilm().getTitlu() &&
                p_originala.getZi() == proiectie_modificata.getZi() &&
                p_originala.getOra() == proiectie_modificata.getOra())
            {
                p_originala = proiectie_modificata;
                break;
            }
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Cinema& c) {
        os << "\n==== Programul complet VisionX ====\n";
        if (c.proiectii.empty()) {
            std::cout << "Nu exista proiectii programate.\n";
            return os;
        }

        for (size_t i = 0; i < c.proiectii.size(); ++i) {
            os << i << ". ";
            os << c.proiectii[i] << "\n";
        }
        os << "======================================\n";
        return os;
    }
};

int main() {
    #ifdef _WIN32
        enable_ansi_coloring();
    #endif

    std::cout.precision(2);

    Cinema cinema;
    cinema.incarca_din_fisier("program.txt");

    if (cinema.get_proiectie(0) == nullptr) {
        std::cout<< "Programul nu a putut fi incarcat!\n";
        return 1;
    }

    std::cout << "Bine ai venit pe site-ul VisionX\n";

    std::cout << cinema;

    while (true) {
        std::string comanda;
        std::cout << "\nPentru a cumpara un bilet foloseste comanda 'cumpara', iar daca vrei sa inchizi pagina foloseste comanda 'exit page'!";
        if (!(std::cin >> comanda)) break;

        std::transform(comanda.begin(), comanda.end(), comanda.begin(), [](unsigned char c){return std::tolower(c); });

        if (comanda == "exit" || comanda == "exit page") {
            break;
        }

        if (comanda != "cumpara") {
            std::cout << "Comanda necunoscuta.\n";
            continue;
        }

        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

        std::string raspuns_filtrare;
        std::cout << "Vrei sa cauti filemele dupa un anumit gen (da/nu)?";
        std::cin >> raspuns_filtrare;
        std::transform(raspuns_filtrare.begin(), raspuns_filtrare.end(), raspuns_filtrare.begin(), [](unsigned char c){return std::tolower(c);});

        std::vector<Proiectie> lista_proiectii_curente;

        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

        if (raspuns_filtrare == "da") {
            std::string gen_ales;
            std::cout << "Introdu genul dorit: ";
            std::cin >> gen_ales;

            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

            lista_proiectii_curente = cinema.filtreaza_pe_gen(gen_ales);

            if (lista_proiectii_curente.empty()) {
                std::cout << "\nNu exista proiectii pentru genul " << gen_ales << " Vanzare anulata!\n";
                continue;
            }

            std::cout << "\n===== Proiectii Genul " << gen_ales << "=====\n";
            std::string zi_curenta;
            for (size_t i=0; i < lista_proiectii_curente.size(); ++i) {
                if (lista_proiectii_curente[i].getZi() != zi_curenta) {
                    zi_curenta = lista_proiectii_curente[i].getZi();
                    std::cout << "\nZiua: " << zi_curenta << "\n";
                }

                std::cout << i << ". " << lista_proiectii_curente[i].getFilm().getTitlu()
                          << " | Sala " << lista_proiectii_curente[i].getSala().getNumar()
                          << " | Ora: " << lista_proiectii_curente[i].getOra()
                          << " (" << lista_proiectii_curente[i].getTip() << ")\n";
            }

        }else{
            cinema.afiseaza_program();
        }

        Utilizator u;
        u.citire_utilizator();

        int idx;
        std::cout << "Introdu numarul proiectiei dorite: ";
        if (!(std::cin >> idx)) {
            std::cout << "Intrare invalida. Vanzare anulata.\n";
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            continue;
        }

        Proiectie* proiectie_selectata = cinema.get_proiectie(idx);

        if (!lista_proiectii_curente.empty()) {
            if (idx >= 0 && idx < (int)lista_proiectii_curente.size()) {
                proiectie_selectata = &lista_proiectii_curente[idx];
            }
        }else {
            proiectie_selectata = cinema.get_proiectie(idx);
        }
        if (!proiectie_selectata) {
            std::cout << "Index de proiectie invalid!";
            continue;
        }

        if (proiectie_selectata->getSala().sala_plina()) {
            std::cout << "Sala este plina! Alege alta proiectie!\n";
            continue;
        }

        proiectie_selectata->getSala().vizualizare_locuri();

        std::cout << "Proiectie aleasa: " << proiectie_selectata->getFilm().getTitlu() << "\n";
        std::cout << "Locuril libere: " << proiectie_selectata->getSala().locuri_libere() << " din " << proiectie_selectata->getSala().getCapacitate() << "\n";
        std::cout << "Alege locurile dorite: ";

        std::vector<int> locuri_dorite;
        std::string line;

        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

        std::getline(std::cin, line);
        std::stringstream ss(line);
        int loc_curent;
        while (ss >> loc_curent) {
            locuri_dorite.push_back(loc_curent);
        }

        if (locuri_dorite.empty()) {
            std::cout << "Nu s-au specificat locuri. Vanzare anulata!\n";
            continue;
        }

        cinema.vinde_bilet(u, *proiectie_selectata, locuri_dorite);

        if (!lista_proiectii_curente.empty()) {
            cinema.actualizeaza_sala_originala(*proiectie_selectata);
        }
    }

    std::cout << "\nMultumim ca ati folosit VisionX!";

    return 0;
}