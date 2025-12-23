//
// Created by Bianca Mihaela on 07/11/2025.
//

#include "Sala.h"
#include "Exceptii.h"

#include <algorithm>
#include <iostream>
#include <iomanip>


Sala::Sala(int n, int cap): numar(n), capacitate(cap), locuri_ocupate(cap,false) {}

Sala::Sala(const Sala &s) = default;

Sala & Sala::operator=(const Sala &s) = default;

Sala::~Sala() = default;

bool Sala::rezervare_multipla(const std::vector<int> &locuri) {
    if (locuri.empty()) {
        return true;
    }

    for (int l : locuri) {
        if (l < 1 || l > capacitate || locuri_ocupate[l-1]) {
            throw Eroare_loc_ocupat(l);
        }
    }

    for (int l : locuri) {
        locuri_ocupate[l-1] = true;
    }

    return true;
}

bool Sala::sala_plina() const {
    return std::ranges::all_of(locuri_ocupate,[](bool ocupat){ return ocupat;});
}

int Sala::locuri_libere() const {
    return (int)std::count(locuri_ocupate.begin(), locuri_ocupate.end(), false);
}

void Sala::vizualizare_locuri() const {
    std::cout << "\n----- Harta Salii " << numar << " ( Locuri libere: " << locuri_libere() << " din " << capacitate << " )-----\n";

    std::cout << "=================================================================================\n";
    std::cout << "                                  E C R A N\n";
    std::cout << "=================================================================================\n";

    const int locuri_pe_rand = 10;
    for (int i = 0; i< capacitate; i++) {
        std::cout << std::setw(4) << i+1;
        if (locuri_ocupate[i]) {
            std::cout << ANSI_RED << "[X]" << ANSI_RESET;
        }else {
            std::cout << ANSI_GREEN << "[O]" << ANSI_RESET;
        }

        std::cout << " ";

        if ((i+1) % locuri_pe_rand == 0) {
            std::cout << "\n";
        }
    }
    std::cout << "-----------------------------------------------------\n";
}

int Sala::getCapacitate() const { return capacitate; }

int Sala::getNumar() const {return numar; }

std::ostream & operator<<(std::ostream &os, const Sala &s) {
    os << "Sala " << s.numar << ", Capacitate: " << s.capacitate<< ", Locuri libere: "<< s.locuri_libere();
    return os;
}
