#ifndef READ_DATA_H
#define READ_DATA_H

#include <vector>
#include <fstream>
#include <iostream>
#include <string>

template<class T>
std::vector<std::vector<T>> loading_voids(std::string filename, configuration conf) {
    int rows = conf.n_voids;
    int cols = 4;
    int counter = 0;
    T x = 0., y = 0., z = 0., R = 0.;

    std::cout<<"Info: Initializing the voids vector...\n";
    std::vector<std::vector<T>> array_2d(rows, std::vector<T>(cols, 0));
    
    std::cout<<"Info: Open the file with voids...\n";
    std::ifstream infile (filename, std::ios::in);
    if (infile.is_open()) {
        std::cout<<"Status: The file with voids was opened...\nStatus: Read voids from file...\n";
        while (counter < rows) {
            infile>>x>>y>>z>>R;
            if (x >= 0 && x < conf.boxsize && y >= 0 && y < conf.boxsize && z >= 0 && z < conf.boxsize && R >= conf.RvMin && R < conf.RvMax) {
                array_2d[counter][0] = x;
                array_2d[counter][1] = y;
                array_2d[counter][2] = z;
                array_2d[counter][3] = R;
                counter ++;
            }
        }
        infile.close();
        std::cout<<"Status: Voids read succesfully...\n";
    }
    return array_2d;
}

template<class T>
int init_density(configuration conf, T **density) {
    *density = new T [conf.Ndens];
    if (*density == nullptr) {
        std::cout<<"ERROR: the density was not loaded\n";
        return -1;
    }
    return 0;
}

template<class T>
int read_density(std::string filename, configuration conf, T **dens) {
    char *bytes;
    bytes = new char [conf.Ndens * sizeof(T)];
    std::ifstream infile (filename, std::ifstream::binary | std::ios::in);
    if (infile.is_open()) {
        infile.read(bytes, conf.Ndens * sizeof(T));
        infile.close();
    }
    else {
        std::cout<<"ERROR: the binary file was not loaded\n";
	return -1;
    }
    *dens = reinterpret_cast<T*>(bytes);

    return 0;
}

#endif
