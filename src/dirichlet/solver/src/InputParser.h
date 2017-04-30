//
// Created by Mark on 24.04.17.
//

#ifndef PURPLE_DISTRIBUTION_INPUTPARSER_H
#define PURPLE_DISTRIBUTION_INPUTPARSER_H

#include "Input.h"

namespace Dirichlet {

    class InputParser {

    public:
        virtual Input parse_input() = 0;

    };

}

#endif //PURPLE_DISTRIBUTION_INPUTPARSER_H
