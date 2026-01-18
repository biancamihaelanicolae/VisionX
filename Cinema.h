//
// Created by Bianca Mihaela on 07/11/2025.
//

#ifndef OOP_CINEMA_H
#define OOP_CINEMA_H
#include <fstream>
#include <vector>
#include <map>
#include <string>

#include "ManagerFidelitate.h"
#include "GestiuneResurse.h"
#include "ManagerRating.h"
#include "ManagerVanzari.h"
#include "Proiectie.h"
#include "Utilizator.h"

struct CriteriiCautare {
    std::string zi = "";
    std::string tip_proiectie = "";
    int durata_maxima = 0;
};

template <typename T>
void afiseazaElementeVector(const std::vector<T> &v, const std::string& separator = ", ") {
    for (size_t i = 0; i < v.size(); i++) {
        std::cout << v[i];
        if (i < v.size() - 1) std::cout << separator;
    }
    std::cout << "\n";
}

class Cinema {
    private:
    ManagerRating m_rating;
    ManagerVanzari m_vanzari;
    GestiuneResurse<Proiectie> proiectii;
    ManagerFidelitate m_fidelitate;

    Cinema() = default;

    Cinema(const Cinema&) = delete;
    Cinema& operator=(const Cinema&) = delete;

    void actualizare_sala_originala(const Proiectie& p);
    [[nodiscard]] std::vector<std::string> get_genuri_disponibile() const;
    static int get_ordinea_zilei(const std::string& zi)
    {
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

    ManagerFidelitate& get_fidelitate() {return m_fidelitate;}
    [[nodiscard]] std::vector<Proiectie> cauta_film(const std::string& query) const;
    ManagerRating& get_rating(){return m_rating;}
    ManagerVanzari& get_vanzari(){return m_vanzari;}

    static Cinema& getInstance() {
        static Cinema instance;
        return instance;
    }

    void incarca_din_fisier(const std::string& nume_fisier);
    void aplica_reguli_sarbatori();
    void vinde_bilet(const Utilizator& u, Proiectie& p, const std::vector<int>& locuri, bool ochelari, double factor_reducere);

    [[nodiscard]] std::vector<Proiectie> get_program_sortat() const;
    [[nodiscard]] std::vector<Proiectie> filtrare_smart(const CriteriiCautare& c) const;
    [[nodiscard]] std::vector<Proiectie> filtreaza_pe_gen(const std::string& gen_cautat) const;
    [[nodiscard]]std::vector<Proiectie> filtreaza_pentru_copii() const;

    std::vector<Proiectie> genereaza_sugestii(const CriteriiCautare& c, size_t limita = 3) const;
    Proiectie* get_proiectie(int index);
    GestiuneResurse<Proiectie>& get_toate_proiectiile() { return proiectii; }
    void afiseaza_meniu_genuri() const;

    friend std::ostream& operator<<(std::ostream& os, const Cinema& c);
};


#endif //OOP_CINEMA_H