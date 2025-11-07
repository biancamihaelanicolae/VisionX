//
// Created by Bianca Mihaela on 07/11/2025.
//

#ifndef OOP_BILET_H
#define OOP_BILET_H
#include <string>

#include "Film.h"
#include "Sala.h"


class Bilet {
private:
    std::string nume_client;
    Film film;
    int loc;
    Sala sala;
    double pret;
    std::string ora;
    std::string zi;

public:
    Bilet(const std::string& nume, const Film& f, const Sala& s, int l, double p, const std::string& o, const std::string& z);
    Bilet(const Bilet& b);
    Bilet& operator=(const Bilet& b);
    ~Bilet();
    friend std::ostream& operator<<(std::ostream& os, const Bilet& b);
};


#endif //OOP_BILET_H