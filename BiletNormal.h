#ifndef OOP_BILETNORMAL_H
#define OOP_BILETNORMAL_H
#include "BazaBilet.h"

class BiletNormal : public BazaBilet{
public:
    BiletNormal() = default;
    ~BiletNormal() override = default;

    std::string get_nume_tip() const override {
        return "Normal";
    }

    double calculeaza_pret(double pret_baza, const std::string& , bool ochelari) const override {
        double pret = pret_baza;
        if (ochelari) pret += 5;

        //adaug o taxa de procesare a biletului online, de asemenea taxa adauga si 10 puncte la cardul de fidelitate facut automat la cumpararea biletului
        return pret + 2.0;
    }

    bool este_valid_la_ora(const std::string &) const override {
        return true;
    }

    void tipareste_mentiuni_speciale(std::ostream &os) const override {
        os << "Acest bilet v-a adus 10 puncte VisionX! "
              "Acestea se acumuleaza pe cardul de fidelitate si pot fi folosite pentru diferite promotii sau reduceri!\n";
    }
    BazaBilet* clone() const override { return new BiletNormal(*this); }
    void afisare_detalii_tip(std::ostream& os) const override{os << "Tip: Normal"; }
};


#endif