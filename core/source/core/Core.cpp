#include "Core.h"

#include <cmath>
#include <cstddef>
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

void Uniform_Sphere_Sim_2d::setParticle(const particle_2d& part, const size_t& element)
{
    _particles[element] = part;
}

void Uniform_Sphere_Sim_2d::setTimeModifier(const coord_type& time_modifier)
{
    _time_modifier = time_modifier;
}

void Uniform_Sphere_Sim_2d::runAsync(const float& elapsedTime)
{
    _coords_ready = false;
    coord_type deltaT = elapsedTime * _time_modifier;

    for (int i = 0; i < _coordinate_array_size; i += 2)
    {
        int x = i / 2;
        // Find radius
        coord_type radius = _particles->radius;

        // Checks for out of bounds particles, and gives them a helping hand to get back within
        // bounds
        if (_particles[x].out_of_bounds > 100)
        {
            if (_particles[x].xpos - radius < _rectangle_dims[bottom_left_x])
                _particles[x].vel_x = std::fabs(_particles[x].vel_x) + 1000;
            else if (_particles[x].xpos + radius > _rectangle_dims[top_right_x])
                _particles[x].vel_x = std::fabs(_particles[x].vel_x) * -1 - 1000;
            if (_particles[x].ypos - radius < _rectangle_dims[bottom_left_y])
                _particles[x].vel_y = std::fabs(_particles[x].vel_y) + 1000;
            else if (_particles[x].ypos + radius > _rectangle_dims[top_right_y])
                _particles[x].vel_y = std::fabs(_particles[x].vel_y) * -1 + 1000;

            _particles[x].out_of_bounds = 0;
        }

        // Border checks
        // X
        if (_particles[x].xpos - radius < _rectangle_dims[bottom_left_x]) // check border left
        {
            _particles[x].vel_x = std::fabs(_particles[x].vel_x) * _bounce_losses;
            _particles[x].xpos += _particles->radius/80;
            _particles[x].out_of_bounds +=2;
        }
        else if (_particles[x].xpos + radius > _rectangle_dims[top_right_x]) // check border right
        {
            _particles[x].vel_x = std::fabs(_particles[x].vel_x) * -1 * _bounce_losses;
            _particles[x].xpos -= _particles[x].radius/80;
            _particles[x].out_of_bounds +=2;
        }
        else {
            _particles[x].out_of_bounds *=0.5; // if i just minus 1 then will intager underflow and bump
        }

        // Y
        if (_particles[x].ypos - radius < _rectangle_dims[bottom_left_y]) // check border below
        {
            _particles[x].vel_y = std::fabs(_particles[x].vel_y) * _bounce_losses;
            _particles[x].ypos += _particles[x].radius/80;
            _particles[x].out_of_bounds +=2;
        }
        else if (_particles[x].ypos + radius > _rectangle_dims[top_right_y]) // check border above
        {
            _particles[x].vel_y = std::fabs(_particles[x].vel_y) * -1 * _bounce_losses;
            _particles[x].ypos -= _particles[x].radius/80;
            _particles[x].out_of_bounds +=2;
        }
        else {
            _particles[x].out_of_bounds *=0.5;
        }

        // Finding Speeds
        coord_type uvx = _particles[x].vel_x;
        _particles[x].vel_x = uvx;
        coord_type uvy = _particles[x].vel_y;
        _particles[x].vel_y = uvy + gravity * deltaT;

        // Position increase
        _particles[x].xpos += deltaT * 0.5 * (uvx + _particles[x].vel_x);
        _particles[x].ypos += deltaT * 0.5 * (uvy + _particles[x].vel_y);

        // Update array
        _coordinate_array[i] = _particles[x].xpos;
        _coordinate_array[i + 1] = _particles[x].ypos;
    }
    _coords_ready = true;

    return;
}

size_t Uniform_Sphere_Sim_2d::setParticleCount(const size_t& particles)
{
    _particle_count = particles;

    if (_coordinate_array_size != particles * 2)
    {
        if (_coordinate_array != nullptr)
            delete[] _coordinate_array;
        if (_particles != nullptr)
            delete[] _particles;

        _coordinate_array_size = particles * 2;
        _coordinate_array = new coord_type[_coordinate_array_size] { 0 };
        _particles = new particle_2d[_particle_count];
    }

    return _coordinate_array_size;
}

Uniform_Sphere_Sim_2d::Uniform_Sphere_Sim_2d()
    : _coords_ready(false)
    , _coordinate_array_size(0)
    , tempmod(0)
    , _coordinate_array(nullptr)
    , _particles(nullptr)
    , _time_modifier(1)
    , _bounce_losses(0.5)

{
    _rectangle_dims[0] = 10;
    _rectangle_dims[1] = 10;
    _rectangle_dims[2] = 100;
    _rectangle_dims[3] = 100;
}

Uniform_Sphere_Sim_2d::~Uniform_Sphere_Sim_2d()
{
    // delete _coordinate_array[_coordinate_array_size];
    delete[] _particles;
    delete[] _coordinate_array;
}

} // namespace crbn
