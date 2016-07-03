//
//  Geminiq.c
//  Source written by Justin Yu
//

#include "Geminiq.h"

static void init() {
    controller.enable_servos();
    create.connect();
    gemini.controller.servo(ARM_SERVO, ARM_UP);
}

static void deinit() {
    controller.disable_servos();
    create.disconnect();
}

static void line_follow(int base_speed, int distance) {
    float offset = 0; // average of white and black sensor values (660, 985) = 822.5
    float integral = 0; // running total of errors
    float last_error = 0;
    float derivative = 0; // current error - last error

    int sensor_value, turn;
    float error;

    float kp = 2.4, // proportional constant
          ki = 0.1,  // integral constant
          kd = 8.2; // derivative constant

    int speed = base_speed; // speed of motors at error = 0

    offset = 823.; // average of the calibration readings
	cmpc(gemini.controller.motor_left);
    while(gmpc(gemini.controller.motor_left) < distance) {
      	printf("%d\n", gemini.controller.gmpc(gemini.controller.motor_left));
        sensor_value = analog10(gemini.tophat_port);

        error = sensor_value - offset;
        integral = (7. / 8.) * integral + error;
        derivative = error - last_error;

        turn = (int)(kp * error + ki * integral + kd * derivative);

        mav(gemini.controller.motor_left, speed + turn);
        mav(gemini.controller.motor_right, speed - turn);

        last_error = error;

        msleep(1);
    }
}

static void line_follow_until_et(int speed) {
    float offset = 0; // average of white and black sensor values (660, 985) = 822.5
    float integral = 0; // running total of errors
    float last_error = 0;
    float derivative = 0; // current error - last error

    int sensor_value, turn;
    float error;

    float kp = 2.4, // proportional constant
          ki = 0.1,  // integral constant
          kd = 8.2; // derivative constant

    // int speed = 1100; // speed of motors at error = 0

    offset = 823.; // average of the calibration readings
    while(gemini.is_outer_space() == 0) {
        sensor_value = analog10(gemini.tophat_port);

        error = sensor_value - offset;
        integral = (7. / 8.) * integral + error;
        derivative = error - last_error;

        turn = (int)(kp * error + ki * integral + kd * derivative);

        mav(gemini.controller.motor_left, speed + turn);
        mav(gemini.controller.motor_right, speed - turn);

        last_error = error;

        msleep(1);
    }
}

static int get_et_value() {
    return analog(gemini.et_port);
}

static int is_outer_space() {
    return get_et_value() > 650 ? 0 : 1;
}

static void lower_arm() {
    gemini.controller.servo(ARM_SERVO, ARM_DOWN);
    msleep(200);
}

Gemini new_gemini(int tophat_port, int et_port) {
    Gemini instance = {
        .tophat_port = tophat_port,
        .et_port = et_port,
        .init = &init,
        .deinit = &deinit,
        .line_follow = &line_follow,
        .get_et_value = &get_et_value,
        .is_outer_space = &is_outer_space,
        .line_follow_until_et = &line_follow_until_et,
        .lower_arm = &lower_arm
    };

    instance.controller = new_controller(1, 0, 14.0, 5.0);
    instance.create = new_create();

    gemini = instance;
    return instance;
}
