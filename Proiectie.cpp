//
// Created by Bianca Mihaela on 07/11/2025.
//

#include "Proiectie.h"

#include <algorithm>

//clasa baza
Proiectie::Proiectie(const Film &f, const Sala &s, const std::string &z, const std::string &o, const std::string &t)
    :film(f),sala(s),zi(z),ora(o),tip(t), pretBaza(25.0) {}

double Proiectie::getPret() const {
    return pretBaza;
}

std::vector<std::string> Proiectie::getBeneficii() const { return {};}

//PremiereSpeciala
PremieraSpeciala::PremieraSpeciala(const Film& f, const Sala& s, const std::string& z, const std::string& o, const std::string& t, const std::string& invitat)
    : Proiectie(f, s, z, o, t), invitat_special(invitat) {}

double PremieraSpeciala::getPret() const {
    return pretBaza+ 20.0;
}

std::string PremieraSpeciala::getNumeEveniment() const {
    return "PREMIERA SPECIALA (Invitat: " + invitat_special + ")";
}

std::vector<std::string> PremieraSpeciala::getBeneficii() const {
    return {"Welcome Drink", "Poster cadou", "Sesiune Q&A"};
}

//MaratonFilme
MaratonFilme::MaratonFilme(const Film& f, const Sala& s, const std::string& z, const std::string& o, const std::string& t, int nrF, const std::vector<std::string>& titluri)
    : Proiectie(f, s, z, o, t), numarFilme(nrF), titluriFilme(titluri) {}

double MaratonFilme::getPret() const {
    return pretBaza *2.2;
}

std::string MaratonFilme::getNumeEveniment() const {
    std::string info = "MARATON DE FILME :" + std::to_string(numarFilme) + " filme consecutive: " + film.getTitlu();
    for (const auto& t : titluriFilme) {
        std::string nume_nou = t;
        std::replace(nume_nou.begin(), nume_nou.end(), '_',' ');
        info += " + " + nume_nou;
    }

    return info;
}

std::vector<std::string> MaratonFilme::getBeneficii() const {
    return {"Refill gratuit popcorn", "Acces lounge intre filme"};
}

bool Proiectie::rezervare_multipla(const std::vector<int> &locuri) {
    return sala.rezervare_multipla(locuri);
}

void Proiectie::eliberare_loc(int nr) {
    sala.elibereaza_loc(nr);
}

bool Proiectie::are_locuri_disponibile() const {
    return !sala.sala_plina();
}

void Proiectie::afiseaza_harta_sala() const {
    sala.vizualizare_locuri();
}

void Proiectie::afiseaza_detalii_disponibilitate()const {
    std::cout << "Locuri libere: " << sala.locuri_libere()
              << " din " << sala.getCapacitate() << "\n";
}

const std::string & Proiectie::getZi() const {return zi;}

const std::string & Proiectie::getOra() const {return ora;}

const std::string & Proiectie::getTip() const {return tip;}

const Film & Proiectie::getFilm() const {return film;}

const Sala & Proiectie::getSala() const {return sala;}

Sala& Proiectie::getSala() {return sala;}

std::ostream & operator<<(std::ostream &os, const Proiectie &p) {
    os << "[" << p.getNumeEveniment() << "] "
       << "Proiectie: " << p.film << "\n"
       << "Sala: " << p.sala << "\n"
       << "Zi: " << p.zi << " - Ora: " << p.ora << " ( " << p.tip << " ) "<<"\n";
    return os;
}
