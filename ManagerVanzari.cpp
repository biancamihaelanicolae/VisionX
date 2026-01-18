//
// Created by Bianca Mihaela on 05/01/2026.
//

#include "ManagerVanzari.h"

#include "Logger.h"

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
            std::cout << lista[i] << "\n";
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
            Logger::getInstance().log("Anulare bilet bilet pentru clientul: " + username);
        }
    }

    return total_refund;
}

void ManagerVanzari::salvare_bilete_utilizator(const std::string &nume_fisier) const {
    std::ofstream fisier_out(nume_fisier, std::ios::app);

    if (fisier_out.is_open()) {
        const auto& lista = bilete_cumparate.getReferinta();
        fisier_out << "----BILETE CUMPARATE DE UTILIZATOR----\n";
        if (lista.empty()) {
            return;
        }
        fisier_out << lista.back() << std::endl;
        fisier_out<< "---------------------------------------\n";

        fisier_out.close();
        std::cout << "\n Biletele au fost salvate cu succes in   " << nume_fisier << ".\n";
        Logger::getInstance().log("Bilet salvat pentru clientul: " + lista.back().getNumeClient());
    }else {
        std::cout << "Nu s-a putut deschide fisierul pentru salvare.\n";
    }
}


