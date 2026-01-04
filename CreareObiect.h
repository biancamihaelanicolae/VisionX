//
// Created by Bianca Mihaela on 04/01/2026.
//

#ifndef OOP_CREAREOBIECT_H
#define OOP_CREAREOBIECT_H

#include "BazaBilet.h"
#include "BiletNormal.h"
#include "BiletStudent.h"
#include "BiletElev.h"
#include "BiletPensionar.h"

#include <vector>
#include <string>
#include <algorithm>

class CreareObiect {
private:
    static inline std::map<std::string, int> contor_bilete;
    public:
    static BazaBilet* creeazaBilet(std::string tip) {
        BazaBilet* b = nullptr;
        std::transform(tip.begin(), tip.end(), tip.begin(), ::tolower);

        if (tip == "student") {
            b = new BiletStudent();
        }
        else if (tip == "elev") {
            b = new BiletElev();
        }
        else if (tip == "pensionar") {
            b = new BiletPensionar();
        }
        else b = new BiletNormal();

        if (b) {
            contor_bilete[b->get_nume_tip()]++;
        }

        return b;
    }

    static const std::map<std::string, int>& get_statistici() {
        return contor_bilete;
    }

    static bool esteTipValid(const std::string& tip) {
        static const std::vector<std::string> tipuri_validate = {
            "normal", "student", "elev", "pensionar"
        };

        return std::find(tipuri_validate.begin(), tipuri_validate.end(), tip) != tipuri_validate.end();
    }
};

#endif //OOP_CREAREOBIECT_H