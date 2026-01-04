//
// Created by Bianca Mihaela on 04/01/2026.
//

#ifndef OOP_MENIUCONSOLA_H
#define OOP_MENIUCONSOLA_H

#include "Cinema.h"
#include <string>

class MeniuConsola {
private:
    Cinema& cinema;

    void meniuRating();
    void meniuAnulare();
    void meniuFiltre();
    void meniuCumparare();
    std::string citesteComanda();

    public:
    explicit MeniuConsola(Cinema& c);
    void ruleaza();
};


#endif //OOP_MENIUCONSOLA_H