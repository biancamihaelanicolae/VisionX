//
// Created by Bianca Mihaela on 07/11/2025.
//

#include "Cinema.h"

#include <algorithm>
#include <iostream>

#include "Bilet.h"

double Cinema::calculeaza_pret(const std::string &tip_bilet) const {
    double pret_baza = 30.0;
    if (tip_bilet == "student") return pret_baza * 0.7;
    if (tip_bilet == "elev") return pret_baza * 0.5;
    return pret_baza;
}

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

void Cinema::afiseaza_program() const {
    if (proiectii.empty()) {
        std::cout << "Nu exista proiectii programate.\n";
        return;
    }

    std::vector<Proiectie> program_sortat = proiectii;

    std::ranges::sort(program_sortat, [](const Proiectie& a, const Proiectie& b) {
        int ordine_a = Cinema::get_ordinea_zilei(a.getZi());
        int ordine_b = Cinema::get_ordinea_zilei(b.getZi());

        if (ordine_a != ordine_b) {
            return ordine_a < ordine_b;
        }

        return a.getOra() < b.getOra();
    });

    std::cout << "\n==== Programul cinematografului VisionX ====\n";
    std::string zi_curenta;

    for (size_t i = 0; i < program_sortat.size(); ++i) {
        if (program_sortat[i].getZi() != zi_curenta) {
            zi_curenta = program_sortat[i].getZi();
            std::cout << "\n Ziua:  " << zi_curenta << "\n";
        }

        std::cout << i << ". ";
        program_sortat[i].afisare();
        std::cout << "\n";
    }

    std::cout << "========================================\n";
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

    if (p.rezervare_multipla(locuri)) {
        double pret_unitar = calculeaza_pret(u.getTip());

        std::cout << "\n----BILETE VANDUTE----\n";

        for (int loc : locuri) {
            Bilet b(u.getUsername(), p.getFilm(), p.getSala(), loc, pret_unitar, p.getOra(), p.getZi());
            std::cout << b;
        }
    }else {
        std::cout << "REZERVARE ESUATA: Cel putin unul dintre locurile selectate este invalid!";
    }
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
