#ifndef OOP_BILETELEV_H
#define OOP_BILETELEV_H
#include "BazaBilet.h"


class BiletElev : public BazaBilet{
public:
    BiletElev() = default;
    ~BiletElev() override= default;

    std::string get_nume_tip() const override {
        return "Elev";
    }

    double calculeaza_pret(double pret_baza, const std::string& , bool ochelari) const override {
        double pret = pret_baza * 0.5;
        if (ochelari) pret += 5;
        return pret;
    }

    //Reducerea de elevi va fi restrictionata pana in ora 22:00
    bool este_valid_la_ora(const std::string &ora) const override {
        return ora < "22:00";
    }

    void tipareste_mentiuni_speciale(std::ostream& os) const override {
        os << "[ATENTIE] Este necesar carnetul de elev vizat la intrare pentru a aplica reducerea!\n"
           << "[RESTRICTIE] Reducerea este invalida dupa ora 22:00 (pentru proiectii de noapte)!\n";
    }

    BazaBilet* clone() const override { return new BiletElev(*this); }
    void afisare_detalii_tip(std::ostream& os) const override{os << "Tip: Elev (50% reducere)"; }
};


#endif