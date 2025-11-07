//
// Created by Bianca Mihaela on 07/11/2025.
//

#include "Bilet.h"

Bilet::Bilet(const std::string &nume, const Film &f, const Sala &s, int l, double p, const std::string &o,
    const std::string &z):nume_client(nume), film(f), loc(l), sala(s), pret(p), ora(o), zi(z) {}

Bilet::Bilet(const Bilet &b) = default;

Bilet & Bilet::operator=(const Bilet &b) = default;

Bilet::~Bilet() = default;

std::ostream & operator<<(std::ostream &os, const Bilet &b) {
    os << "\n---BILET CINEMA---\n"
            << "Client: " << b.nume_client << "\n"
            << "Film: " << b.film << "\n"
            <<"Data/ Ora: "<< b.zi << "/ " << b.ora << "\n"
            << "Sala: " << b.sala << "\n"
            << "Loc: " << b.loc << "\n"
            << "Pret: " << std::fixed << std::setprecision(2) << b.pret << "lei\n"
            <<"---------------------\n";
    return os;
}
