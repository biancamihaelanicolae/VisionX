//
// Created by Bianca Mihaela on 07/11/2025.
//

#include "Film.h"
#include <iostream>

Film::Film(const std::string &t, const std::string &g, int d, bool anim):titlu(t),gen(g),durata(d),animatie(anim){}

Film::Film(const Film &f):titlu(f.titlu),gen(f.gen),durata(f.durata),animatie(f.animatie) {}

Film & Film::operator=(const Film &f) {
    if (this != &f) {
        titlu = f.titlu;
        gen = f.gen;
        durata = f.durata;
        animatie = f.animatie;
    }
    return *this;
}

Film::~Film() {}

bool Film::esteAnimatie() const {return animatie;}

const std::string & Film::getTitlu() const {  return titlu;  }

const std::string & Film::getGen() const {  return gen;  }

int Film::getDurata() const {  return durata;  }

std::ostream & operator<<(std::ostream &os, const Film &f) {
    os << f.titlu << " (" << f.gen << ", " << f.durata<<" min, Animatie: " << (f.animatie? "Da" : "Nu") << ")" ;
    return os;
}
