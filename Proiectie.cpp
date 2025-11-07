//
// Created by Bianca Mihaela on 07/11/2025.
//

#include "Proiectie.h"

#include <iostream>

Proiectie::Proiectie(const Film &f, const Sala &s, const std::string &z, const std::string &o, const std::string &t):film(f),sala(s),zi(z),ora(o),tip(t) {}

void Proiectie::afisare() const {
    std::cout<< film.getTitlu()<< "|" << " " << sala.getNumar()<< " | " << zi<< " - ora: " << ora<< " | " << "Tip: "<< tip<<std::endl;
}

bool Proiectie::rezervare_multipla(const std::vector<int> &locuri) {
    return sala.rezervare_multipla(locuri);
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
