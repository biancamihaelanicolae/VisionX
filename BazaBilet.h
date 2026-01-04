#ifndef OOP_BAZABILET_H
#define OOP_BAZABILET_H
#pragma once
#include <iostream>
#include <string>


class BazaBilet {
public:
    virtual double calculeaza_pret(double pret_baza, const std::string& zi, bool ochelari) const = 0;
    virtual void tipareste_mentiuni_speciale(std::ostream& os) const = 0;
    virtual bool este_valid_la_ora(const std::string& ora) const = 0;
    virtual BazaBilet* clone() const = 0;
    virtual void afisare_detalii_tip(std::ostream& os) const = 0;
    virtual ~BazaBilet() = default;
};


#endif