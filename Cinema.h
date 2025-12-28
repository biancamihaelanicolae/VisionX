//
// Created by Bianca Mihaela on 07/11/2025.
//

#ifndef OOP_CINEMA_H
#define OOP_CINEMA_H
#include <fstream>
#include <map>
#include <vector>
#include "Bilet.h"
#include "Proiectie.h"
#include "Utilizator.h"

struct CriteriiCautare {
    std::string zi = "";
    std::string tip_proiectie = "";
    int durata_maxima = 0;
};

class Cinema {
    private:
    std::multimap<std::string, std::pair<int, std::string>> rating_filme;
    std::vector<Proiectie> proiectii;
    std::vector<Bilet> bilete_cumparate;

    static int get_ordinea_zilei(const std::string& zi) {
        static const std::map<std::string, int > ordine ={
            {"Luni", 0}, {"Marti", 1}, {"Miercuri", 2}, {"Joi", 3},
            {"Vineri", 4}, {"Sambata", 5}, {"Duminica", 6}
        };

        auto it = ordine.find(zi);
        if (it != ordine.end()) {
            return it->second;
        }

        return 99;
    }

    public:
    void adauga_rating(const std::string& titlu_film, int nota, const std::string& comentariu);
    void afiseaza_rating_film(const std::string& titlu_film) const;
    double calculeaza_medie_film(const std::string& titlu_film) const;
    void afiseaza_bilete_utilizator(const std::string& username) const;
    double anuleaza_bilete(const std::string& username, std::vector<int> idx);
    std::vector<Proiectie> filtrare_smart(const CriteriiCautare& c) const;
    void aplica_reguli_sarbatori();
    [[nodiscard]]std::vector<Proiectie> filtreaza_pentru_copii() const;
    double calculeaza_pret(const std::string& tip_bilet) const;
    void incarca_din_fisier(const std::string& nume_fisier);
    [[nodiscard]]std::vector<Proiectie> filtreaza_pe_gen(const std::string& gen_cautat) const;
    Proiectie* get_proiectie(int index);
    void vinde_bilet(const Utilizator& u, Proiectie& p, const std::vector<int>& locuri, bool ochelari);
    void actualizeaza_sala_originala(const Proiectie& proiectie_modificata);
    void salvare_bilete_utilizator(const std::string& nume_fisier) const;
    [[nodiscard]] std::vector<std::string> get_genuri_disponibile() const;
    [[nodiscard]] std::vector<Proiectie> get_program_sortat() const;
    friend std::ostream& operator<<(std::ostream& os, const Cinema& c);
    void afiseaza_statistici_vanzari() const;
    void afiseaza_meniu_genuri() const;
};


#endif //OOP_CINEMA_H