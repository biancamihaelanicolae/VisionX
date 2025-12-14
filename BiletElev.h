#ifndef OOP_BILETELEV_H
#define OOP_BILETELEV_H
#include "BazaBilet.h"


class BiletElev : public BazaBilet{
public:
    BiletElev() = default;
    ~BiletElev() = default;

    double calculeaza_pret(double pret_baza) const override { return pret_baza*0.50; }
    BazaBilet* clone() const override { return new BiletElev(*this); }
    void afisare_detalii_tip(std::ostream& os) const override{os << "Tip: Elev (50% reducere)"; }
};


#endif