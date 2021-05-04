#include "functions.h"


void index_mesh2ijk(std::size_t idx, int *ijk, int ngrid) {
    ijk[0] = idx % ngrid;
    ijk[1] = ((idx - ijk[0]) / ngrid) % ngrid;
    ijk[2] = (idx - ijk[0] - ijk[1] * ngrid) / (ngrid * ngrid);
}

void ijk2index_mesh(int *ijk, std::size_t *idx, int ngrid) {
    *idx = ijk[2] * ngrid * ngrid + ijk[1] * ngrid + ijk[0];
}

