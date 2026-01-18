//
// Created by Bianca Mihaela on 07/11/2025.
//

#include "Cinema.h"
#include "BiletNormal.h"
#include "BiletStudent.h"
#include "BiletElev.h"
#include "BiletPensionar.h"
#include "Exceptii.h"
#include "CreareObiect.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <set>
#include <iterator>
#include <map>

std::vector<Proiectie> Cinema::cauta_film(const std::string &query) const {
    std::vector<Proiectie> rezultat;
    std::string cautare = query;

    std::transform(cautare.begin(), cautare.end(), cautare.begin(), [](unsigned char c){return std::tolower(c);});

    const auto& toate = proiectii.getReferinta();
    for (const auto& p : toate) {
        std::string titlu = p.getFilm().getTitlu();

        std::transform(titlu.begin(), titlu.end(), titlu.begin(), [](unsigned char c){return std::tolower(c);});
        if (titlu.find(cautare) != std::string::npos) {
            rezultat.push_back(p);
        }
    }
    return rezultat;
}


void Cinema::vinde_bilet(const Utilizator &u, Proiectie &p, const std::vector<int> &locuri, bool ochelari = false, double factor_reducere = 1.0, const std::vector<std::string>& snacks, double pret_snacks) {
    if (locuri.empty()) {
        std::cout << "Nu ati specificat niciun loc.\n";
        return;
    }

    BazaBilet* tip_ales = CreareObiect::creeazaBilet(u.getTip());

    if ( !tip_ales->este_valid_la_ora(p.getOra())) {
        std::string mentiune_eroare = "Acest tip de bilet nu este valid la ora " + p.getOra() + "!\n";
        delete tip_ales;
        throw VisionX_Exception(mentiune_eroare);
    }

    double pret_final = tip_ales->calculeaza_pret(Bilet::PRET_BAZA, p.getZi(), ochelari) * factor_reducere;
    std::cout << "Confirmare pret: " << std::fixed << std::setprecision(2) << pret_final << " RON pe bilet.\n";

    try{
        p.rezervare_multipla(locuri);
        std::cout << "\n----BILETE VANDUTE----\n";

        std::vector<std::string> temp_snacks = snacks;
        double temp_pret_snacks = pret_snacks;

        for (int loc : locuri) {
            Bilet b(u.getUsername(), p.getFilm(), loc, p.getSala(), p.getOra(), p.getZi(), p.getTip(), ochelari, *tip_ales, temp_snacks, temp_pret_snacks);
            m_vanzari.adauga_bilet_vandut(b);
            Bilet::marcheaza_vanzare();

            temp_snacks.clear();
            temp_pret_snacks = 0;
        }

        actualizare_sala_originala(p);
        delete tip_ales;
    }catch (const VisionX_Exception& e) {
        std::cerr << e.what() << '\n';
        delete tip_ales;
        throw;
    }
}

std::vector<Proiectie> Cinema::genereaza_sugestii(const CriteriiCautare &c, size_t limita) const {
    std::vector<Proiectie> candidate = filtrare_smart(c);

    if (candidate.empty()) return{};

    std::map<std::string, Proiectie> filme_unice;
    for (const auto& p : candidate) {
        filme_unice.insert({p.getFilm().getTitlu(), p});
    }

    std::vector<std::pair<double, Proiectie>> ratinguri_sortate;
    for (auto const& [titlu, proiectie] : filme_unice) {
        double medie = m_rating.calculeaza_medie_film(titlu);
        ratinguri_sortate.push_back({medie, proiectie});
    }

    std::sort(ratinguri_sortate.begin(), ratinguri_sortate.end(),
        [](const std::pair<double, Proiectie>& a, const std::pair<double, Proiectie>& b) {
            return a.first > b.first;
        });

    std::vector<Proiectie> rezultate;
    for (size_t i = 0; i < std::min(limita, ratinguri_sortate.size()); i++) {
        rezultate.push_back(ratinguri_sortate[i].second);
    }

    return rezultate;
}

std::vector<Proiectie> Cinema::filtrare_smart(const CriteriiCautare& c) const {
    std::vector<Proiectie> rezultat;
    const auto& toate = proiectii.getReferinta();

    for (const auto& p : toate) {
        bool match = true;

        if (!c.zi.empty() && p.getZi() != c.zi) {match = false;}
        if (!c.tip_proiectie.empty() && p.getTip() != c.tip_proiectie) {match = false;}
        if (c.durata_maxima > 0 && p.getFilm().getDurata() > c.durata_maxima) {match = false;}

        if (match) {rezultat.push_back(p);}
    }
    return rezultat;
}

void Cinema::aplica_reguli_sarbatori() {
    static const std::map<std::string, std::string> reguli ={
        {"31_Octombrie", "Horror"},
        {"24_Decembrie", "Craciun"},
        {"25_Decembrie", "Craciun"}
    };

    auto& ref = proiectii.getReferinta();
    for (auto it = ref.begin(); it!= ref.end(); ) {
        auto regula_gasita = reguli.find(it->getZi());

        if (regula_gasita != reguli.end()) {
            if (it->getFilm().getGen() != regula_gasita->second) {
                it = ref.erase(it);
                continue;
            }
        }
        ++it;
    }
}

std::vector<Proiectie> Cinema::filtreaza_pentru_copii() const {
    std::vector<Proiectie > proiectie_copii;
    const auto& ref = proiectii.getReferinta();


    std::copy_if(ref.begin(), ref.end(),std::back_inserter(proiectie_copii),[](const Proiectie& p) {
        return p.getFilm().esteAnimatie();
    });

    return proiectie_copii;
}

void Cinema::incarca_din_fisier(const std::string &nume_fisier) {
    std::ifstream fin(nume_fisier);
    if (!fin.is_open()) {
        throw Eroare_Fisier(nume_fisier);
    }

    std::string titlu, gen, zi, ora, tip;
    int durata, nr_sala, capacitate;

    bool este_animatie;
    while (fin >> titlu >> gen >> durata >> zi >> ora >> tip >> nr_sala >> capacitate >> este_animatie) {
        Film f(titlu, gen, durata, este_animatie);
        Sala s(nr_sala, capacitate);
        proiectii.adauga(Proiectie(f, s, zi, ora, tip)) ;
    }
    fin.close();
}

std::vector<Proiectie> Cinema::get_program_sortat() const {
    std::vector<Proiectie> program_sortat = proiectii.getReferinta();

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
    const auto& ref = proiectii.getReferinta();
    std::copy_if(ref.begin(), ref.end(), std::back_inserter(lista_filtrata),[&gen_cautat](const Proiectie &p) {
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
    if (index <  0 || (size_t)index >= proiectii.nrElemente()) {
        return nullptr;
    }
    return &proiectii.getReferinta()[index];
}



void Cinema::actualizare_sala_originala(const Proiectie &proiectie_modificata) {
    auto& toate = proiectii.getReferinta();
    auto it = std::find_if(toate.begin(), toate.end(), [&](const Proiectie &p) {
        return p.getFilm().getTitlu() == proiectie_modificata.getFilm().getTitlu() &&
               p.getZi() == proiectie_modificata.getZi() &&
               p.getOra() == proiectie_modificata.getOra();
    });

    if (it != toate.end()) {
        *it = proiectie_modificata;
    }
}

std::vector<std::string> Cinema::get_genuri_disponibile() const {
    std::set<std::string> genuri_unice;

    for (const auto& p : proiectii.getReferinta()) {
        genuri_unice.insert(p.getFilm().getGen());
    }

    return std::vector<std::string>(genuri_unice.begin(), genuri_unice.end());
}

std::ostream & operator<<(std::ostream &os, const Cinema &c) {
    os << "\n==== Programul complet VisionX ====\n";
    if (c.proiectii.nrElemente() == 0) {
        os << "Nu exista proiectii programate.\n";
        return os;
    }

    const auto& lista = c.proiectii.getReferinta();
    for (size_t i = 0; i < lista.size(); ++i) {
        os << i << ". ";
        os << lista[i] << "\n";
    }
    os << "======================================\n";
    return os;
}

void Cinema::afiseaza_meniu_genuri() const {
    std::vector<std::string> genuri = get_genuri_disponibile();
    if (genuri.empty()) {
        std::cout << "Nu exista genuri programate.\n";
        return;
    }

    std::cout << "Genuri disponibile: ";
    afiseazaElementeVector(genuri,  " | ");
}