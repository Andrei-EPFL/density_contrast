#ifndef LOAD_CONF_H
#define LOAD_CONF_H

#include <iostream>
#include <fstream>
#include <cstddef>
#include <string>
#include <math.h>
#include "nlohmann/json.hpp"

struct configuration {
    float boxsize;
    int n_large_grid, n_shells, n_small_grid;
    size_t n_voids;
    std::size_t Ndens;
    float cell_side_size;
    float ri, shell_width, rf;
    float r_max;
    float RvMin, RvMax;
    std::string densfilename;
    std::string voidfilename; 
    std::string outfilename; 
};

configuration read_configuration();
void print_configuration(configuration conf);

#endif