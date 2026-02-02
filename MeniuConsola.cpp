//
// Created by Bianca Mihaela on 04/01/2026.
//

#include "MeniuConsola.h"
#include "Cinema.h"
#include "Film.h"
#include "Proiectie.h"
#include "Sala.h"
#include "Utilizator.h"
#include "Exceptii.h"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <limits>
#include <sstream>

MeniuConsola::MeniuConsola(Cinema& c) : cinema(c) {}

void proceseaza_cumparare(Cinema& cinema, Proiectie& proiectie_selectata,const Utilizator& u) {

    bool vrea_ochelari = false;
    if (proiectie_selectata.getTip() == "3D") {
        std::string raspuns_ochelari;
        std::cout << "Filmul este in format 3D. Doriti sa inchiriati ochelari 3D pentru proiectie (cost: 5 RON)? (da/nu): ";
        std::cin >> raspuns_ochelari;

        std::transform(raspuns_ochelari.begin(), raspuns_ochelari.end(), raspuns_ochelari.begin(), [](unsigned char c){ return std::tolower(c); });

        if (raspuns_ochelari == "da") {
            vrea_ochelari = true;
        }
    }

    if (proiectie_selectata.getZi() == "Miercuri" && proiectie_selectata.getTip() != "IMAX") {
        std::cout << ANSI_GREEN
        << "\n [Promo] Pentru proiectiile din ziua de MIERCURI beneficiati de o reducere generala la toate filmele 2D si 3D: \n"
        << "Biletul va costa doar 15 RON!"
        << ANSI_RESET <<"\n";
    }

    if (!proiectie_selectata.are_locuri_disponibile()) {
        std::cout << "Sala este plina! Alege alta proiectie!\n";
        return;
    }

    std::cout << "\n==================================================";
    std::cout << "\nRECENZII PENTRU: " << proiectie_selectata.getFilm().getTitlu();
    cinema.get_rating().afiseaza_rating_film(proiectie_selectata.getFilm().getTitlu());
    std::cout << "\n==================================================";

    proiectie_selectata.afiseaza_harta_sala();

    std::cout << "Proiectie aleasa: " << proiectie_selectata.getFilm().getTitlu() << "\n";

    bool succes = false;
    do {
        proiectie_selectata.afiseaza_detalii_disponibilitate();
        std::cout << "Alege locurile dorite: ";

        std::vector<int> locuri_dorite;
        std::string line;

        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

        std::getline(std::cin, line);
        std::stringstream ss(line);
        int loc_curent;

        while (ss >> loc_curent) {
            locuri_dorite.push_back(loc_curent);
        }

        if (locuri_dorite.empty()) {
            std::cout << "Nu s-au specificat locuri. Vanzare anulata!\n";
            return;
        }

        auto& mp = cinema.get_manager_produse();
        mp.reseteazaCos();

        std::string vrea_gustare;
        std::cout << "Doriti sa adaugati snaks sau bauturi la aceasta comanda? (da/nu): ";
        std::cin >> vrea_gustare;

        if (vrea_gustare == "da" || vrea_gustare == "DA") {
            mp.afiseazaMeniu();
            int idx_snack;
            while (true) {
                std::cout << "Introduceti indexul produsului dorit (-1 pentru a termina selectia): ";
                if (!(std::cin >> idx_snack) || idx_snack == -1) break;
                mp.adaugaInCos(idx_snack);
            }
        }

        std::vector<std::string> snacks_ales = mp.getCos();
        double pret_snacks = mp.getTotal();

        double factor_reducere = 1.0;
        std::string username = u.getUsername();
        auto& mf = cinema.get_fidelitate();

        if (mf.are_card(username)) {
            int puncte_actuale = mf.get_puncte(username);
            std::cout << ANSI_GREEN << "\n[Fidelitate] Aveti un card activ cu " << puncte_actuale << " puncte." << ANSI_RESET << "\n";

            if (puncte_actuale >= 100) {
                std::string optiune;
                std::cout << "Doriti sa folositi 100 de puncte pentru o reducere de 90% la un bilet? (da/nu): ";
                std::cin >> optiune;
                if (optiune == "da") {
                    factor_reducere = 0.1;
                    mf.consuma_puncte(username, 100);
                    std::cout << "[Fidelitate] S-au consumat 100 puncte. Reducere aplicata!\n";
                } else {
                    mf.adauga_puncte(username, 10);
                    std::cout << "[Fidelitate] S-au adaugat 10 puncte pe card.\n";
                }
            } else {
                mf.adauga_puncte(username, 10);
                std::cout << "[Fidelitate] S-au adaugat 10 puncte pentru achizitia curenta.\n";
            }
        } else {
            std::string raspuns;
            std::cout << "Nu aveti card de fidelitate VisionX. Doriti sa va creati unul acum? (da/nu): ";
            std::cin >> raspuns;
            if (raspuns == "da") {
                mf.creeaza_card(username);
                mf.adauga_puncte(username, 10);
                std::cout << "[Fidelitate] Card creat! S-au adaugat primele 10 puncte.\n";
            }
        }

        mf.salveaza_puncte();

        try {
            cinema.vinde_bilet(u, proiectie_selectata, locuri_dorite, vrea_ochelari, factor_reducere, snacks_ales, pret_snacks);
            cinema.get_vanzari().salvare_bilete_utilizator("bilete_utilizator.txt");
            cinema.get_vanzari().afiseaza_bilete_utilizator(u.getUsername());

            if (mf.are_card(username)) {
                std::cout << ANSI_GREEN << "Puncte fidelitate dupa tranzactie: " << mf.get_puncte(username) << ANSI_RESET << "\n";
            }
            succes = true;
        }catch (const Eroare_loc_ocupat& e) {
            std::cerr << "\n" << e.what() << "Te rog sa alegi alte locuri libere!\n";
        }
    }while (!succes);
}

void MeniuConsola::ruleaza() {
    while (true) {
        std::cout << "\nPentru a cumpara un bilet foloseste comanda 'cumpara'!"
                             "\nDaca doresti sa aplici mai multe filtre de cautare foloseste 'filtre'!"
                             "\nDaca doresti sa renunti la un bilet deja cumparat foloseste comanda 'anulare'!"
                             "\nDaca vrei sa inchizi pagina foloseste comanda 'exit page'!"
                             "\nDaca vrei sa dai un rating pentru unul dintre filmele vizionate foloseste comanda 'rating'!"
                             "\nDaca doresti sa cauti un film dupa denumire foloseste comanda 'cauta'!";
        std::string comanda = citesteComanda();

        if (comanda == "exit" || comanda == "exit page") break;
        else if (comanda == "cumpara") meniuCumparare();
        else if (comanda == "filtre") meniuFiltre();
        else if (comanda == "anulare") meniuAnulare();
        else if (comanda == "rating") meniuRating();
        else if (comanda == "cauta") meniuCautare();
        else std::cout << "Comanda necunoscuta!\n";
    }
}

void MeniuConsola::meniuCautare() {
    std::string query;
    std::cout << "Introdu numele filmului (sau o parte din el): ";
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    std::getline(std::cin, query);

    std::vector<Proiectie*> rezultate = cinema.cauta_film(query);

    if (rezultate.empty()) {
        std::cout << "Ne pare rau, nu am gasit niciun film care sa contina: \"" << query << "\"\n";
    }else {
        std::cout << "\n---REZULTATE CAUTARE (" << rezultate.size() << ")---\n";
        for (size_t i = 0; i < rezultate.size(); i++) {
            double medie = cinema.get_rating().calculeaza_medie_film(rezultate[i]->getFilm().getTitlu());
            std::cout << i << ". " << rezultate[i]->getFilm().getTitlu() << " | " << rezultate[i]->getZi() << ", " << rezultate[i]->getOra();

            if (medie > 0) {
                std::cout << "[Rating : " << std::fixed << std::setprecision(1) << medie << "]";
                std::cout << "\n";
            }else std::cout << "[Fara rating]\n";

            std::cout << " [" << rezultate[i]->getNumeEveniment() << "]\n";
        }

        std::string raspuns;
        std::cout << "\nDoriti sa cumparati bilet la unul dintre aceste filme? (da/nu): ";
        std::cin >> raspuns;

        if (raspuns == "da") {
            int idx;
            std::cout << "Introduceti indexul filmului: ";

            if (!(std::cin >> idx) || idx < 0 || (size_t)idx >= rezultate.size()) {
                std::cout << "Index invalid!\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return;
            }

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            Utilizator u;
            u.citire_utilizator();

            std::cout << "\nRecenzii text pentru acest film: ";
            cinema.get_rating().afiseaza_rating_film(rezultate[idx]->getFilm().getTitlu());

            proceseaza_cumparare(cinema, *rezultate[idx], u);
        }
    }
}

void MeniuConsola::meniuRating() {
    std::string titlu,comentariu;
    int nota;

    std::cout << "Introduceti numele filmului pentru recenzie: ";
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    std::getline(std::cin, titlu);

    std::cout << "Nota (1-10): ";
    std::cin >> nota;
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

    std::cout << "Comentariu: ";
    std::getline(std::cin, comentariu);

    try {
        cinema.get_rating().adauga_rating(titlu, nota , comentariu);
        cinema.get_rating().salvare_ratinguri("ratinguri.txt");
    }catch (const ValidationError& e) {
        std::cerr << "\nDate invalide: " << e.what() << "Te rog sa introduci o nota valida (1-10)!\n";
    }catch (const VisionX_Exception& e) {
        std::cerr << "\nEroare: " << e.what() << "\n";
    }
}

void MeniuConsola::meniuCumparare() {
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

    std::string raspuns_filtrare;
    std::cout << "Vrei sa cauti filemele dupa un anumit gen (da/nu)?";
    std::cin >> raspuns_filtrare;
    std::transform(raspuns_filtrare.begin(), raspuns_filtrare.end(), raspuns_filtrare.begin(), [](unsigned char c){return std::tolower(c);});

    std::vector<Proiectie*> lista_proiectii_curente;

    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

    if (raspuns_filtrare == "da") {
        std::string raspuns_copii;
        std::cout << "Doriti sa vedeti doar selectia de filme animate pentru copii (da/nu)?";
        std::cin >> raspuns_copii;
        std::transform(raspuns_copii.begin(), raspuns_copii.end(), raspuns_copii.begin(), [](unsigned char c){return std::tolower(c);});

        if (raspuns_copii == "da") {
            lista_proiectii_curente = cinema.filtreaza_pentru_copii();

            if (lista_proiectii_curente.empty()) {
                std::cout << "Momentan nu avem animatii in program.";
                return;
            }

            std::cout << "\n===== SELECTIE FILME ANIMATE =====\n";
        }
        else {
            cinema.afiseaza_meniu_genuri();
            std::string gen_ales;
            std::cout << "Introdu genul dorit, (din lista de mai sus): ";
            std::cin >> gen_ales;

            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

            lista_proiectii_curente = cinema.filtreaza_pe_gen(gen_ales);

            if (lista_proiectii_curente.empty()) {
                std::cout << "\nNu exista proiectii pentru genul " << gen_ales << " Vanzare anulata!\n";
                return;
            }

            std::cout << "\n===== Proiectii Genul " << gen_ales << "=====\n";
        }
    }else {
        lista_proiectii_curente = cinema.get_program_sortat();

        if (lista_proiectii_curente.empty()) {
            std::cout << "\n Nu exista proiectii programate. Vanzare anulata!\n";
            return;
        }

        std::cout << "\n==== Programul cinematografului VisionX ====\n";
    }
    std::string zi_curenta;

    for (size_t i=0; i < lista_proiectii_curente.size(); ++i) {
        if (lista_proiectii_curente[i]->getZi() != zi_curenta) {
            zi_curenta = lista_proiectii_curente[i]->getZi();
            std::cout << "\nZiua: " << zi_curenta << "\n";
        }

        double medie = cinema.get_rating().calculeaza_medie_film(lista_proiectii_curente[i]->getFilm().getTitlu());
        std::cout << i << ". " << lista_proiectii_curente[i]->getFilm().getTitlu()
        << " | Sala " << lista_proiectii_curente[i]->getSala().getNumar()
        << " | Ora: " << lista_proiectii_curente[i]->getOra()
        << " (" << lista_proiectii_curente[i]->getTip() << ")\n";

        std::cout << " [" << lista_proiectii_curente[i]->getNumeEveniment() << "]\n";

        if (medie > 0) {
            std::cout << "[Rating: " << std::fixed <<std::setprecision(1) << medie << "/10]\n";
        }
        else {
            std::cout << "[Fara rating]\n";
        }
    }

    std::cout << "============================================\n";

    Utilizator u;
    u.citire_utilizator();

    int idx;
    std::cout << "Introdu indexul proiectiei dorite: ";
    if (!(std::cin >> idx)) {
        std::cin.clear();
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
        throw Eroare_selectie_invalida();
    }

    Proiectie* proiectie_selectata = nullptr;

    if (idx >= 0 && static_cast<size_t>(idx)  < lista_proiectii_curente.size()) {
        proiectie_selectata = lista_proiectii_curente[idx];
    }
    if (!proiectie_selectata) {
        throw Eroare_selectie_invalida();
    }

    std::cout << "\nRecenzii pentru acest film: ";
    cinema.get_rating().afiseaza_rating_film(proiectie_selectata->getFilm().getTitlu());

    proceseaza_cumparare(cinema, *proiectie_selectata,u);
}

void MeniuConsola::meniuAnulare() {
    std::string nume_confirmare;

    std::cout << "\n----ANULARE BILET----\n";
    std::cout << "Introdu numele pe care a fost facuta operatiunea de cumparare a biletului: ";
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    std::getline(std::cin, nume_confirmare);

    cinema.get_vanzari().afiseaza_bilete_utilizator(nume_confirmare);

    std::cout << "\nIntroduceti indecsii biletelor de anulat (separati prin spatiu): ";
    std::string linie_indecsi;
    std::getline(std::cin, linie_indecsi);

    std::stringstream ss(linie_indecsi);
    std::vector<int> de_sters;
    int index_citit;
    while (ss >> index_citit) {
        de_sters.push_back(index_citit - 1);
    }

    if (!de_sters.empty()) {
        try {
            double bani = cinema.get_vanzari().anuleaza_bilete(nume_confirmare, de_sters, cinema.get_toate_proiectiile().getReferinta());
            if (bani > 0) {
                std::cout << "S-au anulat cu success biletele selectate. Suma returnata: " << bani << " RON.\n";
                cinema.get_vanzari().salvare_bilete_utilizator("bilete_utilizator.txt");
            }
            else
                std::cout << "Nu a fost anulat niciun bilet (verificati indecsii sau numele).\n";
        } catch (const std::exception& e) {
            std::cerr << "Eroare: " << e.what() << "\n";
        }
    }
}

void MeniuConsola::meniuFiltre() {
    CriteriiCautare criteriu;
    std::string optiune;

    std::cout << "\n---- CONFIGURARE FILTRE ----\n";
    std::cout << "(Introdu 'orice' pentru a sari peste un criteriu)\n";

    std::cout << "Ziua saptamanii: ";
    std::cin >> optiune;
    if (optiune != "orice") criteriu.zi = optiune;

    std::cout << "Tip proiectie (2D/3D/IMAX): ";
    std::cin >> optiune;
    if (optiune != "orice") criteriu.tip_proiectie = optiune;

    std::cout << "Durata maxima (minute) sau 0 pentru orice: ";
    if (!(std::cin >> criteriu.durata_maxima)) {
        std::cin.clear();
        criteriu.durata_maxima = 0;
    }

    std::vector<Proiectie*> rezultat = cinema.filtrare_smart(criteriu);

    std::vector<Proiectie*> sugestii = cinema.genereaza_sugestii(criteriu);
    if (!sugestii.empty()) {
        std::cout << "\n[TOP RECOMANDARI PENTRU TINE]: \n";
        for (size_t i = 0; i < sugestii.size(); i++) {
            double medie = cinema.get_rating().calculeaza_medie_film(sugestii[i]->getFilm().getTitlu());
            std::cout << i + 1 << ". " << sugestii[i]->getFilm().getTitlu()
            << " - Rating: " << std::fixed << std::setprecision(1) << medie << "/10\n";
        }

        std::cout << "-------------------------------\n";
    }

    if (rezultat.empty()) {
        std::cout << "\nNe pare rau, nu am gasit nicio proiectie care sa corespunda criteriilor alese.\n";
    } else {
        std::cout << "\n===== REZULTATE FILTRARE (" << rezultat.size() << ") =====\n";
        for (size_t i = 0; i < rezultat.size(); ++i) {
            double medie = cinema.get_rating().calculeaza_medie_film(rezultat[i]->getFilm().getTitlu());

            std::cout << i << ". " << rezultat[i]->getFilm().getTitlu()
            << " | " << rezultat[i]->getZi()
            << " | " << rezultat[i]->getOra()
            << " | " << rezultat[i]->getTip()
            << " | " << rezultat[i]->getFilm().getDurata() << " min\n";

            if (medie > 0) {
                std::cout << "[Rating: " << std::fixed <<std::setprecision(1) << medie << "/10]\n";
            }
            else {
                std::cout << "[Fara rating]\n";
            }
        }
        std::cout << "==========================================\n";

        std::string vrea_sa_cumpere;
        std::cout << "Doriti sa cumparati bilet la unul dintre filmele listate mai sus? (da/nu): ";
        std::cin >> vrea_sa_cumpere;

        if (vrea_sa_cumpere == "da") {
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

            Utilizator u;
            u.citire_utilizator();
            int idx_ales;
            std::cout << "Introdu indexul filmului ales din lista de mai sus: ";
            if (!(std::cin >> idx_ales) || idx_ales < 0 || idx_ales >= (int)rezultat.size()) {
                std::cout << "Index invalid! Revenire la meniul principal.\n";
                std::cin.clear();
                std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            }else {
                proceseaza_cumparare(cinema, *rezultat[idx_ales],u);
            }
        }
    }
}

std::string MeniuConsola::citesteComanda() {
    std::string comanda;
    if (!(std::cin >> comanda)) return "exit";

    std::transform(comanda.begin(), comanda.end(), comanda.begin(), [](unsigned char c){return std::tolower(c); });
    return comanda;
}