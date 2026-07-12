#ifndef RANDOM_GEM_GENERATOR_H
#define RANDOM_GEM_GENERATOR_H

#include "Common.h"

class RandomGemGenerator
{
private:
    char symbol[10] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    mt19937 gen{random_device{}()};

public:
    char generate()
    {
        uniform_int_distribution<> dist(0, 7);

        int rand_idx = dist(gen);
        return symbol[rand_idx];
    }
};

#endif
