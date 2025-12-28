#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <limits>
#include <sstream>

#include "Cinema.h"
#include "Film.h"
#include "Proiectie.h"
#include "Sala.h"
#include "Utilizator.h"
#include "Exceptii.h"

#ifdef _WIN32
#include <windows.h>
void enable_ansi_coloring() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
}
#endif

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

    proiectie_selectata.afiseaza_harta_sala();

    std::cout << "Proiectie aleasa: " << proiectie_selectata.getFilm().getTitlu() << "\n";
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

    cinema.vinde_bilet(u, proiectie_selectata, locuri_dorite, vrea_ochelari);

    cinema.actualizeaza_sala_originala(proiectie_selectata);
}

int main() {

    #ifdef _WIN32
        enable_ansi_coloring();
    #endif

    std::cout.precision(2);

    try {
        Cinema cinema;
        cinema.incarcare_ratinguri("../ratinguri.txt");
        cinema.incarca_din_fisier("program.txt");
        cinema.aplica_reguli_sarbatori();

        if (cinema.get_proiectie(0) == nullptr) {
            std::cout<< "Programul nu a putut fi incarcat!\n";
            return 1;
        }

        std::cout << "Bine ai venit pe site-ul VisionX\n";
        std::cout << cinema;

        while (true) {
            try {
                std::string comanda;
                std::cout << "\nPentru a cumpara un bilet foloseste comanda 'cumpara'!"
                             "\nDaca doresti sa aplici mai multe filtre de cautare foloseste 'filtre'!"
                             "\nDaca doresti sa renunti la un bilet deja cumparat foloseste comanda 'anulare'!"
                             "\nDaca vrei sa inchizi pagina foloseste comanda 'exit page'!"
                             "\nDaca vrei sa dai un rating pentru unul dintre filmele vizionate foloseste comanda 'rating'!";
                if (!(std::cin >> comanda)) break;

                std::transform(comanda.begin(), comanda.end(), comanda.begin(), [](unsigned char c){return std::tolower(c); });

                if (comanda == "exit" || comanda == "exit page") {
                    break;
                }

                if (comanda == "rating") {
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
                        cinema.adauga_rating(titlu, nota , comentariu);
                        cinema.salvare_ratinguri("../ratinguri.txt");
                    }catch (const VisionX_Exception& e) {
                        std::cout << e.what() << "\n";
                    }

                    continue;
                }

                if (comanda == "anulare") {
                    std::string nume_confirmare;

                    std::cout << "\n----ANULARE BILET----\n";
                    std::cout << "Introdu numele pe care a fost facuta operatiunea de cumparare a biletului: ";
                    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
                    std::getline(std::cin, nume_confirmare);

                    cinema.afiseaza_bilete_utilizator(nume_confirmare);

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
                            double bani = cinema.anuleaza_bilete(nume_confirmare, de_sters);
                            if (bani > 0)
                                std::cout << "S-au anulat cu success biletele selectate. Suma returnata: " << bani << " RON.\n";
                            else
                                std::cout << "Nu a fost anulat niciun bilet (verificati indecsii sau numele).\n";
                        } catch (const std::exception& e) {
                            std::cerr << "Eroare: " << e.what() << "\n";
                        }
                    }
                    continue;
                }

                if (comanda == "filtre") {
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

                    std::vector<Proiectie> rezultat = cinema.filtrare_smart(criteriu);

                    if (rezultat.empty()) {
                        std::cout << "\nNe pare rau, nu am gasit nicio proiectie care sa corespunda criteriilor alese.\n";
                    } else {
                        std::cout << "\n===== REZULTATE FILTRARE (" << rezultat.size() << ") =====\n";
                        for (size_t i = 0; i < rezultat.size(); ++i) {
                            std::cout << i << ". " << rezultat[i].getFilm().getTitlu()
                                      << " | " << rezultat[i].getZi()
                                      << " | " << rezultat[i].getOra()
                                      << " | " << rezultat[i].getTip()
                                      << " | " << rezultat[i].getFilm().getDurata() << " min\n";
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
                                proceseaza_cumparare(cinema, rezultat[idx_ales],u);
                            }
                        }
                    }
                    continue;
                }

                if (comanda == "cumpara") {
                    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

                    std::string raspuns_filtrare;
                    std::cout << "Vrei sa cauti filemele dupa un anumit gen (da/nu)?";
                    std::cin >> raspuns_filtrare;
                    std::transform(raspuns_filtrare.begin(), raspuns_filtrare.end(), raspuns_filtrare.begin(), [](unsigned char c){return std::tolower(c);});

                    std::vector<Proiectie> lista_proiectii_curente;

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
                                continue;
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
                                continue;
                            }

                            std::cout << "\n===== Proiectii Genul " << gen_ales << "=====\n";
                        }
                    }else {
                        lista_proiectii_curente = cinema.get_program_sortat();

                        if (lista_proiectii_curente.empty()) {
                            std::cout << "\n Nu exista proiectii programate. Vanzare anulata!\n";
                            continue;
                        }

                        std::cout << "\n==== Programul cinematografului VisionX ====\n";
                    }
                    std::string zi_curenta;

                    for (size_t i=0; i < lista_proiectii_curente.size(); ++i) {
                        if (lista_proiectii_curente[i].getZi() != zi_curenta) {
                            zi_curenta = lista_proiectii_curente[i].getZi();
                            std::cout << "\nZiua: " << zi_curenta << "\n";
                        }

                        std::cout << i << ". " << lista_proiectii_curente[i].getFilm().getTitlu()
                                  << " | Sala " << lista_proiectii_curente[i].getSala().getNumar()
                                  << " | Ora: " << lista_proiectii_curente[i].getOra()
                                  << " (" << lista_proiectii_curente[i].getTip() << ")\n";
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
                        proiectie_selectata = &lista_proiectii_curente[idx];
                    }
                    if (!proiectie_selectata) {
                        throw Eroare_selectie_invalida();
                    }

                    std::cout << "\nRecenzii pentru acest film: ";
                    cinema.afiseaza_rating_film(proiectie_selectata->getFilm().getTitlu());

                    proceseaza_cumparare(cinema, *proiectie_selectata,u);
                }else{
                    std::cout << "Comanda necunoscuta.\n";
                }

            }catch (const VisionX_Exception& e) {
                std::cerr << "\nEROARE: " << e.what() << " Incercati din nou!\n";
            }
        }

        cinema.salvare_ratinguri("../ratinguri.txt");
        cinema.salvare_bilete_utilizator("../bilete_utilizator.txt");
        cinema.afiseaza_statistici_vanzari();
    }
    catch (const Eroare_Fisier& e) {
        std::cerr << "\nEROARE: " << e.what() << " Programul se va inchide!\n";
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr <<"EROARE: " << e.what() << "\n";
        return 1;
    }

    std::cout << "\nMultumim ca ati folosit VisionX!";

    return 0;
}