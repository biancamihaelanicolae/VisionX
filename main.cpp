#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>


#include "Cinema.h"
#include "MeniuConsola.h"
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
        Cinema& cinema = Cinema::getInstance();
        cinema.get_rating().incarcare_ratinguri("ratinguri.txt");
        cinema.incarca_din_fisier("program.txt");
        cinema.aplica_reguli_sarbatori();

        if (cinema.get_proiectie(0) == nullptr) {
            std::cout<< "Programul nu a putut fi incarcat!\n";
            return 1;
        }

        std::cout << "Bine ai venit pe site-ul VisionX\n";
        std::cout << cinema;

        MeniuConsola meniu(cinema);
        meniu.ruleaza();

        cinema.get_rating().salvare_ratinguri("ratinguri.txt");
        cinema.get_vanzari().afiseaza_statistici_vanzari();
    }
    catch (const Eroare_Fisier& e) {
        std::cerr << "\nEROARE: " << e.what() << " Programul se va inchide!\n";
        return 1;
    }
    catch (const ValidationError& e) {
        std::cerr << "\nEroare de validare: " << e.what() << "\n";
    }
    catch (const VisionX_Exception& e) {
        std::cerr << "\nEroare sistem: " << e.what() << "\n";
    }
    catch (const std::exception& e) {
        std::cerr <<"EROARE: " << e.what() << "\n";
        return 1;
    }

    std::cout << "\nMultumim ca ati folosit VisionX!";

    return 0;
}