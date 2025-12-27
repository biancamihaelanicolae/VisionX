#ifndef OOP_BILET_H
#define OOP_BILET_H
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

    BazaBilet* tip_bilet_ptr = nullptr;

    static constexpr double PRET_BAZA = 30.0;

public:
    [[nodiscard]]const std::string& getNumeClient() const;
    [[nodiscard]]int getLoc() const;
    [[nodiscard]]const Film& getFilm() const;
    [[nodiscard]]const std::string& getOra() const;
    [[nodiscard]]const std::string& getZi() const;

    static void marcheaza_vanzare();
    static int getNrBileteVandute();
    BazaBilet* get_tip_ptr() const {return tip_bilet_ptr;}
    Bilet(const std::string& nume_client, const Film& f, int loc, const Sala& s, const std::string& ora, const std::string& zi, const std::string& tip_pr, bool ochelari3D, const BazaBilet& tip);
    Bilet(const Bilet& other);
    Bilet& operator=(Bilet other);
    ~Bilet();
    friend void swap(Bilet& first, Bilet& second) noexcept;
    [[nodiscard]] double get_pret_final() const;
    friend std::ostream& operator<<(std::ostream& os, const Bilet& b);
};


#endif //OOP_BILET_H