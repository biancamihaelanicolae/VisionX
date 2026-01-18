//
// Created by Bianca Mihaela on 18/01/2026.
//

#include "ManagerProduse.h"

ManagerProduse::ManagerProduse() : total_plata(0.0) {
    meniu.push_back(std::make_unique<Snack>("Popcorn", 15.0, "Mare"));
    meniu.push_back(std::make_unique<Snack>("Popcorn", 11.0, "Mediu"));
    meniu.push_back(std::make_unique<Snack>("Nachos", 18.0, "Mediu"));
    meniu.push_back(std::make_unique<Snack>("Nachos", 21.0, "Mare"));
    meniu.push_back(std::make_unique<Bautura>("Coca-Cola", 10.0, true));
    meniu.push_back(std::make_unique<Bautura>("Coca-Cola Zero", 10.0, true));
    meniu.push_back(std::make_unique<Bautura>("Fanta", 10.0, false));
    meniu.push_back(std::make_unique<Bautura>("Fuzetea de piersici", 8.0, false));
    meniu.push_back(std::make_unique<Bautura>("Apa Plata", 7.0, false));
}

void ManagerProduse::afiseazaMeniu() const {
    std::cout << "\n========= GUSTARI SI BAUTURI VISIONX =========\n";
    for (size_t i = 0; i < meniu.size(); ++i) {
        std::cout << i << ". " << std::left << std::setw(20) << meniu[i]->getDetalii()
                  << " | Pret: " << meniu[i]->calculeazaPret() << " RON\n";
    }
    std::cout << "==============================================\n";
}

void ManagerProduse::adaugaInCos(int index) {
    if (index >= 0 && (size_t)index < meniu.size()) {
        total_plata += meniu[index]->calculeazaPret();
        cos_nume.push_back(meniu[index]->getDetalii());
        std::cout << "S-a adaugat in cos: " << meniu[index]->getDetalii() << "\n";
    } else {
        std::cout << "Index produs invalid!\n";
    }
}

double ManagerProduse::getTotal() const { return total_plata; }

void ManagerProduse::reseteazaCos() {
    cos_nume.clear();
    total_plata = 0.0;
}

const std::vector<std::string>& ManagerProduse::getCos() const { return cos_nume; }