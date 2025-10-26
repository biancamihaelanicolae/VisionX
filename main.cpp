#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>

class Film {
private:
    std::string titlu;
    std::string gen;
    int durata; // in minute

public:
    explicit Film(const std::string& t="", const std::string& g="", int d=0)
        :titlu(t),gen(g),durata(d){}

    Film(const Film& f)=default;

    Film& operator=(const Film& f)=default;

    ~Film()=default;

    void modificare_durata(int d) {
        if (d>0) durata=d;
    }
    [[nodiscard]]bool este_gen(const std::string& g) const{
        return gen == g;
    }
    void afisare_detalii() const {
        std::cout << "Titlu: " << titlu <<" (" << gen << ") "<<" Durata: "<< durata<<"min";
    }

    friend std::ostream& operator<<(std::ostream& os, const Film& f) {
        os << f.titlu << " (" << f.gen << ") " << f.durata;
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
        if (l>=0 && l<capacitate && !locuri_ocupate[l]) {
            locuri_ocupate[l]=true;
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

    friend std::ostream& operator<<(std::ostream& os, const Sala& s) {
        os << "Sala " << s.numar << ", capacitate: " << s.capacitate<< ", locuri libere: "<< s.locuri_libere();
        return os;
    }
};

class Bilet {
    private:
    std::string nume_client;
    Film film;
    int loc;
    Sala sala;

    public:
    Bilet(const std::string& nume, const Film& f, const Sala& s, int l)
        :nume_client(nume), film(f), loc(l), sala(s){}

    Bilet(const Bilet& b)=default;

    Bilet& operator=(const Bilet& b)=default;

    ~Bilet()=default;

    void schimba_loc(int l){ loc = l;}

    void afisare_bilet() const {
        std::cout << "Bilet pentru: " << nume_client << "\n"
                  << "Film: " << film << "\n"
                  << "Sala: " << sala << "\n"
                  << "Loc: " << loc << "\n";
    }

    void upgradeLoc(int loc_nou, Sala& s) {
        if (loc_nou >= 0 && loc_nou < s.getCapacitate()) {
            if (s.rezervare_loc(loc_nou)) {
                loc = loc_nou;
                std::cout << "Locul a fost schimbat cu succes la locul " << loc_nou << "!\n";
            } else {
                std::cout << "Locul " << loc_nou << " este deja ocupat!\n";
            }
        } else {
            std::cout << "Numarul de loc este invalid!\n";
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Bilet& b) {
        os << "Bilet pentru: " << b.nume_client << "\n"
        << "Film: " << b.film << "\n"
        << "Sala: " << b.sala << "\n"
        << "Loc: " << b.loc;
        return os;
    }
};

class Cinema {
    private:
    std::vector<Film> filme;
    std::vector<Sala> sali;
    std::vector<Bilet> bilete;

    public:
    Cinema()=default;
    Cinema(const Cinema& c)=default;

    Cinema& operator=(const Cinema& c)=default;

    ~Cinema()=default;

    void adaug_film(const Film& f) {filme.push_back(f);}
    void adaug_sala(const Sala& s) {sali.push_back(s);}
    void vinde_bilete(const std::string& nume_client, int film_index, int sala_index,int loc) {
        if (film_index < 0 || film_index >= static_cast<int>(filme.size()) || sala_index < 0 || sala_index >= static_cast<int>(sali.size())) return;
        if (sali[sala_index].rezervare_loc(loc)) {
            bilete.emplace_back(nume_client,filme[film_index],sali[sala_index],loc);
            std::cout << "Bilet vandut cu succes!\n";
        }else{
            std::cout << "Locul nu este dispoibil!\n";
        }
    }

    void afiseaza_program() const {
        std::cout << "Filme disponibile:\n";
        for (const auto& f : filme) std::cout << "- " << f << "\n";
        std::cout << "Sali disponibile:\n";
        for (const auto& s : sali) std::cout << "- " << s << "\n";
    }
    [[nodiscard]]std::vector<Film> getFilme() const { return filme; }
    [[nodiscard]]std::vector<Sala> getSali() const { return sali; }


    friend std::ostream& operator<<(std::ostream& os, const Cinema& c) {
        os << "Cinema:\nFilme:\n";
        for (const auto& f : c.filme) os << f << "\n";
        os << "Sali:\n";
        for (const auto& s : c.sali) os << s << "\n";
        os << "Bilete:\n";
        for (const auto& b : c.bilete) os << b << "\n";
        return os;
    }
};

int main() {
    Cinema cinema;

    cinema.adaug_film(Film("Inception", "SF", 148));
    cinema.adaug_film(Film("Titanic", "Romantic", 195));
    cinema.adaug_film(Film("Home alone", "Comedie", 103));

    cinema.adaug_sala(Sala(1,100));
    cinema.adaug_sala(Sala(2,70));
    cinema.adaug_sala(Sala(3,90));

    cinema.afiseaza_program();

    std::string nume;
    int film_index, sala_index, loc;
    char opt;

    do {
        std::cout << "\nNumele clientului: ";
        std::getline(std::cin, nume);

        while (true) {
            std::cout << "Alege film :";
            std::cin >> film_index;
            if(film_index >=0 && film_index < static_cast<int>(cinema.getFilme().size())) break;
            std::cout << "Index invalid! Incearca din nou.\n";
        }

        while (true) {
            std::cout << "Alege sala: ";
            std::cin >> sala_index;
            if(sala_index >=0 && sala_index < static_cast<int>(cinema.getSali().size())) break;
            std::cout << "Index invalid! Incearca din nou.\n";
        }

        while (true) {
            std::cout << "Alege locul dorit: ";
            std::cin >> loc;
            if(loc >=0 && loc < cinema.getSali()[sala_index].getCapacitate()) break;
            std::cout << "Loc invalid! Incearca din nou.\n";
        }
        std::cin.ignore();

        if (cinema.getSali()[sala_index].rezervare_loc(loc)) {
            Bilet bilet(nume, cinema.getFilme()[film_index], cinema.getSali()[sala_index], loc);
            std::cout << "\nBiletul tau:\n";
            bilet.afisare_bilet();
        } else {
            std::cout << "Locul nu este disponibil!\n";
        }

        std::cout << "Vrei sa cumperi alt bilet? (y/n): ";
        std::cin >> opt;
        std::cin.ignore();
    } while(opt == 'y' || opt == 'Y');

    std::cout << "\nMultumim pentru achizitie!\n";


    return 0;
}