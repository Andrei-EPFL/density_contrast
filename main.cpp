#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <string>
#include <cstddef>
#include <chrono>

#include "load_conf.h"
#include "read_data.h"
#include "functions.h"

#ifdef OMP
#include <omp.h>
#endif

# define M_PI           3.14159265358979323846  /* pi */


int main() {
    std::cout<<"\n1. Status: Reading configuration...\n";
    configuration conf;
    conf = read_configuration();
    std::cout<<"Status: Configuration successfully read...\n";
    print_configuration(conf);

    return 0;
    std::cout<<"2. Status: Reading the density field...\n";
    float *density;
    //init_density(conf, &density);
    std::chrono::steady_clock sc;
    auto start = sc.now();
    read_density<float>(conf.densfilename, conf, &density);
    auto end = sc.now();
    auto time_span = static_cast<std::chrono::duration<double>>(end - start);
    std::cout<<"INFO: It took "<<time_span.count()<<" seconds to read the density!\n";


    std::cout<<"\n3. Status: Loading the voids...\n";
    start = sc.now();
    std::vector<std::vector<float>> voids = loading_voids<float>(conf.voidfilename, conf);
    end = sc.now();
    time_span = static_cast<std::chrono::duration<double>>(end - start);
    std::cout<<"INFO: It took "<<time_span.count()<<" seconds to load the voids!\n";


    std::cout<<"\n4. Status: Computing the volumes/radii of the shells...\n";
    std::vector<float> radius(conf.n_shells, 0), volume(conf.n_shells, 0), density_profile_voids(conf.n_shells, 0), density2_profile_voids(conf.n_shells, 0);
    float a = conf.shell_width;
    std::generate(radius.begin(), radius.end(), [n = 0, &a]() mutable { return n++ * a; });

    for(unsigned int i = 0; i < volume.size(); i++) {
        volume[i] = compute_shell_volume(radius[i], conf.shell_width) / pow(conf.cell_side_size, 3);
    }
    std::cout<<"Status: The volumes of the shells succesfully computed...\n";

    
    std::cout<<"\n5. Status: Computing the density contrast of the selected voids...\n";
    std::size_t i = 0;

    start = sc.now();
#ifdef OMP
#pragma omp parallel shared(density_profile_voids, density2_profile_voids) private(i)
    {
#endif
        int *small_mesh_ijk = new int [3]; 
        int *large_mesh_ijk = new int [3];
        int *voids_ijk = new int [3];
        std::vector<float> mass(conf.n_shells, 0);
        std::size_t idx;
        int layer;
        float r_max;
        std::vector<float> l_density_profile_voids(conf.n_shells, 0), l_density2_profile_voids(conf.n_shells, 0);

#ifdef OMP
#pragma omp for 
#endif
        for(i = 0; i < voids.size(); i++) {
            std::fill(mass.begin(), mass.end(), 0);
            voids_ijk[0] = round(voids[i][0] / conf.cell_side_size);
            voids_ijk[1] = round(voids[i][1] / conf.cell_side_size);
            voids_ijk[2] = round(voids[i][2] / conf.cell_side_size);
            r_max = voids[i][3] * conf.reduced_r_max;
            conf.n_small_grid = 2 * trunc(conf.r_max / conf.cell_side_size) + 3;

            for(std::size_t j = 0; j < pow(conf.n_small_grid, 3); j++) { 
                
                index_mesh2ijk(j, small_mesh_ijk, conf.n_small_grid);

                for(unsigned int ijk = 0; ijk < 3; ijk ++) {
                    large_mesh_ijk[ijk] = voids_ijk[ijk] + small_mesh_ijk[ijk] - trunc(conf.n_small_grid / 2);
                    large_mesh_ijk[ijk] = (large_mesh_ijk[ijk] + conf.n_large_grid) % conf.n_large_grid; 
                }
                                      
                ijk2index_mesh(large_mesh_ijk, &idx, conf.n_large_grid);
                layer = shell<float>(voids_ijk, large_mesh_ijk, conf);
                
                if(layer >= 0) {
                    mass[layer] = mass[layer] + density[idx];
                }
            }

            for(unsigned int k = 0; k < density_profile_voids.size(); k++) {
                l_density_profile_voids[k] += mass[k];
                l_density2_profile_voids[k] += pow(mass[k], 2);
            }
        }
#ifdef OMP
#pragma omp critical
#endif
        {
            for(unsigned int k = 0; k < density_profile_voids.size(); k++) {
                density_profile_voids[k] += l_density_profile_voids[k];
                density2_profile_voids[k] += l_density2_profile_voids[k];
            }
        }
        delete small_mesh_ijk;
        delete large_mesh_ijk;
        delete voids_ijk;
#ifdef OMP
    }
#endif 
    end = sc.now();
    time_span = static_cast<std::chrono::duration<double>>(end - start);
    std::cout<<"INFO: It took "<<time_span.count()<<" seconds to compute the density contrast of the selected voids...\n";
    

    std::cout<<"\n6. Status: Writing the density contrast of the selected voids...\n";
    std::ofstream output_file;
    output_file.open(conf.outfilename);
    output_file << "The number of voids is: " << voids.size() << ".\n";
    output_file<< "r" << "   " << "delta(r)"  << "    " <<  "variance(delta(r))"  << "\n";
    
    for(unsigned int k = 0; k < density_profile_voids.size(); k++) {
        output_file<< radius[k] << " " << density_profile_voids[k]  / volume[k] / voids.size()  << " " << (density2_profile_voids[k] / voids.size() - pow(density_profile_voids[k] / voids.size(), 2)) / pow(volume[k], 2)  << "\n";
    }
    output_file.close();
    
    delete density;
    
    std::cout<<"THE END! That's all folks!\n";

    return 0;
}


           