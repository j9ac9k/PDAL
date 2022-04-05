#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <fstream>

#include "Pulse.hpp"
#include "SplineFit.hpp"

namespace LidarTrajectory
{

class PulseCollection
{
    struct Args;

private:
    bool m_first;
    double m_lastAdjTime;
    double m_timeOrigin;
    double m_timeMin;
    double m_timeMax;
    Eigen::Vector3d m_rOrigin;
    Eigen::Vector3d m_rlow;
    Eigen::Vector3d m_rhigh;
    int m_lowhigh;
    std::vector<Pulse> m_multiBuf;
    std::vector<Pulse> m_singleBuf;
    std::unique_ptr<Args> m_args;

    void addPoint(double time, const Eigen::Vector3d& startPos, const Eigen::Vector3d& endPos,
        double angle);
    void registerMulti(const std::vector<Pulse>& buf);
    void registerSingle(const std::vector<Pulse>& buf);
    
    // Estimate pos + vel at t based on pulses within +/- tblock at t.
    bool EstimatedPositionVelocity(double t, Eigen::Vector3d& r,
                                   Eigen::Vector3d& v) const;
    void InitializeTrajectory();
    // Do the ceres solution

public:
    PulseCollection();
    ~PulseCollection();

//    double tmin, tmax;

    std::vector<Pulse> pulses;
    SplineFit3 traj;

    // scan angle
    SplineFit2 attitude;

    // Ingest a lidar return
    void Add(double t, const Eigen::Vector3d& r, int num, int ret, double ang);
    void Solve();

    Eigen::Vector3d Trajectory(double t,
                               Eigen::Vector3d& v, Eigen::Vector3d& a) const;
    Eigen::Vector2d Attitude(double t, Eigen::Vector2d& v) const;
    /* Return the environment variable SRI_QTM_CONFIG_DIR.  If it's
     * not defined return ".". */
    static std::string ConfigRoot();
  };
}
