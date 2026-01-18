//
// Created by Bianca Mihaela on 07/11/2025.
//

#include "Bilet.h"

#include <iomanip>
#include <sstream>

int Bilet::nr_bilete_vandute = 0;

Bilet::Bilet(const std::string& nume, const Film& f, int loc, const Sala& s, const std::string& ora, const std::string& zi, const std::string& tip_pr,
             bool ochelari, const BazaBilet& tip, const std::vector<std::string>& snacks_alese, double p_snacks)
    : nume_client(nume), film(f), loc(loc), sala(s), ora(ora), zi(zi), tip_proiectie(tip_pr), ochelari3D(ochelari),
      snacks_cumparate(snacks_alese), cost_snacks(p_snacks), tip_bilet_ptr(tip.clone())
    {
        this -> tip_bilet_ptr = tip.clone();
    }

Bilet::Bilet(const Bilet& other)
    : nume_client(other.nume_client), film(other.film), loc(other.loc), sala(other.sala),
      ora(other.ora), zi(other.zi),tip_proiectie(other.tip_proiectie), ochelari3D(other.ochelari3D),
      snacks_cumparate(other.snacks_cumparate), cost_snacks(other.cost_snacks), tip_bilet_ptr(other.tip_bilet_ptr ? other.tip_bilet_ptr->clone() : nullptr) {}

const std::string& Bilet::getNumeClient() const {
    return nume_client;
}

int Bilet::getLoc() const {
    return loc;
}

const Film& Bilet::getFilm() const {
    return film;
}

const std::string& Bilet::getOra() const {
    return ora;
}

const std::string& Bilet::getZi() const {
    return zi;
}

void Bilet::marcheaza_vanzare() {
    nr_bilete_vandute++;
}

int Bilet::getNrBileteVandute() {
    return nr_bilete_vandute;
}

Bilet::~Bilet() {
    delete tip_bilet_ptr;
}

void swap(Bilet& first, Bilet& second) noexcept {
    using std::swap;
    swap(first.nume_client, second.nume_client);
    swap(first.film, second.film);
    swap(first.loc, second.loc);
    swap(first.sala, second.sala);
    swap(first.ora, second.ora);
    swap(first.zi, second.zi);
    swap(first.tip_proiectie, second.tip_proiectie);
    swap(first.ochelari3D, second.ochelari3D);
    swap(first.tip_bilet_ptr, second.tip_bilet_ptr);
    swap(first.snacks_cumparate, second.snacks_cumparate);
    swap(first.cost_snacks, second.cost_snacks);
}

Bilet& Bilet::operator = (Bilet other) {
    swap(*this, other);
    return *this;
}

double Bilet::get_pret_final() const {
    double pret_total = 0.0;

    if (zi == "Miercuri" && tip_proiectie!="IMAX") {
        pret_total = 15.0;
    }
    else if (tip_bilet_ptr) {
        pret_total =  tip_bilet_ptr->calculeaza_pret(PRET_BAZA, zi , ochelari3D);
    }else {
        pret_total = PRET_BAZA;
    }

    if (ochelari3D) {
        pret_total += 5.0;
    }

    return pret_total;
}

std::ostream & operator<<(std::ostream &os, const Bilet &b) {
    os << "\n---BILET CINEMA---\n"
            << "Client: " << b.nume_client << "\n"
            << "Film: " << b.film.getTitlu() << "\n"
            <<"Data/ Ora: "<< b.zi << "/ " << b.ora << "\n"
            << "Sala: " << b.sala.getNumar() << "\n"
            << "Loc: " << b.loc << "\n"
            <<"Ochelari 3D inchiriati: " << (b.ochelari3D ? "Da" : "Nu") << "\n"
            <<"---------------------\n";


    if (b.tip_bilet_ptr) {
        b.tip_bilet_ptr->afisare_detalii_tip(os);
        os << "\n";
        b.tip_bilet_ptr->tipareste_mentiuni_speciale(os);
    }

    if (!b.snacks_cumparate.empty()) {
        os << "\n GUSTARI SI BAUTURI:\n";
        for (const auto& snack : b.snacks_cumparate) {
            os << snack << "\n";
        }
        os << " Cost Produse: " << std::fixed << std::setprecision(2) << b.cost_snacks << " RON\n";
    }

    os <<"---------------------\n";
    os << "\nPret final: " << (b.get_pret_final() + b.cost_snacks) << " RON\n";

    os << "\033[31m"
       << "VA RUGAM SA PREZENTATI ACEST BILET LA CASA PENTRU A RIDICA PRODUSELE COMANDATE INAINTE DE INCEPEREA FILMULUI!"
       << "\033[0m" << "\n";
    return os;
}
