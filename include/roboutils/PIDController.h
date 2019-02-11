//
// Created by Matous Hybl on 2018-10-28.
//

#ifndef FIRMWARE_PIDCONTROLLER_H
#define FIRMWARE_PIDCONTROLLER_H

namespace RoboUtils {
    class PIDController {
    public:
        PIDController(float p, float i, float d, unsigned int sampleTimeMs, float absoluteMaximumAction);

        void tune(float p, float i, float d);

        float update(float setpoint, float currentValue);

    private:
        float p = 0;
        float i = 0;
        float d = 0;

        float previousError = 0;
        float integrator = 0;
        unsigned int sampleTimeMs = 0;
        float absoluteMaximumAction = 0;
    };
};


#endif //FIRMWARE_PIDCONTROLLER_H
