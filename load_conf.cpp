#include "load_conf.h"

using json = nlohmann::json;

configuration read_configuration() {
    configuration conf;

    std::ifstream config_in("config.json");
    json j;
    config_in >> j;

    // Set by user
    conf.n_voids = j["n_voids"];

    conf.n_large_grid = j["n_large_grid"];
    conf.boxsize = j["boxsize"];
    
    conf.ri = j["ri"];
    conf.shell_width = j["shell_width"];
    conf.r_max = j["r_max"];
    
    conf.RvMax = j["RvMax"];
    conf.RvMin = j["RvMin"];

    conf.densfilename = j["densfilename"];
    conf.voidfilename = j["voidfilename"]; 
    conf.outfilename = j["outfilename"]; 

    config_in.close();

    // Computed parameters
    conf.Ndens = pow(conf.n_large_grid, 3);
    conf.cell_side_size = conf.boxsize / conf.n_large_grid;
    
    conf.n_shells = (conf.r_max - conf.ri) / conf.shell_width;
    conf.rf = conf.ri + (conf.n_shells - 1) * conf.shell_width;
    
    conf.n_small_grid = 2 * trunc(conf.r_max / conf.cell_side_size) + 3;

    return conf;
}

void print_configuration(configuration conf) {
    std::cout<<"############\nThe configuration is:\n\n";
    std::cout<<"###\nSet by the user:\n";
    
    std::cout<<"n_voids="<<conf.n_voids<<"\n";
    std::cout<<"n_large_grid="<<conf.n_large_grid<<"\n";
    std::cout<<"boxsize="<<conf.boxsize<<"\n";
    std::cout<<"ri="<<conf.ri<<"\n";
    std::cout<<"shell_width="<<conf.shell_width<<"\n";
    std::cout<<"r_max="<<conf.r_max<<"\n";
    std::cout<<"min void radius="<<conf.RvMin<<"\n";
    std::cout<<"max void radius="<<conf.RvMax<<"\n";
    std::cout<<"density file name="<<conf.densfilename<<"\n";
    std::cout<<"void file name="<<conf.voidfilename<<"\n";
    std::cout<<"output file name="<<conf.outfilename<<"\n";
    
    std::cout<<"\n###\nComputed:\n";
    std::cout<<"Ndens = "<<conf.Ndens<<"\n";
    std::cout<<"cell_side_size = "<<conf.cell_side_size<<"\n";
    std::cout<<"n_shells = "<<conf.n_shells<<"\n";
    std::cout<<"rf = "<<conf.rf<<"\n";
    std::cout<<"n_small_grid = "<<conf.n_small_grid<<"\n############\n\n";
}