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
#include "Utilizator.h"
#include "Proiectie.h"
#include "CreareObiect.h"


class ManagerVanzari {
private:
    std::vector<Bilet> bilete_cumparate;

public:
    void adauga_bilet_vandut(const Bilet& b) { bilete_cumparate.push_back(b); }
    void afiseaza_statistici_vanzari() const;
    void salvare_bilete_utilizator(const std::string& nume_fisier) const;
    void afiseaza_bilete_utilizator(const std::string& username) const;
    double anuleaza_bilete(const std::string& username, std::vector<int> idx, std::vector<Proiectie>& proiectii);
    const std::vector<Bilet>& get_bilete() const { return bilete_cumparate; }
};


#endif //OOP_MANAGERVANZARI_H