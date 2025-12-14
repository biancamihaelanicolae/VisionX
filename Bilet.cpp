//
// Created by Bianca Mihaela on 07/11/2025.
//

#include "Bilet.h"

#include <iomanip>

Bilet::Bilet(const std::string &nume_client, const Film &f, const Sala &s, int loc, const std::string &ora,
             const std::string &zi, const BazaBilet& tip)
    :nume_client(nume_client), film(f), sala(s), loc(loc), ora(ora), zi(zi)
    {
        this -> tip_bilet_ptr = tip.clone();
    }

Bilet::Bilet(const Bilet& other)
    : nume_client(other.nume_client), film(other.film), sala(other.sala), loc(other.loc),
      ora(other.ora), zi(other.zi),

      tip_bilet_ptr(other.tip_bilet_ptr ? other.tip_bilet_ptr->clone() : nullptr)
{}

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
    swap(first.tip_bilet_ptr, second.tip_bilet_ptr);
}

Bilet& Bilet::operator = (Bilet other) {
    swap(*this, other);
    return *this;
}

double Bilet::get_pret_final() const {
    if (tip_bilet_ptr) {
        return tip_bilet_ptr->calculeaza_pret(PRET_BAZA);
    }

    return PRET_BAZA;
}

std::ostream & operator<<(std::ostream &os, const Bilet &b) {
    os << "\n---BILET CINEMA---\n"
            << "Client: " << b.nume_client << "\n"
            << "Film: " << b.film.getTitlu() << "\n"
            <<"Data/ Ora: "<< b.zi << "/ " << b.ora << "\n"
            << "Sala: " << b.sala.getNumar() << "\n"
            << "Loc: " << b.loc << "\n"
            <<"---------------------\n";

    if (b.tip_bilet_ptr) {
        b.tip_bilet_ptr->afisare_detalii_tip(os);
    }

    os << "Pret final: " << b.get_pret_final() << " RON\n";
    return os;
}
