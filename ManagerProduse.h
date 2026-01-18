//
// Created by Bianca Mihaela on 18/01/2026.
//

#ifndef OOP_MANAGERPRODUSE_H
#define OOP_MANAGERPRODUSE_H

#include <vector>
#include <memory>
#include "ProdusConsumabil.h"

class ManagerProduse {
private:
    std::vector<std::unique_ptr<ProdusConsumabil>> meniu;
    std::vector<std::string> cos_nume;
    double total_plata;

public:
    ManagerProduse();
    void afiseazaMeniu() const;
    void adaugaInCos(int index);
    double getTotal() const;
    void reseteazaCos();
    const std::vector<std::string>& getCos() const;
};

#endif //OOP_MANAGERPRODUSE_H