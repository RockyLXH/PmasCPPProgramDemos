/*
 * pid.cpp
 *
 * Created on: Jul 25, 2022
 * Author: RockyLiu
 */

#include "pid.h"

PIDController::PIDController(double kp, double ki, double kd, double ramp,
		double limit, double ts) :
		kp_(kp), ki_(ki), kd_(kd), output_ramp_(ramp), limit_(limit), ts_(ts)
{

}

double PIDController::operator()(double error)
{
	double proportional = kp_ * error;

	double integral = integral_prev_ + ki_ * ts_ * 0.5f * (error + error_prev_);
	integral = _constrain(integral, -limit_, limit_);

	double derivative = kd_ * (error - error_prev_) / ts_;

	double output = proportional + integral + derivative;
	output = _constrain(output, -limit_, limit_);

	if (output_ramp_ > 0)
	{
		double output_rate = (output - output_prev_) / ts_;

		if (output_rate > output_ramp_)
			output = output_prev_ + output_ramp_ * ts_;
		else if (output_rate < -output_ramp_)
			output = output_prev_ - output_ramp_ * ts_;
	}

	integral_prev_ = integral;
	output_prev_ = output;
	error_prev_ = error;

	return output;
}

