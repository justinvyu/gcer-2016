#include <kipr/botball.h>
#include "Controller.h"

int main()
{
    controller = new_controller(1, 0, 14.5, 5.0);

    // initialize variables

    int tophat = 5;
    int et = 1, et_threshold = 1700; // greater than 1600 means that the pole is there

    // PID

    float kp = 2.4, // proportionality constant
          ki = 0.1,  // integral constant
          kd = 8.2; // derivative constant
    float offset = 823; // average of white and black sensor values (660, 985) = 822.5
    int tp = 1080; // speed of motors at error = 0
    float integral = 0; // running total of errors
    float last_error = 0;
    float derivative = 0; // current error - last error

    int sensor_value, turn;
    float error;

    while(1) {
        sensor_value = analog10(tophat);
        error = sensor_value - offset;
        integral = (7. / 8.) * integral + error;
        derivative = error - last_error;

        // printf("%d, %d\n", kp, error);

        turn = (int)(kp * error + ki * integral + kd * derivative);

        // printf("turn: %d\n", turn);
        //
        mav(0, tp - turn);
        mav(1, tp + turn);

        last_error = error;

        if(analog(et) > et_threshold) break; // sees pole

        msleep(1);
    }

    return 0;
}
