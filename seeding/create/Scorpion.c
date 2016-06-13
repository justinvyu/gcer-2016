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
    scorpion.controller.slow_servo(ARM_SERVO, ARM_DOWN, 0.8);
}

static void raise_arm() {
    scorpion.controller.servo(ARM_SERVO, ARM_UP);
    msleep(700);
    // scorpion.controller.slow_servo(ARM_SERVO, ARM_UP, SERVO_DURATION);
}

static void raise_arm_slow(float time) {
    scorpion.controller.slow_servo(ARM_SERVO, ARM_UP, time);
}

static void open_claw() {
    scorpion.controller.servo(CLAW_SERVO, CLAW_OPEN);
    msleep(200);
}

static void close_claw() {
    scorpion.controller.servo(CLAW_SERVO, CLAW_CLOSED);
    msleep(200);
}

static void close_claw_slow(float time) {
    scorpion.controller.slow_servo(CLAW_SERVO, CLAW_CLOSED, time);
}

static void set_claw_to_position(int position) {
    scorpion.controller.servo(CLAW_SERVO, position);
    msleep(250);
}

static void set_arm_to_position(int position) {
    scorpion.controller.servo(ARM_SERVO, position);
    msleep(250);
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

void _close_claw_slow() {
    scorpion.close_claw_slow(1.3);
}

void _raise_arm_slow() {
    scorpion.close_claw_slow(1.0);
}

static void grab_tribbles() {
    scorpion.controller.slow_servo(CLAW_SERVO, CLAW_PARTIAL, 0.6);
    scorpion.grab_tribbles_async();
}

static void grab_tribbles_async() {
    thread tid = thread_create(_close_claw_slow);
    thread_start(tid);
    scorpion.create.forward(12, 100);
    thread_wait(tid);
    thread_destroy(tid);
    scorpion.set_claw_to_position(CLAW_PARTIAL);
    scorpion.close_claw_slow(0.6);
}

static void grab_tribbles_slow() {
    scorpion.controller.slow_servo(CLAW_SERVO, CLAW_PARTIAL, 0.4);
    scorpion.set_claw_to_position(CLAW_OPEN - 300);
    scorpion.create.forward(8, 100);
    scorpion.controller.slow_servo(CLAW_SERVO, CLAW_PARTIAL, 0.4);
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
    scorpion.controller.slow_servo(CLAW_SERVO, CLAW_PARTIAL, 0.4);
    scorpion.create.forward(8, 150);
    scorpion.close_claw();

    int i;
    for(i = 0; i < 6; i++) {
        scorpion.controller.slow_servo(CLAW_SERVO, CLAW_PARTIAL, 0.2);
        scorpion.close_claw();
    }

    scorpion.set_claw_to_position(400);
    scorpion.create.backward(6, 150);
    scorpion.create.forward(6, 150);
    scorpion.close_claw();
    scorpion.controller.slow_servo(CLAW_SERVO, CLAW_PARTIAL, 0.2);
    scorpion.create.backward(5, 120);
    scorpion.close_claw();
    scorpion.controller.slow_servo(CLAW_SERVO, CLAW_PARTIAL, 0.2);
    scorpion.close_claw();
    scorpion.controller.slow_servo(CLAW_SERVO, CLAW_PARTIAL, 0.2);
    scorpion.create.backward(7, 120);
    scorpion.close_claw();
    scorpion.create.forward(4, 120);
    scorpion.create.backward(4, 120);
}

static int get_tophat_value() {
    return analog10(scorpion.tophat_port);
}

static int is_tophat_on_line() {
    return get_tophat_value() > 680 ? 1 : 0;
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

Scorpion new_scorpion(int tophat) {
	Scorpion instance = {
        // Assign instance properties
        .tophat_port = tophat,
        .get_tophat_value = &get_tophat_value,
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
        .left_until_white = &left_until_white
    };
    instance.create = new_create();
    instance.controller = new_create_controller();
    instance.camera = new_camera();
    return instance;
}
