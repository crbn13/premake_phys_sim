#include "Core.h"

#include <cmath>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <ostream>
#include <utility>
#include <vector>

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
    return _coordinate_array.data();
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
        coord_type radius = _particles[x].radius;

        // Checks for out of bounds particles, and gives them a helping hand to get back within
        // bounds
        if (_particles[x].out_of_bounds > 10)
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
            _particles[x].xpos += _particles[x].radius / 80;
            _particles[x].out_of_bounds += 8;
        }
        else if (_particles[x].xpos + radius > _rectangle_dims[top_right_x]) // check border right
        {
            _particles[x].vel_x = std::fabs(_particles[x].vel_x) * -1 * _bounce_losses;
            _particles[x].xpos -= _particles[x].radius / 80;
            _particles[x].out_of_bounds += 8;
        }
        else
        {
            _particles[x].out_of_bounds *= 0.8; // if i just minus 1 then will intager underflow and
                                                // bump
        }

        // Y
        if (_particles[x].ypos - radius < _rectangle_dims[bottom_left_y]) // check border below
        {
            _particles[x].vel_y = std::fabs(_particles[x].vel_y) * _bounce_losses;
            _particles[x].ypos += _particles[x].radius / 80;
            _particles[x].out_of_bounds += 8;
        }
        else if (_particles[x].ypos + radius > _rectangle_dims[top_right_y]) // check border above
        {
            _particles[x].vel_y = std::fabs(_particles[x].vel_y) * -1 * _bounce_losses;
            _particles[x].ypos -= _particles[x].radius / 80;
            _particles[x].out_of_bounds += 8;
        }
        else
        {
            _particles[x].out_of_bounds *= 0.8;
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

    dirtyCollisionDetector();

    _coords_ready = true;

    return;
}

size_t Uniform_Sphere_Sim_2d::setParticleCount(const size_t& particles)
{
    _particle_count = particles;

    if (_coordinate_array_size != particles * 2)
    {
        _coordinate_array_size = particles * 2;
        _coordinate_array.resize(_coordinate_array_size);
        _particles.resize(_particle_count);
    }

    return _coordinate_array_size;
}

void Uniform_Sphere_Sim_2d::dirtyCollisionDetector()
{
    // divides particles up into 3d chunks
    // std::vector<std::vector<std::vector<particle_2d*>>> xyChunks;
    std::vector<std::vector<particle_2d*>> xyChunks;

    int chunksCount = _chunks.first * _chunks.second;

    xyChunks.resize(chunksCount);

    for (auto& x : xyChunks)
    {
        x.reserve(((float)_particle_count / chunksCount) * 2 + 5);
    }

    for (int i = 0; i < _particle_count; i++)
    {
        std::cout << "num = " << _particles[i].ypos;

        coord_type leny
            = ((_rectangle_dims[top_right_y] - _rectangle_dims[bottom_left_y])
               / (float)_chunks.second);
        coord_type lenx
            = ((_rectangle_dims[top_right_x] - _rectangle_dims[bottom_left_x])
               / (float)_chunks.first);
        // std::cout << "len = " << len << std::endl;;
        //  sort out particles into their y chunks
        for (int y = 0; y < _chunks.second; y++)
        { // i could do binary search instead
            // std::cout << _rectangle_dims[bottom_left_y] + (y+1) * len << "\n";
            if (_particles[i].ypos < (_rectangle_dims[bottom_left_y] + (y + 1) * leny))
            {
                for (int x = 0; x < _chunks.first; x++)
                    if (_particles[i].xpos < (_rectangle_dims[bottom_left_x] + (x + 1) * lenx))
                    {
                        xyChunks[x + y * _chunks.second].emplace_back(&_particles[i]);
                        // xyChunks[x][y].emplace_back(&_particles[i]);
                        x = _chunks.first;
                        y = _chunks.second;
                    }
            }
        }
    }

    for (int y = 0; y < _chunks.second; y++)
        for (int x = 0; x < _chunks.first; x++)
        {
            // Logging
            std::cout << "\nCHUNK (" << x << "," << y << ")  DATA : \n";
            for (auto& z : xyChunks[x + y * _chunks.second])
                std::cout << "(" << z->xpos << "," << z->ypos << ")\n";

            // Checking for multiple particles in 1 Chunk

            if (xyChunks[x + y * _chunks.second].size() >= 2)
            {
                std::cout << "Size" << xyChunks[x + y * _chunks.second].size() << std::endl;
                dirtyColliderProcess(xyChunks[x + y * _chunks.second]);
            }
        }
    std::cout << std::endl;
}

void Uniform_Sphere_Sim_2d::dirtyColliderProcess(std::vector<particle_2d*>& particles)
{
    std::cout << " Dirty Start \n" << std::flush;
    // Check for colission

    auto collision_check = [&](const particle_2d* p1, const particle_2d* p2) -> bool
    {
        std::cout << "collision checker " << std::endl;
        if (sqrt(pow(p1->xpos - p2->xpos, 2) + pow(p1->ypos - p2->ypos, 2))
            < (p1->radius + p2->radius))
            return true;

        return false;
    };

    for (int x = 0; x< particles.size()-1; x++)
        for (int x1 = 1; x1 < particles.size(); x1++)
            if (collision_check(particles[x], particles[x1]))
            {
                std::cout << "SWAPPING\n" << std::flush;
                coord_type tmpx, tmpx1;
                tmpx = particles[x]->vel_y;
                tmpx1 = particles[x1]->vel_y;
                particles[x]->vel_y = particles[x1]->vel_x;
                particles[x1]->vel_y = particles[x]->vel_x;
                particles[x]->vel_x = tmpx1;
                particles[x1]->vel_x = tmpx;
            }
    std::cout << " Dirty End \n" << std::flush;
}

Uniform_Sphere_Sim_2d::Uniform_Sphere_Sim_2d()
    : _coords_ready(false)
    , _coordinate_array_size(0)
    , tempmod(0)
    , _time_modifier(1)
    , _bounce_losses(0.5)
    , _chunks(5, 5)
{
    _rectangle_dims[0] = 10;
    _rectangle_dims[1] = 10;
    _rectangle_dims[2] = 100;
    _rectangle_dims[3] = 100;
}

Uniform_Sphere_Sim_2d::~Uniform_Sphere_Sim_2d() { }

} // namespace crbn
