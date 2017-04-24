//
// Created by Mark on 22.04.17.
//

#ifndef PURPLE_DISTRIBUTION_DIRICHLETJOB_H
#define PURPLE_DISTRIBUTION_DIRICHLETJOB_H

#include <purple/purple.h>
#include <ostream>

class DirichletJob: public Purple::Job {

public:
    DirichletJob(const int offset, const int height);

    virtual int get_weight() const override;

    friend std::ostream &operator<<(std::ostream &os, const DirichletJob &job);

private:
    const int offset, height;
    const int weight;

};


#endif //PURPLE_DISTRIBUTION_DIRICHLETJOB_H
