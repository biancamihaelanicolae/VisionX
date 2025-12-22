//
// Created by Bianca Mihaela on 07/11/2025.
//

#include "Proiectie.h"

#include <iostream>

Proiectie::Proiectie(const Film &f, const Sala &s, const std::string &z, const std::string &o, const std::string &t):film(f),sala(s),zi(z),ora(o),tip(t) {}

bool Proiectie::rezervare_multipla(const std::vector<int> &locuri) {
    return sala.rezervare_multipla(locuri);
}

bool Proiectie::are_locuri_disponibile() const {
    return !sala.sala_plina();
}

int Proiectie::numar_locuri_libere() const {
    return sala.locuri_libere();
}

void Proiectie::afiseaza_harta_sala() const {
    sala.vizualizare_locuri();
}

const std::string & Proiectie::getZi() const {return zi;}

const std::string & Proiectie::getOra() const {return ora;}

const std::string & Proiectie::getTip() const {return tip;}

const Film & Proiectie::getFilm() const {return film;}

const Sala & Proiectie::getSala() const {return sala;}

std::ostream & operator<<(std::ostream &os, const Proiectie &p) {
    os << "Proiectie: " << p.film << "\n"
            << "Sala: " << p.sala << "\n"
            << "Zi: " << p.zi << " - Ora: " << p.ora << " ( " << p.tip << " ) "<<"\n";
    return os;
}
