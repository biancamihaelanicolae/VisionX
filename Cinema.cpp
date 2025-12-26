//
// Created by Bianca Mihaela on 07/11/2025.
//

#include "Cinema.h"
#include "BiletNormal.h"
#include "BiletStudent.h"
#include "BiletElev.h"
#include "Exceptii.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <set>
#include <iterator>
#include <map>

void Cinema::aplica_reguli_sarbatori() {
    static const std::map<std::string, std::string> reguli ={
        {"31_Octombrie", "Horror"},
        {"24_Decembrie", "Craciun"},
        {"25_Decembrie", "Craciun"}
    };

    for (auto it = proiectii.begin(); it!= proiectii.end(); ) {
        auto regula_gasita = reguli.find(it->getZi());

        if (regula_gasita != reguli.end()) {
            if (it->getFilm().getGen() != regula_gasita->second) {
                it = proiectii.erase(it);
                continue;
            }
        }
        ++it;
    }
}

std::vector<Proiectie> Cinema::filtreaza_pentru_copii() const {
    std::vector<Proiectie > proiectie_copii;

    std::copy_if(proiectii.begin(), proiectii.end(),std::back_inserter(proiectie_copii),[](const Proiectie& p) {
        return p.getFilm().esteAnimatie();
    });

    return proiectie_copii;
}

void Cinema::incarca_din_fisier(const std::string &nume_fisier) {
    std::ifstream fin(nume_fisier);
    if (!fin) {
        throw Eroare_Fisier(nume_fisier);
    }

    std::string titlu, gen, zi, ora, tip;
    int durata, nr_sala, capacitate;

    bool este_animatie;
    while (fin >> titlu >> gen >> durata >> zi >> ora >> tip >> nr_sala >> capacitate >> este_animatie) {
        Film f(titlu, gen, durata, este_animatie);
        Sala s(nr_sala, capacitate);
        proiectii.emplace_back(f, s, zi, ora, tip);
    }
    fin.close();
}

std::vector<Proiectie> Cinema::get_program_sortat() const {
    std::vector<Proiectie> program_sortat = proiectii;

    std::ranges::sort(program_sortat, [](const Proiectie& a, const Proiectie& b) {
        int ordine_a = Cinema::get_ordinea_zilei(a.getZi());
        int ordine_b = Cinema::get_ordinea_zilei(b.getZi());

        if (ordine_a != ordine_b) {
            return ordine_a < ordine_b;
        }

        return a.getOra() < b.getOra();
    });

    return program_sortat;
}

std::vector<Proiectie> Cinema::filtreaza_pe_gen(const std::string &gen_cautat) const {
    std::vector<Proiectie> lista_filtrata;

    std::copy_if(proiectii.begin(), proiectii.end(), std::back_inserter(lista_filtrata),[&gen_cautat](const Proiectie &p) {
        return p.getFilm().getGen() == gen_cautat;
    });

    std::ranges::sort(lista_filtrata, [](const Proiectie& a, const Proiectie& b) {
        int ordine_a = Cinema::get_ordinea_zilei(a.getZi());
        int ordine_b = Cinema::get_ordinea_zilei(b.getZi());

        if (ordine_a != ordine_b) {
            return ordine_a < ordine_b;
        }

        return a.getOra() < b.getOra();
    });

    return lista_filtrata;
}

Proiectie * Cinema::get_proiectie(int index) {
    if (index >= 0 && index < (int)proiectii.size()) {
        return &proiectii[index];
    }
    return nullptr;
}

void Cinema::vinde_bilet(const Utilizator &u, Proiectie &p, const std::vector<int> &locuri, bool ochelari = false) {
    if (locuri.empty()) {
        std::cout << "Nu ati specificat niciun loc.\n";
        return;
    }

    BazaBilet* tip_ales = nullptr;
    std::string tip_u = u.getTip();

    if (tip_u == "student") {
        tip_ales = new BiletStudent();
    } else if (tip_u == "elev") {
        tip_ales = new BiletElev();
    } else {
        tip_ales = new BiletNormal();
    }

    try{
        p.rezervare_multipla(locuri);
        std::cout << "\n----BILETE VANDUTE----\n";

        for (int loc : locuri) {
            Bilet b(u.getUsername(), p.getFilm(), loc, p.getSala(), p.getOra(), p.getZi(), p.getTip(), *tip_ales, ochelari);
            bilete_cumparate.push_back(b);
            Bilet::marcheaza_vanzare();
            std::cout<<b<<"\n";
        }

        delete tip_ales;

    }catch (const VisionX_Exception& e) {
        std::cerr << e.what() << '\n';
        delete tip_ales;
        throw;
    }
}

void Cinema::actualizeaza_sala_originala(const Proiectie &proiectie_modificata) {
    auto it = std::find_if(proiectii.begin(), proiectii.end(), [&](const Proiectie &p) {
        return p.getFilm().getTitlu() == proiectie_modificata.getFilm().getTitlu() &&
               p.getZi() == proiectie_modificata.getZi() &&
               p.getOra() == proiectie_modificata.getOra();
    });

    if (it != proiectii.end()) {
        *it = proiectie_modificata;
    }
}

void Cinema::salvare_bilete_utilizator(const std::string& nume_fisier) const {
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

std::vector<std::string> Cinema::get_genuri_disponibile() const {
    std::set<std::string> genuri_unice;

    for (const auto& p:proiectii) {
        genuri_unice.insert(p.getFilm().getGen());
    }

    return std::vector<std::string>(genuri_unice.begin(), genuri_unice.end());
}

std::ostream & operator<<(std::ostream &os, const Cinema &c) {
    os << "\n==== Programul complet VisionX ====\n";
    if (c.proiectii.empty()) {
        std::cout << "Nu exista proiectii programate.\n";
        return os;
    }

    for (size_t i = 0; i < c.proiectii.size(); ++i) {
        os << i << ". ";
        os << c.proiectii[i] << "\n";
    }
    os << "======================================\n";
    return os;
}

void Cinema::afiseaza_statistici_vanzari() const {
    std::map<std::string, int> statistici = {
        {"Normal", 0}, {"Student", 0}, {"Elev", 0}
    };

    for (const auto& bilet : bilete_cumparate) {
        BazaBilet* ptr = bilet.get_tip_ptr();

        if (dynamic_cast<BiletStudent*>(ptr)) {
            statistici["Student"] += 1;
        }else if (dynamic_cast<BiletElev*>(ptr)) {
            statistici["Elev"] += 1;
        }else if (dynamic_cast<BiletNormal*>(ptr)) {
            statistici["Normal"] += 1;
        }
    }

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

void Cinema::afiseaza_meniu_genuri() const {
    std::vector<std::string> genuri = get_genuri_disponibile();
    if (genuri.empty()) {
        std::cout << "Nu exista genuri programate.\n";
        return;
    }

    std::cout << "Genuri disponibile: ";
    for (size_t i = 0; i < genuri.size(); ++i) {
        std::cout << "[" << genuri[i] << "]";
        if (i < genuri.size() - 1) std::cout << ", ";
    }
    std::cout << "\n";
}