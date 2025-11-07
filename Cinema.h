//
// Created by Bianca Mihaela on 07/11/2025.
//

#ifndef OOP_CINEMA_H
#define OOP_CINEMA_H
#include <fstream>
#include <map>
#include <vector>

#include "Proiectie.h"
#include "Utilizator.h"


class Cinema {
    private:
    std::vector<Proiectie> proiectii;

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
    double calculeaza_pret(const std::string& tip_bilet) const;
    void incarca_din_fisier(const std::string& nume_fisier);
    void afiseaza_program() const;
    [[nodiscard]]std::vector<Proiectie> filtreaza_pe_gen(const std::string& gen_cautat) const;
    Proiectie* get_proiectie(int index);
    void vinde_bilet(const Utilizator& u, Proiectie& p, const std::vector<int>& locuri);
    void actualizeaza_sala_originala(const Proiectie& proiectie_modificata);
    friend std::ostream& operator<<(std::ostream& os, const Cinema& c);
};


#endif //OOP_CINEMA_H