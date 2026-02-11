#include "SLConfigModel.hpp"


SLConfigModel::SLConfigModel() : total_runtime(0.0f),counted_time(0.0f),in_fullscreen(false)
{
    for (size_t i = 0; i < 120; i++)
    {
        frame_times.push(0.0f);
    }
}

SLConfigModel::~SLConfigModel()
{
}

void SLConfigModel::step(double dt)
{
    total_runtime += dt;
    counted_time += dt;
    counted_time -= frame_times.front();
    frame_times.push(dt);
    frame_times.pop();
}
