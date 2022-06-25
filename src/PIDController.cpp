//
// Created by Matous Hybl on 2018-10-28.
//

#include "roboutils/PIDController.h"

#include <cmath>
#include "roboutils/util/swift.h"
#include "roboutils/util/timing.h"
#include "roboutils/util/mathematic.h"

namespace RoboUtils {

    PIDController::PIDController(float p, float i, float d, unsigned int sampleTimeMs, float absoluteMaximumAction) {
        this->p = p;
        this->i = i;
        this->d = d;
        this->sampleTimeMs = sampleTimeMs;
        this->absoluteMaximumAction = absoluteMaximumAction;
    }

    void PIDController::tune(float p, float i, float d) {
        this->p = p;
        this->i = i;
        this->d = d;
    }

    float PIDController::update(float setpoint, float currentValue) {
        let error = setpoint - currentValue;

        integrator += error * sampleTimeMs;
        integrator = saturate(integrator, -absoluteMaximumAction, absoluteMaximumAction);

        let integral = i * integrator;

        let derivative = d * (error - previousError) / sampleTimeMs;

        previousError = error;

        return saturate(p * (error + integral + derivative), -absoluteMaximumAction, absoluteMaximumAction);
    }
}
