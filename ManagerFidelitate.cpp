//
// Created by Bianca Mihaela on 17/01/2026.
//

#include "ManagerFidelitate.h"
#include <fstream>

ManagerFidelitate::ManagerFidelitate() {incarca_puncte();}
ManagerFidelitate::~ManagerFidelitate() {salveaza_puncte();}

void ManagerFidelitate::incarca_puncte() {
    std::ifstream f(fisier_puncte);
    std::string nume;
    int p;
    while (f >> nume >> p) {
        baza_puncte[nume] = p;
    }
}

void ManagerFidelitate::salveaza_puncte() const {
    std::ofstream f(fisier_puncte);
    if (f.is_open()) {
        for (auto const& [nume, puncte] : baza_puncte) {
            f << nume << " " << puncte << "\n";
        }

        f.close();
    }
}

bool ManagerFidelitate::are_card(const std::string &username) const {
    return baza_puncte.find(username) != baza_puncte.end();
}

int ManagerFidelitate::get_puncte(const std::string &username) const {
    auto it = baza_puncte.find(username);
    return (it != baza_puncte.end()) ? it->second : 0;
}

void ManagerFidelitate::creeaza_card(const std::string &username) {
    baza_puncte[username] = 0;
    salveaza_puncte();
}

void ManagerFidelitate::adauga_puncte(const std::string &username, int puncte) {
    baza_puncte[username] += puncte;
    salveaza_puncte();
}

void ManagerFidelitate::consuma_puncte(const std::string &username, int puncte) {
    baza_puncte[username] -= puncte;
    if (baza_puncte[username] < 0) baza_puncte[username] = 0;
    salveaza_puncte();
}
