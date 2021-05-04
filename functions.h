
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <cstddef>
#include "load_conf.h"

template<class T>
T compute_shell_volume(T r, T dr) {
    return 4.0 * M_PI * (pow((r+dr), 3) - pow(r, 3))/3. ;
}

template<class T>
T distance(int *pos1, int *pos2) {
    return sqrt((pow(pos1[0] - pos2[0], 2) + pow(pos1[1] - pos2[1], 2) + pow(pos1[2] - pos2[2], 2)));
}

template<class T>
int shell(int *pc, int *p, configuration conf) {
    //return the index of the shell
    T d = distance<T>(pc, p) * conf.cell_side_size;
    T layer = 0;
    if (d < conf.ri) {
        return -1;
    }

    if(d < conf.rf + conf.shell_width) {
        layer = (d - conf.ri)/conf.shell_width;
        return layer;
    }

    if(d == conf.rf + conf.shell_width) {
        layer = (d - conf.ri)/conf.shell_width-1 ;
        return layer;
    }
    // return layer will give an int which will remove the values
    // after the comma (point). Basically, it will give the floor(layer).
    // Layer will always give the smallest integer approximation.
    return -1;
}

void index_mesh2ijk(std::size_t idx, int *ijk, int ngrid);

void ijk2index_mesh(int *ijk, std::size_t *idx, int ngrid);

#endif
