//
//  Scorpion.c
//  Source written by Justin Yu
//

#include "Scorpion.h"
#include <math.h>

// custom subroutines

static void init() {
    scorpion.create.connect();
    scorpion.camera.open();

    scorpion.controller.enable_servos();
    scorpion.controller.servo(BACK_SERVO, BACK_UP);
    scorpion.set_claw_to_position(CLAW_PARTIAL);
    scorpion.set_arm_to_position(ARM_UP);
}

static void deinit() {
    scorpion.controller.disable_servos();
    scorpion.camera.close();
    scorpion.create.disconnect();
}

static void shake_arm() {
    scorpion.controller.servo(ARM_SERVO, 0);
    msleep(250);
    int i;
    for(i = 0; i < 3; i++) {
        scorpion.controller.servo(ARM_SERVO, ARM_UP);
        msleep(200);
        scorpion.controller.servo(ARM_SERVO, 0);
        msleep(200);
    }}

static void lower_arm() {
    scorpion.controller.slow_servo(ARM_SERVO, ARM_DOWN, 0.6);
}

static void raise_arm() {
    scorpion.controller.slow_servo(ARM_SERVO, ARM_UP + 400, 0.6);
    scorpion.controller.slow_servo(ARM_SERVO, ARM_UP, 0.2);
    // scorpion.controller.slow_servo(ARM_SERVO, ARM_UP, SERVO_DURATION);
}

static void raise_arm_slow(float time) {
    scorpion.controller.slow_servo(ARM_SERVO, ARM_UP, time);
}

static void open_claw() {
    scorpion.controller.servo(CLAW_SERVO, CLAW_OPEN);
    msleep(150);
}

static void open_claw_slow(float time) {
    scorpion.controller.slow_servo(CLAW_SERVO, CLAW_OPEN, time);
}

static void close_claw() {
    scorpion.controller.servo(CLAW_SERVO, CLAW_CLOSED);
    msleep(150);
}

static void close_claw_slow(float time) {
    scorpion.controller.slow_servo(CLAW_SERVO, CLAW_CLOSED, time);
}

static void set_claw_to_position(int position) {
    scorpion.controller.servo(CLAW_SERVO, position);
    msleep(150);
}

static void set_arm_to_position(int position) {
    scorpion.controller.servo(ARM_SERVO, position);
    msleep(150);
}

static void lift_basket() {
    scorpion.controller.servo(BACK_SERVO, BACK_UP);
    msleep(250);
}

static void drop_basket() {
    scorpion.controller.servo(BACK_SERVO, BACK_DOWN);
    msleep(250);
}

static void lift_basket_slow() {
    scorpion.controller.slow_servo(BACK_SERVO, BACK_UP, SERVO_DURATION);
}

static void drop_basket_slow() {
    scorpion.controller.slow_servo(BACK_SERVO, BACK_DOWN, SERVO_DURATION);
}

void _open_claw_slow() {
    scorpion.open_claw_slow(0.6);
}

void _close_claw_slow() {
    scorpion.close_claw_slow(1.3);
}

void _raise_arm_slow() {
    scorpion.close_claw_slow(1.0);
}

static void lower_arm__open_claw() { // async call
    thread tid = thread_create(_open_claw_slow);
    thread_start(tid);
    scorpion.lower_arm();
    thread_wait(tid);
    thread_destroy(tid);
}

static void grab_tribbles() {
    scorpion.controller.slow_servo(CLAW_SERVO, CLAW_PARTIAL, 0.2);
    scorpion.grab_tribbles_async();
}

static void grab_tribbles_async() {
    thread tid = thread_create(_close_claw_slow);
    thread_start(tid);
    scorpion.create.forward(12, 250);
    thread_wait(tid);
    thread_destroy(tid);
    scorpion.controller.slow_servo(CLAW_SERVO, CLAW_PARTIAL, 0.4);
    scorpion.close_claw_slow(0.3);
    scorpion.set_claw_to_position(CLAW_PARTIAL);
    scorpion.close_claw();
}

void _claw_partial() {
    scorpion.controller.slow_servo(CLAW_SERVO, CLAW_PARTIAL, 0.3);
}

static void grab_tribbles_slow() {
    scorpion.controller.slow_servo(CLAW_SERVO, CLAW_PARTIAL, 0.3);
    scorpion.set_claw_to_position(CLAW_OPEN - 300);

    thread tid = thread_create(_claw_partial);
    thread_start(tid);
    scorpion.create.forward(14, 250);
    thread_wait(tid);
    thread_destroy(tid);

    scorpion.grab_tribbles_async();
}

static enum Channel get_tribble_color() {
    scorpion.camera.update();
    scorpion.camera.update();

    enum Channel desired_channel;

    scorpion.create.drive_direct(20, 20);
    while(1) {
        scorpion.camera.update();
        scorpion.camera.update();
        scorpion.camera.update();

        int red_area = scorpion.camera.get_object_area(RED_CHANNEL, 0);
        int green_area = scorpion.camera.get_object_area(GREEN_CHANNEL, 0);
        if(red_area == -1 && green_area == -1) {
            continue;
        }
        if(red_area < green_area) {
            printf("GREEN\n\n");
            desired_channel = GREEN_CHANNEL;
            break;
        } else if(red_area > green_area) {
            printf("RED\n\n");
            desired_channel = RED_CHANNEL;
            break;
        }
        msleep(20);
    }
    scorpion.create.stop();

    return desired_channel;
}

static enum Channel track_tribbles() {
    enum Channel desired_channel = scorpion.get_tribble_color();

    printf("DESIRED CHANNEL = %d\n", desired_channel);

    while(1) {
        scorpion.camera.update();

        int x = scorpion.camera.get_object_center_x(desired_channel, 0);
        if(x >= 0 && x <= 72) { // left
            scorpion.create.spin_counterclockwise(40);
        } else if(x >= 89 && x <= 159) {
            scorpion.create.spin_clockwise(40);
        } else if(x >= 73 && x <= 88) { // center range
             break;
        }
        msleep(5);

        if(scorpion.create.get_lbump() == 1) {
             scorpion.create.right(1, 0, 150);
             break;
        }
        if(scorpion.create.get_rbump() == 1) {
             scorpion.create.left(1, 0, 150);
             break;
        }
    }
    scorpion.create.stop();

    return desired_channel;
}

static void determine_action(int *red, int *green) {
    scorpion.create.backward(10, 200);

    enum Channel channel = get_tribble_color();

    scorpion.create.forward(10, 200);

    if(channel == RED_CHANNEL)
        (*red)++;
    else if(channel == GREEN_CHANNEL)
        (*green)++;
    int total_collected = (*red) + (*green);
    if(((*green) == 0 || (*green) == 1)) { // (no 2 green)
        if(total_collected == 2 || total_collected == 3 || total_collected == 4) // 2nd, 3rd, 4th piles
            scorpion.grab_tribbles();
        else if(total_collected == 1)
            scorpion.grab_tribbles_slow();
    }
    else if(channel == RED_CHANNEL)
        scorpion.grab_tribbles();
    else if(channel == GREEN_CHANNEL && (*green) == 2)
        scorpion.isolate_tribbles();

}

static void isolate_tribbles() {
    scorpion.create.forward(26, 180);
    scorpion.close_claw_slow(0.5);

    int i;
    for(i = 0; i < 15; i++) {
        scorpion.controller.slow_servo(CLAW_SERVO, CLAW_PARTIAL, 0.2);
        scorpion.close_claw();
    }
    scorpion.controller.slow_servo(CLAW_SERVO, CLAW_PARTIAL, 0.4);
    scorpion.create.backward(4, 200);
    for(i = 0; i < 3; i++) {
        scorpion.close_claw();
        msleep(50);
        scorpion.set_claw_to_position(CLAW_PARTIAL);
    }
    scorpion.close_claw();
  /*
    for(i = 0; i < 2; i++) {
        scorpion.close_claw();
        scorpion.set_arm_to_position(ARM_DOWN - 300);

        thread tid = thread_create(lower_arm);
        thread_start(tid);
        scorpion.set_claw_to_position(500);
        thread_wait(tid);
        thread_destroy(tid);

        msleep(500);
    }
  */
}

static int get_tophat_value() {
    return analog10(scorpion.tophat_port);
}

static int get_touch_value() {
    return digital(scorpion.touch_port);
}

static int is_tophat_on_line() {
    return get_tophat_value() > 680 ? 1 : 0;
}

static void forward_until_touch(int speed) {
    scorpion.create.drive_direct(speed, speed);
    while(get_touch_value() == 0);
    scorpion.create.stop();
}

static void forward_until_black(int speed) {
    scorpion.create.drive_direct(speed, speed);
    while(is_tophat_on_line() == 0);
    scorpion.create.stop();
}

static void left_until_black(int speed) {
	scorpion.create.spin_counterclockwise(speed);
    while(is_tophat_on_line() == 0);
    scorpion.create.stop();
}

static void left_until_white(int speed) {
	scorpion.create.spin_counterclockwise(speed);
    while(is_tophat_on_line() == 1);
    scorpion.create.stop();
}

static void right_until_black(int speed) {
    scorpion.create.spin_clockwise(speed);
    while(is_tophat_on_line() == 0);
    scorpion.create.stop();
}

static void right_until_white(int speed) {
    scorpion.create.spin_clockwise(speed);
    while(is_tophat_on_line() == 1);
    scorpion.create.stop();
}

// precondition: just got 5th tribble pile with up, claw open
static void move_botguy() {
    scorpion.set_arm_to_position(1500);

    scorpion.close_claw();
    scorpion.create.forward(32, 300);

    scorpion.create.left(22, 0, 300);
    scorpion.lower_arm();
    scorpion.create.right(12, 0, 420);
    scorpion.set_claw_to_position(CLAW_PARTIAL);

    scorpion.right_until_black(100);
}

static void line_follow(int create_distance) {

    scorpion.create.set_distance(0);
    float kp = 1.6, // proportional constant
          ki = 0.1,  // integral constant
          kd = 4.2; // derivative constant

    int speed = 200; // speed of motors at error = 0
    float offset = 823.;

    float integral = 0; // running total of errors
    float last_error = 0;
    float derivative = 0; // current error - last error

    int sensor_value, turn;
    float error;

    while(scorpion.create.get_distance() < create_distance) {
        sensor_value = scorpion.get_tophat_value();

        error = sensor_value - offset;
        integral = (7. / 8.) * integral + error;
        derivative = error - last_error;

        turn = (int)(kp * error + ki * integral + kd * derivative);
		turn /= 15;

        scorpion.create.drive_direct(speed + turn, speed - turn);

        last_error = error;

        msleep(50);
    }
    scorpion.create.stop();
}

static void line_follow_backward(int create_distance) {
    scorpion.create.set_distance(0);
    float kp = 1.6, // proportional constant
          ki = 0.1,  // integral constant
          kd = 4.2; // derivative constant

    int speed = -200; // speed of motors at error = 0
    float offset = 823.;

    float integral = 0; // running total of errors
    float last_error = 0;
    float derivative = 0; // current error - last error

    int sensor_value, turn;
    float error;

    while(scorpion.create.get_distance() > create_distance) {
        sensor_value = scorpion.get_tophat_value();

        error = sensor_value - offset;
        integral = (7. / 8.) * integral + error;
        derivative = error - last_error;

        turn = (int)(kp * error + ki * integral + kd * derivative);
		turn /= 15;

        scorpion.create.drive_direct(speed + turn, speed - turn);

        last_error = error;

        msleep(50);
    }
    scorpion.create.stop();
}

void score_bin() {
    scorpion.create.left(160, 0, 250);
    scorpion.left_until_black(100);
    scorpion.create.backward(35, 300);
    scorpion.set_arm_to_position(ARM_PARTIAL);
    scorpion.drop_basket_slow();
}

void score(enum Case tribble_pile) {
    switch(tribble_pile) {
    case PILE_ONE:
        scorpion.create.backward(20, 400);
        scorpion.lower_arm__open_claw();
        scorpion.create.forward(22, 300);
        scorpion.isolate_tribbles();
        scorpion.set_arm_to_position(ARM_PARTIAL + 400);

        scorpion.line_follow(1300);
        scorpion.raise_arm();
        scorpion.set_claw_to_position(CLAW_PARTIAL);
        score_bin();
        scorpion.create.right(6, 0, 300);
        break;
    case PILE_TWO:
        scorpion.create.forward(5, 300);
        scorpion.set_arm_to_position(ARM_PARTIAL + 200);
        scorpion.open_claw();
        scorpion.create.backward(15, 280);
        scorpion.lower_arm();
        scorpion.create.forward(5, 250);
        scorpion.isolate_tribbles();
        scorpion.raise_arm_slow(0.8);
        scorpion.controller.slow_servo(CLAW_SERVO, CLAW_PARTIAL, 0.4);
        scorpion.create.left(85, 0, 300);
        scorpion.create.backward(40, 300);
        scorpion.create.backward(20, 250);

        scorpion.create.forward(8, 300);
        scorpion.create.left(83, 0, 300);
        scorpion.create.backward(105, 300);

        scorpion.set_arm_to_position(ARM_PARTIAL);
        scorpion.drop_basket_slow();
        scorpion.create.right(6, 0, 300);
        break;
    case PILE_THREE:
        scorpion.lower_arm__open_claw();
        scorpion.set_claw_to_position(CLAW_OPEN - 200);
        scorpion.isolate_tribbles();
        scorpion.raise_arm_slow(0.8);
        scorpion.controller.slow_servo(CLAW_SERVO, CLAW_PARTIAL, 0.5);

        scorpion.create.left(85, 0, 300);
        scorpion.create.backward(30, 300);
        scorpion.create.backward(20, 250);

        scorpion.create.forward(7, 300);
        scorpion.create.left(79, 0, 300);
        scorpion.create.backward(73, 300);

        scorpion.set_arm_to_position(ARM_PARTIAL);
        scorpion.drop_basket_slow();
        scorpion.create.right(6, 0, 300);
        break;
    }
}

Scorpion new_scorpion(int tophat, int touch) {
	Scorpion instance = {
        // Assign instance properties
        .tophat_port = tophat,
        .get_tophat_value = &get_tophat_value,
        .touch_port = touch,
        .get_touch_value = &get_touch_value,
        .grab_tribbles = &grab_tribbles,
        .grab_tribbles_slow = &grab_tribbles_slow,
        .grab_tribbles_async = &grab_tribbles_async,
        .determine_action = &determine_action,
        .isolate_tribbles = &isolate_tribbles,
        .shake_arm = &shake_arm,
        .lower_arm = &lower_arm,
        .raise_arm = &raise_arm,
        .raise_arm_slow = &raise_arm_slow,
        .open_claw = &open_claw,
        .close_claw = &close_claw,
        .close_claw_slow = &close_claw_slow,
        .open_claw_slow = &open_claw_slow,
        .set_claw_to_position = &set_claw_to_position,
        .set_arm_to_position = &set_arm_to_position,
        .lift_basket = &lift_basket,
        .drop_basket = &drop_basket,
        .lift_basket_slow = &lift_basket_slow,
        .drop_basket_slow = &drop_basket_slow,
        .track = &track_tribbles,
        .get_tribble_color = &get_tribble_color,
        .init = &init,
        .deinit = &deinit,
        .left_until_black = &left_until_black,
        .right_until_black = &right_until_black,
        .right_until_white = &right_until_white,
        .left_until_white = &left_until_white,
        .forward_until_black = &forward_until_black,
        .lower_arm__open_claw = &lower_arm__open_claw,
        .move_botguy = &move_botguy,
        .score = &score,
        .forward_until_touch = &forward_until_touch,
        .line_follow = &line_follow,
        .line_follow_backward = &line_follow_backward
    };
    instance.create = new_create();
    instance.controller = new_create_controller();
    instance.camera = new_camera();
    return instance;
}
