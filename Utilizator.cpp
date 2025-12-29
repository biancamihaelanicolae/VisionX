//
// Created by Bianca Mihaela on 07/11/2025.
//

#include "Utilizator.h"

#include <algorithm>
#include <iostream>

Utilizator::Utilizator() = default;

Utilizator::Utilizator(const std::string &u, const std::string &t):username(u),tip(t) {}

const std::string & Utilizator::getTip() const {return tip;}

const std::string & Utilizator::getUsername() const {return username;}

void Utilizator::citire_utilizator() {
    std::cout << "Introdu numele clientului: ";
    std::getline(std::cin, username);

    std::string temp_tip;

    while (true) {
        std::cout<< "Introdu tipul biletului dorit (normal/student/elev/pensionar): ";

        if (!(std::cin >> temp_tip)) {
            std::cin.clear();
            std::cout << "Citirea tipului de bilet a esuat!";
            tip="normal";
            break;
        }

        std::transform(temp_tip.begin(), temp_tip.end(), temp_tip.begin(), [](unsigned char c) {
            return std::tolower(c);
        });

        if (temp_tip == "normal" || temp_tip == "student" || temp_tip == "elev" || temp_tip == "pensionar") {
            tip = temp_tip;
            break;
        } else {
            std::cout << "Tipul biletului este invalid! Te rog introdu 'normal', 'student', 'elev' sau 'pensionar'.\n";
        }
    }
}
