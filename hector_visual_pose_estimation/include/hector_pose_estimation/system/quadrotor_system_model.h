//=================================================================================================
// Copyright (c) 2013, Thorsten Graber and Johannes Meyer, Alexander Sendobry and Martin Nowara, TU Darmstadt
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the Flight Systems and Automatic Control group,
//       TU Darmstadt, nor the names of its contributors may be used to
//       endorse or promote products derived from this software without
//       specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//=================================================================================================

#ifndef HECTOR_POSE_ESTIMATION_QUADROTOR_SYSTEM_MODEL_H
#define HECTOR_POSE_ESTIMATION_QUADROTOR_SYSTEM_MODEL_H

#include <hector_quadrotor_model/quadrotor_model.h>
#include <hector_pose_estimation/system_model.h>
#include <hector_pose_estimation/system/motor_voltage_input.h>

namespace hector_pose_estimation {

class QuadrotorSystemModel;
template <> struct Input_<QuadrotorSystemModel> {
    typedef MotorVoltageInput Type;
};

class QuadrotorSystemModel : public SystemModel
{
public:
    static const unsigned int InputDimension = MotorVoltageInput::InputDimension;
    static const unsigned int InternalStateDimension = 4; // 4 motor voltages
    typedef MotorVoltageInput::InputVector InputVector;

    QuadrotorSystemModel();
    virtual ~QuadrotorSystemModel();

    virtual std::string getName() const { return "QuadrotorSystemModel"; }
    virtual bool init();

    virtual SystemStatus getStatusFlags() const;

    virtual ColumnVector ExpectedValueGet(double dt) const;
    virtual SymmetricMatrix CovarianceGet(double dt) const;
    virtual Matrix dfGet(unsigned int i, double dt) const;

    virtual void Limit(StateVector& x) const;

protected:
    static void normalize(StateVector& x);
    geometry_msgs::Vector3 getBodyVel(const geometry_msgs::Vector3& nav_vel) const;

protected:
    hector_quadrotor_model::Quadrotor* quad_model;

    double voltage_stddev_;
    double rate_stddev_;
    double angular_acceleration_stddev_;
    double acceleration_stddev_;
    double velocity_stddev_;

    mutable double q0,q1,q2,q3;
    mutable SymmetricMatrix_<StateDimension> noise_;
};

} // namespace hector_pose_estimation

#endif // HECTOR_POSE_ESTIMATION_QUADROTOR_SYSTEM_MODEL_H
