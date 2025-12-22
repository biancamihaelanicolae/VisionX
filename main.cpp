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

int main() {

    #ifdef _WIN32
        enable_ansi_coloring();
    #endif

    std::cout.precision(2);

    try {
        Cinema cinema;
        cinema.incarca_din_fisier("program.txt");

        if (cinema.get_proiectie(0) == nullptr) {
            std::cout<< "Programul nu a putut fi incarcat!\n";
            return 1;
        }

        std::cout << "Bine ai venit pe site-ul VisionX\n";
        std::cout << cinema;

        while (true) {
            try {
                std::string comanda;
                std::cout << "\nPentru a cumpara un bilet foloseste comanda 'cumpara', iar daca vrei sa inchizi pagina foloseste comanda 'exit page'!";
                if (!(std::cin >> comanda)) break;

                std::transform(comanda.begin(), comanda.end(), comanda.begin(), [](unsigned char c){return std::tolower(c); });

                if (comanda == "exit" || comanda == "exit page") {
                    break;
                }

                if (comanda != "cumpara") {
                    std::cout << "Comanda necunoscuta.\n";
                    continue;
                }

                std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

                std::vector<std::string> genuri = cinema.get_genuri_disponibile();
                std::string lista_genuri;
                if (!genuri.empty()) {
                    for (size_t i = 0; i < genuri.size(); i++) {
                        lista_genuri += genuri[i];
                        if (i < genuri.size() - 1) {
                            lista_genuri += ", ";
                        }
                    }
                }else {
                    lista_genuri = "N/A\n";
                }

                std::string raspuns_filtrare;
                std::cout << "Vrei sa cauti filemele dupa un anumit gen (da/nu)?";
                std::cin >> raspuns_filtrare;
                std::transform(raspuns_filtrare.begin(), raspuns_filtrare.end(), raspuns_filtrare.begin(), [](unsigned char c){return std::tolower(c);});

                std::vector<Proiectie> lista_proiectii_curente;

                std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

                if (raspuns_filtrare == "da") {
                    std::string gen_ales;
                    std::cout << "Introdu genul dorit, (Genuri disponibile: " << lista_genuri << "): \n";
                    std::cin >> gen_ales;

                    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

                    lista_proiectii_curente = cinema.filtreaza_pe_gen(gen_ales);

                    if (lista_proiectii_curente.empty()) {
                        std::cout << "\nNu exista proiectii pentru genul " << gen_ales << " Vanzare anulata!\n";
                        continue;
                    }

                    std::cout << "\n===== Proiectii Genul " << gen_ales << "=====\n";
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

                }else{
                    lista_proiectii_curente = cinema.get_program_sortat();

                    if (lista_proiectii_curente.empty()) {
                        std::cout << "\n Nu exista proiectii programate. Vanzare anulata!\n";
                        continue;
                    }

                    std::cout << "\n==== Programul cinematografului VisionX ====\n";
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
                }

                if (lista_proiectii_curente.empty()) {
                    continue;
                }

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

                if (proiectie_selectata->getSala().sala_plina()) {
                    std::cout << "Sala este plina! Alege alta proiectie!\n";
                    continue;
                }

                proiectie_selectata->getSala().vizualizare_locuri();

                std::cout << "Proiectie aleasa: " << proiectie_selectata->getFilm().getTitlu() << "\n";
                std::cout << "Locuril libere: " << proiectie_selectata->getSala().locuri_libere() << " din " << proiectie_selectata->getSala().getCapacitate() << "\n";
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
                    continue;
                }

                cinema.vinde_bilet(u, *proiectie_selectata, locuri_dorite);

                if (!lista_proiectii_curente.empty()) {
                    cinema.actualizeaza_sala_originala(*proiectie_selectata);
                }
            }

            catch (const VisionX_Exception& e) {
                std::cerr << "\nEROARE: " << e.what() << " Incercati din nou!\n";
            }
        }

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