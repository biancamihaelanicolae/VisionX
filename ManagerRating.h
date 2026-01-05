//
// Created by Bianca Mihaela on 05/01/2026.
//

#ifndef OOP_MANAGERRATING_H
#define OOP_MANAGERRATING_H

#include <string>
#include <map>
#include <vector>

#include "Exceptii.h"

class ManagerRating {
private:
    std::multimap<std::string, std::pair<int, std::string>> rating_filme;

public:
    void salvare_ratinguri(const std::string& nume_fisier) const;
    void incarcare_ratinguri(const std::string& nume_fisier);
    void adauga_rating(const std::string& titlu_film, int nota, const std::string& comentariu);
    void afiseaza_rating_film(const std::string& titlu_film) const;
    double calculeaza_medie_film(const std::string& titlu_film) const;
};


#endif //OOP_MANAGERRATING_H