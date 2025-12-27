//
// Created by Bianca Mihaela on 07/11/2025.
//

#include "Bilet.h"

#include <iomanip>
#include <sstream>

int Bilet::nr_bilete_vandute = 0;

Bilet::Bilet(const std::string &nume_client, const Film &f, int loc, const Sala &s, const std::string &ora,
             const std::string &zi, const std::string &tip_pr, bool ochelari3D, const BazaBilet& tip)
    :nume_client(nume_client), film(f), loc(loc), sala(s), ora(ora), zi(zi), tip_proiectie(tip_pr), ochelari3D(ochelari3D)
    {
        this -> tip_bilet_ptr = tip.clone();
    }

Bilet::Bilet(const Bilet& other)
    : nume_client(other.nume_client), film(other.film), loc(other.loc), sala(other.sala),
      ora(other.ora), zi(other.zi),tip_proiectie(other.tip_proiectie), ochelari3D(other.ochelari3D),
      tip_bilet_ptr(other.tip_bilet_ptr ? other.tip_bilet_ptr->clone() : nullptr) {}

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
        pret_total =  tip_bilet_ptr->calculeaza_pret(PRET_BAZA);
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
    }

    os << "\nPret final: " << b.get_pret_final() << " RON\n";
    return os;
}
