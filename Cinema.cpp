//
// Created by Bianca Mihaela on 07/11/2025.
//

#include "Cinema.h"
#include "BiletNormal.h"
#include "BiletStudent.h"
#include "BiletElev.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <set>

void Cinema::incarca_din_fisier(const std::string &nume_fisier) {
    std::ifstream fin(nume_fisier);
    if (!fin) {
        std::cerr << "Eroare la deschiderea fisierului " << nume_fisier << "\n";
        return;
    }

    std::string titlu, gen, zi, ora, tip;
    int durata, nr_sala, capacitate;

    while (fin >> titlu >> gen >> durata >> zi >> ora >> tip >> nr_sala >> capacitate) {
        Film f(titlu, gen, durata);
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
    for (const auto& p : proiectii) {
        if (p.getFilm().getGen() == gen_cautat) {
            lista_filtrata.push_back(p);
        }
    }

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

void Cinema::vinde_bilet(const Utilizator &u, Proiectie &p, const std::vector<int> &locuri) {
    if (locuri.empty()) {
        std::cout << "Nu ati specificat niciun loc.\n";
        return;
    }

    BazaBilet* tip_ales = nullptr;
    std::string tip_u = u.getTip();

    if (tip_u == "student") {
        tip_ales = new BiletStudent(); // sau tip_ales = new BiletStudent;
    } else if (tip_u == "elev") {
        tip_ales = new BiletElev();
    } else {
        tip_ales = new BiletNormal();
    }

    if (p.rezervare_multipla(locuri)) {
        std::cout << "\n----BILETE VANDUTE----\n";

        for (int loc : locuri) {
            Bilet b(u.getUsername(), p.getFilm(), loc, p.getSala(), p.getOra(), p.getZi(), *tip_ales);
            bilete_cumparate.push_back(b);
            std::cout << b;
        }
    }else {
        std::cout << "REZERVARE ESUATA: Cel putin unul dintre locurile selectate este invalid!";
    }

    delete tip_ales;
}

void Cinema::actualizeaza_sala_originala(const Proiectie &proiectie_modificata) {
    for (auto& p_originala : proiectii) {
        if (p_originala.getFilm().getTitlu() == proiectie_modificata.getFilm().getTitlu() &&
            p_originala.getZi() == proiectie_modificata.getZi() &&
            p_originala.getOra() == proiectie_modificata.getOra())
        {
            p_originala = proiectie_modificata;
            break;
        }
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
