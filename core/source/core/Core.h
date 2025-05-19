#pragma once

namespace crbn
{

void start_sim();

struct particle_2d
{
    double _mass;
    double _vel_y;
    double _vel_x;
};

typedef double coord_type; 

class Sphere_Sim_2d
{
public: // Public callable functions :

    // Called to run a single "frame" of the simulation
    coord_type run(float elapsedTime);


    Sphere_Sim_2d();

private: // Member Variables
    coord_type* _coordinate_array;

private: // Member Functions

};

}
