#include "Core.h"

#include <cstdio>
#include <iostream>

namespace crbn
{

void PrintHelloWorld()
{
    std::cout << "Hello World!\n";
    std::cin.get();
}

void start_sim() { std::cout << "simulatoring time\n"; }

coord_type* Uniform_Sphere_Sim_2d::getCoordBuf()
{
    while (!_coords_ready)
    {
    }
    return _coordinate_array;
}

void Uniform_Sphere_Sim_2d::runAsync(float elapsedTime)
{
    _coords_ready = false;
    tempmod += elapsedTime;

    particle._pos.x += elapsedTime * tempmod;

    _coordinate_array[0] = particle._pos.x;
    _coordinate_array[1] = particle._pos.y;

    _coords_ready = true;

    return;
}

Uniform_Sphere_Sim_2d::Uniform_Sphere_Sim_2d()
    : _coords_ready(false)
    , _coordinate_array_size(2)
    , tempmod(0)
{
    _coordinate_array = new double[_coordinate_array_size] { 0 };
}

Uniform_Sphere_Sim_2d::~Uniform_Sphere_Sim_2d()
{
    // delete _coordinate_array[_coordinate_array_size];
    delete[] _coordinate_array;
}

} // namespace crbn
