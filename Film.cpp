//
// Created by Bianca Mihaela on 07/11/2025.
//

#include "Film.h"
#include <iostream>

Film::Film(const std::string &t, const std::string &g, int d):titlu(t),gen(g),durata(d) {}

Film::Film(const Film &f):titlu(f.titlu),gen(f.gen),durata(f.durata) {}

Film & Film::operator=(const Film &f) {
    if (this != &f) {
        titlu = f.titlu;
        gen = f.gen;
        durata = f.durata;
    }
    return *this;
}

Film::~Film() {}

const std::string & Film::getTitlu() const {  return titlu;  }

const std::string & Film::getGen() const {  return gen;  }

std::ostream & operator<<(std::ostream &os, const Film &f) {
    os << f.titlu << " (" << f.gen << ", " << f.durata<<" min)";
    return os;
}
