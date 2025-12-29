//
// Created by Bianca Mihaela on 29/12/2025.
//

#ifndef OOP_BILETPENSIONAR_H
#define OOP_BILETPENSIONAR_H

#include "BazaBilet.h"

class BiletPensionar : public BazaBilet {
    public:
    BiletPensionar() = default;
    ~BiletPensionar() override = default;

    double calculeaza_pret(double pret_baza) const override {
        return pret_baza * 0.6;;
    }

    BazaBilet* clone() const override {
        return new BiletPensionar(*this);
    }

    void afisare_detalii_tip(std::ostream &os) const override {
        os << "Tip: Pensionar (40% reducere)";
    }
};

#endif //OOP_BILETPENSIONAR_H