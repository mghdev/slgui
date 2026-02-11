#ifndef SILHOUETTE_INCLUDE_SLCONFIGMODEL_H_
#define SILHOUETTE_INCLUDE_SLCONFIGMODEL_H_

#include <queue>
#include <string>

class SLConfigModel
{
public:
    double total_runtime;
    double counted_time;
    std::queue<double> frame_times;
    
    std::string filename;

    bool in_fullscreen;

    SLConfigModel();
    ~SLConfigModel();
    
    void step(double dt);
};

#endif