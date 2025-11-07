//
// Created by Bianca Mihaela on 07/11/2025.
//

#ifndef OOP_PROIECTIE_H
#define OOP_PROIECTIE_H
#include "Film.h"
#include "Sala.h"


class Proiectie {
private:
    Film film;
    Sala sala;
    std::string zi;
    std::string ora;
    std::string tip; //2D, 3D, IMAX

public:
    Proiectie(const Film& f, const Sala& s, const std::string& z, const std::string& o, const std::string& t);
    void afisare() const;
    bool rezervare_multipla(const std::vector<int>& locuri);
    [[nodiscard]]const std::string& getZi() const;
    [[nodiscard]]const std::string& getOra() const;
    [[nodiscard]]const std::string& getTip() const;
    [[nodiscard]]const Film& getFilm() const;
    [[nodiscard]]const Sala& getSala() const;
    friend std::ostream& operator<<(std::ostream& os, const Proiectie& p);
};


#endif //OOP_PROIECTIE_H