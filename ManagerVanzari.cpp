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

    for (size_t i=0; i < bilete_cumparate.size(); i++) {
        if (bilete_cumparate[i].getNumeClient() == username) {
            std::cout << i + 1 << ". " << bilete_cumparate[i].getFilm().getTitlu()
                      << " [" << bilete_cumparate[i].getZi() << " " << bilete_cumparate[i].getOra()
                      << "] - Loc: " << bilete_cumparate[i].getLoc() << "\n";
            gasit = true;
        }
    }

    if (!gasit) {
        std::cout << "Nu s-au gaist bilete pentru acest nume!\n";
    }
}

double ManagerVanzari::anuleaza_bilete(const std::string &username, std::vector<int> idx, std::vector<Proiectie> &proiectii) {
    double total_refund = 0;

    std::sort(idx.begin(), idx.end(), std::greater<int>());

    for (int index : idx) {
        if (index < 0 || index >= (int)bilete_cumparate.size()) continue;

        const Bilet& bilet = bilete_cumparate[index];
        if (bilet.getNumeClient() == username) {
            for (auto& p : proiectii) {
                if (p.getFilm().getTitlu() == bilet.getFilm().getTitlu() &&
                    p.getZi() == bilet.getZi() && p.getOra() == bilet.getOra()) {
                    p.eliberare_loc(bilet.getLoc());
                    break;
                    }
            }

            total_refund += bilet.get_pret_final() * 0.7;
            bilete_cumparate.erase(bilete_cumparate.begin() + index);
        }
    }

    return total_refund;
}

void ManagerVanzari::salvare_bilete_utilizator(const std::string &nume_fisier) const {
    std::ofstream fisier_out(nume_fisier);

    if (fisier_out.is_open()) {
        fisier_out << "----BILETE CUMPARATE DE UTILIZATOR----\n";
        fisier_out<< "---------------------------------------\n";

        if (bilete_cumparate.empty()) {
            fisier_out << "Nu au fost cumparate bilete.\n";
        }else {
            for (size_t i=0; i < bilete_cumparate.size(); ++i) {
                fisier_out << "Bilet " << i+1 << ":\n";
                fisier_out << bilete_cumparate[i] << "\n";
                fisier_out<< "---------------------------------------\n";
            }
        }

        fisier_out.close();
        std::cout << "\n Biletele au fost salvate cu succes in   " << nume_fisier << ".\n";
    }else {
        std::cout << "Nu s-a putut deschide fisierul pentru salvare.\n";
    }
}


