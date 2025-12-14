#ifndef OOP_BAZABILET_H
#define OOP_BAZABILET_H
#pragma once
#include <iostream>
#include <string>


class BazaBilet {
public:
    virtual double calculeaza_pret(double pret_baza) const = 0;
    virtual BazaBilet* clone() const = 0;
    virtual void afisare_detalii_tip(std::ostream& os) const = 0;
    virtual ~BazaBilet() = default;
};


#endif