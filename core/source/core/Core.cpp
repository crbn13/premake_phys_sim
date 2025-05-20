#include "Core.h"

#include <cmath>
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

void Uniform_Sphere_Sim_2d::setParticle(particle_2d& part) { _particles[0] = part ; }

void Uniform_Sphere_Sim_2d::runAsync(float elapsedTime)
{
    _coords_ready = false;
    tempmod += elapsedTime;

    for (int i = 0; i < _coordinate_array_size; i += 2)
    {
        int x = i / 2;
        coord_type uvx = _particles[x].vel_x;
        _particles[x].vel_x = uvx;
        _particles[x].xpos = elapsedTime * 0.5 * (uvx + _particles[x].vel_x);
        coord_type uvy = _particles[x].vel_y;
        _particles[x].vel_y = uvy + acceleration * elapsedTime;
        _particles[x].ypos = elapsedTime * 0.5 * (uvy + _particles[x].vel_y);

        _coordinate_array[i] = _particles[x].xpos;
        _coordinate_array[i + 1] = _particles[x].xpos;

        if (_particles[x].xpos < 0)
            _particles[x].vel_y = std::fabs(_particles[x].vel_y);
        if (_particles[x].ypos < 0)
            _particles[x].vel_y = std::fabs(_particles[x].vel_y);
    }


    _coords_ready = true;

    return;
}

size_t Uniform_Sphere_Sim_2d::setParticleCount(size_t particles)
{
    _particle_count = particles;

    while (!_coords_ready)
    {
    } // wait
    if (_coordinate_array_size != particles * 2)
    {
        delete[] _coordinate_array;
        _coordinate_array_size = particles * 2;
        _coordinate_array = new coord_type[_coordinate_array_size];
        delete[] _particles;
        _particles = new particle_2d[_particle_count];
    }

    return _coordinate_array_size;
}

Uniform_Sphere_Sim_2d::Uniform_Sphere_Sim_2d()
    : _coords_ready(false)
    , _coordinate_array_size(0)
    , tempmod(0)
{
    _coordinate_array = new coord_type[2];
}

Uniform_Sphere_Sim_2d::~Uniform_Sphere_Sim_2d()
{
    // delete _coordinate_array[_coordinate_array_size];
    delete[] _coordinate_array;
}

} // namespace crbn
