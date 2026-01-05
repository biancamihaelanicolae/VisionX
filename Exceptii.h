//
// Created by Bianca Mihaela on 17/12/2025.
//

#ifndef OOP_EXCEPTII_H
#define OOP_EXCEPTII_H

#include <exception>
#include <string>
#include <utility>

class VisionX_Exception: public std::exception {
    protected:
        std::string mesaj;
    public:
        explicit VisionX_Exception(std::string m) : mesaj(std::move(m)){}
        [[nodiscard]] const char* what() const noexcept override {return mesaj.c_str(); }
};

class Eroare_Fisier : public VisionX_Exception {
    public:
        explicit Eroare_Fisier (const std::string& nume)
            : VisionX_Exception("EROARE: Nu s-a putut deschide fisierul: " + nume){}
};

class Eroare_loc_ocupat : public VisionX_Exception {
    public:
        explicit Eroare_loc_ocupat(int loc)
            : VisionX_Exception("EROARE: Locul: " + std::to_string(loc) + " este deja rezervat!"){}
};

class Eroare_selectie_invalida : public VisionX_Exception {
    public:
        Eroare_selectie_invalida()
            : VisionX_Exception ("EROARE: Selectia introdusa nu este valida!"){}
};

class RatingException : public VisionX_Exception {
    public:
    using VisionX_Exception::VisionX_Exception;
};

class ValidationError : public RatingException {
public:
    ValidationError(const std::string& msg) : RatingException("Eroare Validare: " + msg) {}
};

#endif //OOP_EXCEPTII_H