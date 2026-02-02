//
// Created by Bianca Mihaela on 07/11/2025.
//

#ifndef OOP_PROIECTIE_H
#define OOP_PROIECTIE_H
#include "Film.h"
#include "Sala.h"
#include <string>
#include <vector>
#include <iostream>

class Proiectie {
protected:
    Film film;
    Sala sala;
    std::string zi;
    std::string ora;
    std::string tip; //2D, 3D, IMAX
    double pretBaza;

public:
    Proiectie(const Film& f, const Sala& s, const std::string& z, const std::string& o, const std::string& t);
    virtual ~Proiectie() = default;

    virtual double getPret() const;
    virtual std::string getNumeEveniment() const = 0;
    virtual std::vector<std::string> getBeneficii() const;

    void eliberare_loc(int nr);
    bool are_locuri_disponibile() const;
    void afiseaza_harta_sala() const;
    void afiseaza_detalii_disponibilitate()const;
    bool rezervare_multipla(const std::vector<int>& locuri);

    [[nodiscard]]const std::string& getZi() const;
    [[nodiscard]]const std::string& getOra() const;
    [[nodiscard]]const std::string& getTip() const;
    [[nodiscard]]const Film& getFilm() const;
    [[nodiscard]]const Sala& getSala() const;
    [[nodiscard]]Sala& getSala();
    friend std::ostream& operator<<(std::ostream& os, const Proiectie& p);
};

class ProiectieStandard : public Proiectie {
public:
    using Proiectie::Proiectie;
    std::string getNumeEveniment() const override{return "Proiectie Standard";}
};

class PremieraSpeciala : public Proiectie {
private:
    std::string invitat_special;

public:
    PremieraSpeciala(const Film& f, const Sala& s, const std::string& z, const std::string& o, const std::string& t, const std::string& invitat = "Echipa Filmului");

    [[maybe_unused]]double getPret() const override;
    std::string getNumeEveniment() const override;
    [[maybe_unused]]std::vector<std::string> getBeneficii() const override;
};

class MaratonFilme : public Proiectie {
private:
    int numarFilme;
    std::vector<std::string> titluriFilme;

public:
    MaratonFilme(const Film& f, const Sala& s, const std::string& z, const std::string& o, const std::string& t, int nrF = 3, const std::vector<std::string>& titluri = {"Film 2", "Film 3"});
    double getPret() const override;
    std::string getNumeEveniment() const override;
    std::vector<std::string> getBeneficii() const override;
    const std::vector<std::string>& getListaFilme() const { return titluriFilme; }
};


#endif //OOP_PROIECTIE_H