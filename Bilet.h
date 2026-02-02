#ifndef OOP_BILET_H
#define OOP_BILET_H
#include <memory>
#include <string>

#include "Film.h"
#include "Sala.h"
#include "BazaBilet.h"


class Bilet {
private:
    static int nr_bilete_vandute;
    std::string nume_client;
    Film film;
    int loc;
    Sala sala;
    std::string ora;
    std::string zi;
    std::string tip_proiectie;
    bool ochelari3D;
    std::vector<std::string> snacks_cumparate;
    double cost_snacks;

    std::unique_ptr<BazaBilet> tip_bilet_ptr = nullptr;

public:
    Bilet(const std::string& nume, const Film& f, int loc, const Sala& s, const std::string& ora, const std::string& zi, const std::string& tip_pr,
        bool ochelari, const BazaBilet& tip, const std::vector<std::string>& snacks_alese = {},  double p_snacks = 0.0);

    static constexpr double PRET_BAZA = 30.0;
    [[nodiscard]]const std::string& getNumeClient() const;
    [[nodiscard]]int getLoc() const;
    [[nodiscard]]const Film& getFilm() const;
    [[nodiscard]]const std::string& getOra() const;
    [[nodiscard]]const std::string& getZi() const;

    static void marcheaza_vanzare();
    static int getNrBileteVandute();
    Bilet(const std::string& nume_client, const Film& f, int loc, const Sala& s, const std::string& ora, const std::string& zi, const std::string& tip_pr, bool ochelari3D, const BazaBilet& tip);
    Bilet(const Bilet& other);
    Bilet& operator=(const Bilet& other);
    ~Bilet() = default;
    friend void swap(Bilet& first, Bilet& second) noexcept;
    [[nodiscard]] double get_pret_final() const;
    friend std::ostream& operator<<(std::ostream& os, const Bilet& b);
};


#endif //OOP_BILET_H