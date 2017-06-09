//
// Created by Mark on 21.05.17.
//

#ifndef PURPLE_DISTRIBUTION_WAIT_H
#define PURPLE_DISTRIBUTION_WAIT_H

#include <purple/Job.h>

class WaitJob : public Purple::Job {

public:
    WaitJob(int seconds = 0):seconds(seconds) {}

    virtual int get_weight() const override {
        return seconds;
    }

    int get_seconds() const { return seconds; }

private:
    PURPLE_SERIALIZE() {
        Purple::Job::serialize(ar, version);
        ar & seconds;
    }

    int seconds;

};

class WaitProcessor : public Purple::Processor<WaitJob, void> {

public:
    void process(WaitJob &job) override {
        sleep(job.get_seconds());
    }

};

#endif //PURPLE_DISTRIBUTION_WAIT_H
