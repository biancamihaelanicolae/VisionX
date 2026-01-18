//
// Created by Bianca Mihaela on 17/01/2026.
//

#ifndef OOP_MANAGERFIDELITATE_H
#define OOP_MANAGERFIDELITATE_H

#include <string>
#include <map>

class ManagerFidelitate {
private:
    std::map<std::string, int> baza_puncte;
    const std::string fisier_puncte = "fidelitate.txt";

public:
    ManagerFidelitate();
    ~ManagerFidelitate();

    void incarca_puncte();
    void salveaza_puncte() const;

    int get_puncte(const std::string& username) const;
    bool are_card(const std::string& username) const;

    void creeaza_card(const std::string& username);
    void adauga_puncte(const std::string& username, int puncte);
    void consuma_puncte(const std::string& username, int puncte);
};


#endif //OOP_MANAGERFIDELITATE_H