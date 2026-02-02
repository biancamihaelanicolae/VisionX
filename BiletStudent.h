#ifndef OOP_BILETSTUDENT_H
#define OOP_BILETSTUDENT_H
#include "BazaBilet.h"

class BiletStudent : public BazaBilet{
public:
    BiletStudent() = default;
    ~BiletStudent() override = default;

    std::string get_nume_tip() const override {
        return "Student";
    }

    //reducere 30% in timpul saptamanii
    //reducere 10% in weekend
    double calculeaza_pret(double pret_baza, const std::string& zi, bool ochelari) const override {
        double m = (zi == "Sambata" || zi == "Duminica" || zi == "Vineri") ? 0.9 : 0.7;
        return (pret_baza * m) + (ochelari ? 5 : 0);
    }

    bool este_valid_la_ora(const std::string &) const override{ return true; }

    void tipareste_mentiuni_speciale(std::ostream &os) const override {
        os << "Reducerea de 30% va fi valabila de Luni pana Joi. Pe weekend se va aplica o reducere de 10%!\n";
    }

    BazaBilet* clone() const override { return new BiletStudent(*this); }
    void afisare_detalii_tip(std::ostream& os) const override {
        os << "Tip: Student ";
    }
};


#endif