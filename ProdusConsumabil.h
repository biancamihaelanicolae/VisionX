//
// Created by Bianca Mihaela on 18/01/2026.
//

#ifndef OOP_PRODUSCONSUMABIL_H
#define OOP_PRODUSCONSUMABIL_H

#include <string>
#include <iostream>
#include <iomanip>

class ProdusConsumabil {
protected:
    std::string nume;
    double pretBaza;
public:
    ProdusConsumabil(std::string n, double p);
    virtual ~ProdusConsumabil() = default;

    virtual double calculeazaPret() const = 0;
    virtual std::string getDetalii() const = 0;

    const std::string& getNume() const;
};

class Snack : public ProdusConsumabil {
private:
    std::string marime;
public:
    Snack(std::string& n, double p, std::string& m);
    double calculeazaPret() const override;
    std::string getDetalii() const override;
};

class Bautura : public ProdusConsumabil {
private:
    bool areGheata;
public:
    Bautura(std::string& n, double p, bool g);
    double calculeazaPret() const override;
    std::string getDetalii() const override;
};


#endif //OOP_PRODUSCONSUMABIL_H