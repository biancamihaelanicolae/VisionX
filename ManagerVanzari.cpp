//
// Created by Bianca Mihaela on 05/01/2026.
//

#include "ManagerVanzari.h"

void ManagerVanzari::afiseaza_statistici_vanzari() const {
    const auto& statistici = CreareObiect::get_statistici();

    std::cout << "\n===========================================";
    std::cout << "\n       STATISTICI VANZARI VISIONX";
    std::cout << "\n===========================================\n";
    for (const auto& [tip, count] : statistici) {
        std::cout << std::left << std::setw(12) << tip << ": " << count << "\n";
    }
    std::cout << "-------------------------------------------";
    std::cout << "\nTotal bilete create: " << Bilet::getNrBileteVandute();
    std::cout <<"\n===========================================\n";
}

void ManagerVanzari::afiseaza_bilete_utilizator(const std::string &username) const {
    bool gasit = false;
    std::cout << "----Bilete gasite pentru utilizatorul: " << username << "----\n";

    const auto& lista = bilete_cumparate.getReferinta();
    for (size_t i=0; i < bilete_cumparate.nrElemente(); i++) {
        if (lista[i].getNumeClient() == username) {
            std::cout << i + 1 << ". " << lista[i].getFilm().getTitlu()
                      << " [" << lista[i].getZi() << " " << lista[i].getOra()
                      << "] - Loc: " << lista[i].getLoc() << "\n";
            gasit = true;
        }
    }

    if (!gasit) {
        std::cout << "Nu s-au gaist bilete pentru acest nume!\n";
    }
}

double ManagerVanzari::anuleaza_bilete(const std::string &username, std::vector<int> idx, std::vector<Proiectie> &proiectii) {
    double total_refund = 0;
    auto& lista = bilete_cumparate.getReferinta();

    std::sort(idx.begin(), idx.end(), std::greater<int>());

    for (int index : idx) {
        if (index < 0 || (size_t)index >= bilete_cumparate.nrElemente()) continue;

        const Bilet& bilet = lista[index];
        if (bilet.getNumeClient() == username) {
            for (auto& p : proiectii) {
                if (p.getFilm().getTitlu() == bilet.getFilm().getTitlu() &&
                    p.getZi() == bilet.getZi() && p.getOra() == bilet.getOra()) {
                    p.eliberare_loc(bilet.getLoc());
                    break;
                    }
            }

            total_refund += bilet.get_pret_final() * 0.7;
            lista.erase(lista.begin() + index);
        }
    }

    return total_refund;
}

void ManagerVanzari::salvare_bilete_utilizator(const std::string &nume_fisier) const {
    std::ofstream fisier_out(nume_fisier);
    const auto& lista = bilete_cumparate.getReferinta();
    if (fisier_out.is_open()) {
        fisier_out << "----BILETE CUMPARATE DE UTILIZATOR----\n";
        fisier_out<< "---------------------------------------\n";

        if (lista.empty()) {
            fisier_out << "Nu au fost cumparate bilete.\n";
        }else {
            int contor = 1;
            for (const auto& bilet : lista) {
                fisier_out << "Bilet " << contor++ << ":\n";
                fisier_out << bilet << "\n";
                fisier_out<< "---------------------------------------\n";
            }
        }

        fisier_out.close();
        std::cout << "\n Biletele au fost salvate cu succes in   " << nume_fisier << ".\n";
    }else {
        std::cout << "Nu s-a putut deschide fisierul pentru salvare.\n";
    }
}


