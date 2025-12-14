#ifndef OOP_BILETSTUDENT_H
#define OOP_BILETSTUDENT_H
#include "BazaBilet.h"

class BiletStudent : public BazaBilet{
public:
    BiletStudent() = default;
    ~BiletStudent() override = default;

    double calculeaza_pret(double pret_baza) const override { return pret_baza*0.70; }
    BazaBilet* clone() const override { return new BiletStudent(*this); }
    void afisare_detalii_tip(std::ostream& os) const override{os << "Tip: Student (30% reducere)"; }
};


#endif