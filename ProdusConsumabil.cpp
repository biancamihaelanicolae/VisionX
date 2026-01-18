//
// Created by Bianca Mihaela on 18/01/2026.
//

#include "ProdusConsumabil.h"

ProdusConsumabil::ProdusConsumabil(std::string n, double p) : nume(n), pretBaza(p) {}

const std::string& ProdusConsumabil::getNume() const {
    return nume;
}

Snack::Snack(std::string& n, double p, std::string& m) : ProdusConsumabil(n, p) , marime (m){}

double Snack::calculeazaPret() const {
    if (marime == "Mare") return pretBaza + 10.0;
    if (marime == "Mediu") return pretBaza + 5.0;
    return pretBaza;
}

std::string Snack::getDetalii() const {
    return nume + " (" + marime + ")";
}

Bautura::Bautura(std::string& n, double p, bool g) : ProdusConsumabil(n , p), areGheata(g){}

double Bautura::calculeazaPret() const {
    return pretBaza + 0.5; /// taxa sgr
}

std::string Bautura::getDetalii() const {
    return nume + (areGheata ? " cu gheata" : " fara gheata");
}



