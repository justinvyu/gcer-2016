//
//  Controller.c
//

#include "Controller.h"
#include <math.h>

// CUSTOM FOR 2016 WALLABY

#define MOTOR_CORRECTION_CONSTANT -0.5

// -----------------------

static void forward(float dist, int speed);
static void backward(float dist, int speed);
static void left(int angle, float radius, int speed);
static void right(int angle, float radius, int speed);

/**
 * Converts from centimeters to BackEMF ticks (robot measurement).
 * @param  cm a distance in centimeters that you want to convert
 * @return    the parameter `cm` in BackEMF ticks
 */
long CMtoBEMF(float cm) {
 	return (long)(cm * 1150. / (M_PI * controller.wheel_diameter));
}

/**
 * Converts from BackEMF ticks to centimeters.
 * @param  ticks a distance in BackEMF ticks that you want to convert
 * @return       the parameter `ticks` in centimeters
 */
float BEMFtoCM(long ticks) {
 	return (float)(ticks * (M_PI * controller.wheel_diameter) / 1100.);
}

static void forward(float dist, int speed) {
    if(dist < 0.) {
    	backward(-dist, speed);
      	return;
    }

  	// Calculate the # of ticks the robot must move for each wheel
	long ticks = CMtoBEMF(dist);
	long totalLeftTicks = gmpc(controller.motor_left) + ticks;
	long totalRightTicks = gmpc(controller.motor_right) + ticks;

  	// Start motors
	motor(controller.motor_left, speed - MOTOR_CORRECTION_CONSTANT);
	motor(controller.motor_right, speed + MOTOR_CORRECTION_CONSTANT);

  	// Keep moving until both motors reach their desired # of ticks
	while(gmpc(controller.motor_left) < totalLeftTicks
          && gmpc(controller.motor_right) < totalRightTicks) {
		if (gmpc(controller.motor_left) >= totalLeftTicks)
			off(controller.motor_left);
		if (gmpc(controller.motor_right) >= totalRightTicks)
			off(controller.motor_right);
	}

	off(controller.motor_left);
  	off(controller.motor_right);
}

static void backward(float dist, int speed) {
    if(dist < 0.) {
        forward(-dist, speed);
        return;
    }

  	// Calculate the # of ticks the robot must move for each wheel
	long ticks = CMtoBEMF(dist);
	long totalLeftTicks = gmpc(controller.motor_left) - ticks;
	long totalRightTicks = gmpc(controller.motor_right) - ticks;

  	// Start motors
	motor(controller.motor_left, -speed + MOTOR_CORRECTION_CONSTANT);
	motor(controller.motor_right, -speed - MOTOR_CORRECTION_CONSTANT);

  	// Keep moving until both motors reach their desired # of ticks
	while(gmpc(controller.motor_left) > totalLeftTicks
          && gmpc(controller.motor_right) > totalRightTicks) {
		if (gmpc(controller.motor_left) <= totalLeftTicks)
			off(controller.motor_left);
		if (gmpc(controller.motor_right) <= totalRightTicks)
			off(controller.motor_right);
	}
    off(controller.motor_left);
    off(controller.motor_right);
}

static void left(int angle, float radius, int speed) {
    // calculate radii
    float left_radius = radius;
    float right_radius = radius + controller.distance_between_wheels;

    if(left_radius < 0)
        return;
    if(right_radius <= 0)
        return;

    // calculate distance in CM
    float right_distance = (right_radius * M_PI) * ((float)(angle) / 180.);

    // calculate relative velocity

    int left_speed = left_radius * (float)speed / right_radius;
    int right_speed = speed;

    long right_distance_ticks = CMtoBEMF(right_distance);

    // clear motor tick counter
    controller.cmpc(controller.motor_left);
    controller.cmpc(controller.motor_right);

    // power motors

    controller.motor(controller.motor_left, left_speed);
    controller.motor(controller.motor_right, right_speed);

    while(abs(controller.gmpc(controller.motor_right)) <= abs(right_distance_ticks)) {
        msleep(50);
    }

    off(controller.motor_left);
    off(controller.motor_right);
}


static void right(int angle, float radius, int speed) {
	// calculate radii
    float left_radius = radius + controller.distance_between_wheels;
    float right_radius = radius;

    if(left_radius <= 0)
        return;
    if(right_radius < 0)
        return;

    // calculate distance in CM
    float left_distance = (left_radius * M_PI) * ((float)(angle) / 180.);

    // calculate relative velocity

    int left_speed = speed;
    int right_speed = right_radius * (float)speed / left_radius;

    long left_distance_ticks = CMtoBEMF(left_distance);

    // clear motor tick counter
    controller.cmpc(controller.motor_left);
    controller.cmpc(controller.motor_right);

    // power motors

    controller.motor(controller.motor_left, left_speed);
    controller.motor(controller.motor_right, right_speed);

    while(abs(controller.gmpc(controller.motor_left)) <= abs(left_distance_ticks)) {
        msleep(50);
    }

    off(controller.motor_left);
    off(controller.motor_right);
}

// FIX THIS
static void slow_servo(int port, int position, float time) {
    float increment = .01;
	float curr, start = controller.get_servo_position(port);
	float i = ((position - start) / time) * increment;
	curr = start;
	if (start > position)
	{
		while(curr > position)
		{
			controller.servo(port, curr);
			curr += i;
			msleep((long)(increment * 1000));
		}
	}
	else if (start < position)
	{
		while(curr < position)
		{
			controller.servo(port, curr);
			curr += i;
			msleep((long)(increment * 1000));
		}
	}
	controller.servo(port, position);
}

// Setters

static int set_motor_left(int port) {
    if(port < 0 || port > 3) {
        return 0;
    }
    controller.motor_left = port;
    return 1;
}

static int set_motor_right(int port) {
    if(port < 0 || port > 3) {
        return 0;
    }
    controller.motor_right = port;
    return 1;
}

// Constructors

Controller new_controller(int motor_left, int motor_right, float distance_between_wheels, float wheel_diameter) {
	Controller instance = {

        // Instance Variables (with no setters)
        .motor_left = motor_left,
        .motor_right = motor_right,
        .distance_between_wheels = distance_between_wheels,
        .wheel_diameter = wheel_diameter,

        // Setters

        .set_motor_left = &set_motor_left,
        .set_motor_right = &set_motor_right,

        // Assign method references
        .forward = &forward, .backward = &backward,
        .left = &left, .right = &right,
        .motor = &motor,
        .mav = &mav, .mtp = &mtp, .mrp = &mrp,
        .stop = &ao,
        .motor_off = &off,
        .gmpc = &gmpc,
        .clear_motor_position_counter = &cmpc,
        .gmpc = &gmpc, .cmpc = &cmpc,
        .enable_servo = &enable_servo,
        .disable_servo = &disable_servo,
        .enable_servos = &enable_servos,
        .disable_servos = &disable_servos,
        .get_servo_position = &get_servo_position,
        .servo = &set_servo_position,
        .slow_servo = &slow_servo,
        .digital = &digital,
        .analog = &analog,
        .analog10 = &analog10,
        .analog_et = &analog_et,
        .light_start = &wait_for_light,
        .shut_down_in = &shut_down_in
    };

    controller = instance;

    return instance;
}

Controller new_create_controller() {
	Controller instance = {
        // Assign method references
        .motor = &motor,
        .mav = &mav, .mtp = &mtp, .mrp = &mrp,
        .stop = &ao,
        .motor_off = &off,
        .gmpc = &gmpc,
        .clear_motor_position_counter = &cmpc,
        .gmpc = &gmpc, .cmpc = &cmpc,
        .enable_servo = &enable_servo,
        .disable_servo = &disable_servo,
        .enable_servos = &enable_servos,
        .disable_servos = &disable_servos,
        .get_servo_position = &get_servo_position,
        .servo = &set_servo_position,
        .slow_servo = &slow_servo,
        .digital = &digital,
        .analog = &analog,
        .analog10 = &analog10,
        .analog_et = &analog_et,
        .light_start = &wait_for_light,
        .shut_down_in = &shut_down_in
    };
    controller = instance;
    return instance;
}
