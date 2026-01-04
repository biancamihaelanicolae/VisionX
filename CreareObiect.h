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
#include <string>
#include <algorithm>

class CreareObiect {
    public:
    static BazaBilet* creeazaBilet(std::string tip) {
        std::transform(tip.begin(), tip.end(), tip.begin(), ::tolower);

        if (tip == "student") {
            return new BiletStudent();
        }
        if (tip == "elev") {
            return new BiletElev();
        }
        if (tip == "pensionar") {
            return new BiletPensionar();
        }

        return new BiletNormal();
    }

    static bool esteTipValid(const std::string& tip) {
        static const std::vector<std::string> tiputi_validate = {
            "normal", "student", "elev", "pensionar"
        };

        return std::find(tiputi_validate.begin(), tiputi_validate.end(), tip) != tiputi_validate.end();
    }
};

#endif //OOP_CREAREOBIECT_H