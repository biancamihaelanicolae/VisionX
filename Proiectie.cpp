//
// Created by Bianca Mihaela on 07/11/2025.
//

#include "Proiectie.h"

#include <iostream>

Proiectie::Proiectie(const Film &f, const Sala &s, const std::string &z, const std::string &o, const std::string &t):film(f),sala(s),zi(z),ora(o),tip(t) {}

bool Proiectie::rezervare_multipla(const std::vector<int> &locuri) {
    return sala.rezervare_multipla(locuri);
}

void Proiectie::eliberare_loc(int nr) {
    sala.elibereaza_loc(nr);
}

bool Proiectie::are_locuri_disponibile() const {
    return !sala.sala_plina();
}

void Proiectie::afiseaza_harta_sala() const {
    sala.vizualizare_locuri();
}

void Proiectie::afiseaza_detalii_disponibilitate()const {
    std::cout << "Locuri libere: " << sala.locuri_libere()
              << " din " << sala.getCapacitate() << "\n";
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
