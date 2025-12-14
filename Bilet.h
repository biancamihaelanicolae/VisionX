#ifndef OOP_BILET_H
#define OOP_BILET_H
#include <string>
#include <iostream>
#include <algorithm>

#include "Film.h"
#include "Sala.h"
#include "BazaBilet.h"


class Bilet {
private:
    std::string nume_client;
    Film film;
    int loc;
    Sala sala;
    std::string ora;
    std::string zi;

    BazaBilet* tip_bilet_ptr = nullptr;

    static constexpr double PRET_BAZA = 30.0;

public:
    Bilet(const std::string& nume_client, const Film& f, int loc, const Sala& s, const std::string& ora, const std::string& zi, const BazaBilet& tip);
    Bilet(const Bilet& other);
    Bilet& operator=(Bilet other);
    ~Bilet();
    friend void swap(Bilet& first, Bilet& second) noexcept;
    [[nodiscard]] double get_pret_final() const;
    friend std::ostream& operator<<(std::ostream& os, const Bilet& b);
};


#endif //OOP_BILET_H