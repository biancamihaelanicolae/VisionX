#ifndef OOP_BILETNORMAL_H
#define OOP_BILETNORMAL_H
#include "BazaBilet.h"

class BiletNormal : public BazaBilet{
public:
    BiletNormal() = default;
    ~BiletNormal() override = default;

    double calculeaza_pret(double pret_baza) const override { return pret_baza; }
    BazaBilet* clone() const override { return new BiletNormal(*this); }
    void afisare_detalii_tip(std::ostream& os) const override{os << "Tip: Normal"; }
};


#endif