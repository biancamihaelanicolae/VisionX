//
// Created by Bianca Mihaela on 05/01/2026.
//

#ifndef OOP_MANAGERVANZARI_H
#define OOP_MANAGERVANZARI_H

#include <vector>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include "Bilet.h"
#include "GestiuneResurse.h"
#include "Proiectie.h"
#include "CreareObiect.h"


class ManagerVanzari {
private:
    GestiuneResurse<Bilet> bilete_cumparate;

public:
    void adauga_bilet_vandut(const Bilet& b) { bilete_cumparate.adauga(b); }
    void afiseaza_statistici_vanzari() const;
    void salvare_bilete_utilizator(const std::string& nume_fisier) const;
    void afiseaza_bilete_utilizator(const std::string& username) const;
    double anuleaza_bilete(const std::string& username, std::vector<int> idx, std::vector<Proiectie>& proiectii);
};


#endif //OOP_MANAGERVANZARI_H