//
// Created by Bianca Mihaela on 05/01/2026.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "ManagerRating.h"

void ManagerRating::adauga_rating(const std::string &titlu_film, int nota, const std::string &comentariu) {
    if (nota < 1 || nota > 10) {
        throw ValidationError("Nota trebuie sa fie intre 1 si 10!");
    }

    rating_filme.insert({titlu_film, {nota, comentariu}});
    std::cout << "Rating adaugat cu succes pentru " << titlu_film << "!\n";
}

double ManagerRating::calculeaza_medie_film(const std::string &titlu_film) const {
    auto range = rating_filme.equal_range(titlu_film);
    int count = 0;
    double suma = 0;

    for (auto it = range.first; it != range.second; ++it) {
        suma += it->second.first;
        count ++;
    }

    return (count == 0) ? 0 : (suma/count);
}

void ManagerRating::salvare_ratinguri(const std::string &nume_fisier) const {
    std::ofstream f(nume_fisier);
    if (!f.is_open()) return;

    for (const auto& [titlu,date] : rating_filme) {
        f << titlu << "|" << date.first << "|" << date.second << "\n";
    }

    f.close();
}

void ManagerRating::incarcare_ratinguri(const std::string &nume_fisier) {
    std::ifstream f(nume_fisier);
    if (!f.is_open()) return;

    std::string linie;
    while (std::getline(f, linie)) {
        if (linie.empty()) continue;

        std::stringstream ss(linie);
        std::string titlu, nota_str, comentariu;

        if (std::getline(ss, titlu, '|') &&
            std::getline(ss, nota_str, '|') &&
            std::getline(ss, comentariu)) {
            int nota = std::stoi(nota_str);
            rating_filme.insert({titlu, {nota, comentariu}});
            }
    }

    f.close();
}

void ManagerRating::afiseaza_rating_film(const std::string &titlu_film) const {
    auto range = rating_filme.equal_range(titlu_film);
    std::cout << "\n-----Recenzii pentru " << titlu_film << " (Media: " << std::fixed << std::setprecision(1) << calculeaza_medie_film(titlu_film) << "/10)-----\n";

    bool are_recenzii = false;
    for (auto it = range.first; it != range.second; it++) {
        std::cout << "Nota: " << it->second.first << "/10 | Comentariu: " << it->second.second << "\n";
        are_recenzii = true;
    }

    if (!are_recenzii) {
        std::cout << "Acest film nu are inca recenzii.\n";
    }
}



