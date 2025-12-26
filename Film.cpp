//
// Created by Bianca Mihaela on 07/11/2025.
//

#include "Film.h"
#include <iostream>

Film::Film(const std::string &t, const std::string &g, int d, bool anim, bool m3D):titlu(t),gen(g),durata(d),animatie(anim), este3D(m3D){}

Film::Film(const Film &f):titlu(f.titlu),gen(f.gen),durata(f.durata),animatie(f.animatie), este3D(f.este3D) {}

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

bool Film::getEste3D() const {return este3D; }

bool Film::esteAnimatie() const {return animatie;}

const std::string & Film::getTitlu() const {  return titlu;  }

const std::string & Film::getGen() const {  return gen;  }

std::ostream & operator<<(std::ostream &os, const Film &f) {
    os << f.titlu << " (" << f.gen << ", " << f.durata<<" min, Animatie: " << (f.animatie? "Da" : "Nu") << ")" << (f.este3D ? " [3D]" : " [2D]");
    return os;
}
