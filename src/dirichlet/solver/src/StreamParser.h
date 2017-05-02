//
// Created by Mark on 24.04.17.
//

#ifndef PURPLE_DISTRIBUTION_STREAMPARSER_H
#define PURPLE_DISTRIBUTION_STREAMPARSER_H

#include "InputParser.h"
#include <istream>

namespace Dirichlet {

    class StreamParser : public InputParser {

    public:
        StreamParser(istream &&stream) : stream(stream) {}

        StreamParser(istream &stream) : stream(stream) {}

        virtual Input parse_input() override;

        double *parse_mesh(int width, int height);

    protected:
        istream &stream;

    };

}

#endif //PURPLE_DISTRIBUTION_STREAMPARSER_H
