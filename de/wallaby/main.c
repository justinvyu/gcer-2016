#include <kipr/botball.h>
#include "Controller.h"
//john kim sucks
enum ServoPositions {
    BACK_ARM = 0,
    BACK_ARM_UP = 2000,	//1920
    BACK_ARM_DOWN = 0,	//1254
    CAR_ARM = 2,
    CAR_RELEASE = 0,
    CAR_HOLDON = 1100,
    CUBE_ARM = 0,
    CUBE_ARM_DOWN = 550,
    CUBE_ARM_UP = 2407,
    CUBE_ARM_MIDDLE = 1100,
    CUBE_GRIPPER = 1,
    CUBE_GRIPPER_CLOSED = 1960,
    CUBE_GRIPPER_OPEN = 0
};

enum RunType {
    ACTUAL = 0,
    TEST = 1
};


/*pressed = 1
not pressed = 0*/

void actual() {
    controller = new_controller(0, 1, 14.5, 5.0);

    controller.servo(CAR_ARM, CAR_HOLDON);
    controller.servo(CUBE_ARM, CUBE_ARM_UP);
    controller.servo(CUBE_GRIPPER, CUBE_GRIPPER_OPEN);
    controller.enable_servos();

    controller.forward(60, 75);
  	controller.left(15, 0, 100);
    controller.forward(20, 75);
 	msleep(100);
 	controller.forward(55, 100);
    controller.left(5, 0, 30);

    controller.servo(CAR_ARM, CAR_RELEASE);
    msleep(1000);
    controller.backward(10, 100);
    // controller.right(130, 0, -100);
    controller.right(152, 0, -100);
    // controller.backward(10, 75);
    motor(1, -75);	//square up on wall
    motor(0, -75);
    msleep(1700);
    off(1);
    off(0);
    msleep(300);
    controller.forward(40, 100);
    controller.right(5, 0, -100);	//turn because robot veers to the right while driving
    controller.forward(50, 100);
    msleep(300);
    //controller.left(152, 0, 100);
    controller.left(140, 0, 100);
    msleep(500);
    controller.backward(40, 100);
    motor(1, -75);
    motor(0, -75);
    msleep(1000);
    off(1);
    off(0);
    // controller.backward(40, 100);
    // controller.backward(7, 50);
    controller.slow_servo(CUBE_GRIPPER, CUBE_GRIPPER_OPEN, 0.5);
    controller.servo(CUBE_ARM, CUBE_ARM_DOWN);
    msleep(1000);
    // controller.backward(7, 25);
    controller.slow_servo(CUBE_GRIPPER, CUBE_GRIPPER_CLOSED, 0.5);
    msleep(800);
    controller.servo(CUBE_ARM, CUBE_ARM_UP);
    msleep(1000);

    //Got the cube

    controller.forward(10, 100);
    controller.left(260, 0, 100);
    controller.backward(13, 100);
    controller.slow_servo(CUBE_ARM, CUBE_ARM_DOWN, 1);
    msleep(1000);
    controller.servo(CUBE_GRIPPER, CUBE_GRIPPER_OPEN);
    msleep(500);
    controller.servo(CUBE_ARM, CUBE_ARM_UP);
    msleep(500);

    // Put the cube down

  	controller.forward(13, 100);
    controller.left(225, 0, -100);
  	controller.backward(10, 100);
  	motor(1, -75);
  	motor(0, -75);
  	msleep(1000);
  	off(1);
  	off(0);
  	msleep(500);
  	controller.servo(CUBE_ARM, CUBE_ARM_DOWN);
  	msleep(1000);
  	controller.servo(CUBE_GRIPPER, CUBE_GRIPPER_OPEN);
  	msleep(500);
  	controller.forward(60, 100);
  	controller.servo(CUBE_ARM, CUBE_ARM_UP);
	msleep(100);
  	controller.servo(CUBE_GRIPPER, CUBE_GRIPPER_OPEN);
  	controller.forward(20, 100);
  	controller.right(135, 0, 100);
  	controller.backward(70, 100);

    //controller.left(90, 50, -100);
    //controller.backward(8, 70);
    //controller.slow_servo(CUBE_ARM, 150, 0.6);
    //controller.servo(CUBE_GRIPPER, CUBE_GRIPPER_OPEN);
    //msleep(150);

    //controller.forward(45, 100);
  	//controller.servo(CUBE_GRIPPER, CUBE_GRIPPER_OPEN);
    //msleep(150);
    //controller.left(105, 0, -100);
  	//controller.servo(CUBE_GRIPPER, CUBE_GRIPPER_OPEN);
    //msleep(150);
    //controller.backward(10, 100);
    //controller.slow_servo(CUBE_ARM, CUBE_ARM_UP, 0.4);
    //controller.right(105, 10, 100);


    /*** =============================		DO NOT RUN THIS CODE DURING SCRIMMAGE 6/28/16		=============================== ***/

    // controller.forward(5, 100);
    // controller.left(210, 0, -100);
    // controller.backward(8, 100);
    // motor(1, -75);
    // motor(0, -75);
    // msleep(1500);
    // off(1);
    // off(0);
    //
    // //back to pipe and sqaured up
    //
    // controller.servo(CUBE_GRIPPER, CUBE_GRIPPER_OPEN);
    // msleep(300);
    // controller.servo(CUBE_ARM, CUBE_ARM_DOWN);
    // msleep(300);
    // controller.right(25, 90, 100);
    //
    //
    // controller.forward(25, 100);
    // controller.left(10, 0, -100);
    // controller.forward(20, 100);
    //
    //
    // controller.servo(CUBE_GRIPPER, CUBE_GRIPPER_CLOSED);
    // controller.servo(CUBE_GRIPPER, CUBE_GRIPPER_OPEN);	//BTW THIS CODE DOESNT ACTUALLY DO ANYTHING JOHN ASK JUSTIN ABOUT IT
    // // pulled pipe away
    //
    // controller.right(140, 0, -100);
    // controller.backward(50, 100);
    // msleep(100);
    // controller.servo(CUBE_ARM, CUBE_ARM_UP);
    // msleep(500);
    // controller.forward(51, 100);
    // controller.right(140, 0, 100);
    // controller.backward(95, 100);
    // msleep(500);
    // controller.left(142, 0, -100);
    // controller.backward(110, 100);
    // controller.forward(18, 100);
    // controller.left(145, 0, 100);
    // controller.backward(210, 100);
    // motor(1, -50);
    // motor(0, -50);
    // msleep(1000);
    // off(1);
    // off(0);
    // controller.forward(3, 100);
    // controller.right(140, 0, 100);
    // controller.forward(400, 100);

    /*** =====================================			OLD CODE		================================================ ***/

    // controller.left(130, 0, 100);
    // controller.servo(CUBE_ARM, CUBE_ARM_DOWN);
    // msleep(1000);
    // over ramp and pipe, car released

    // controller.servo(CUBE_ARM, CUBE_ARM_DOWN);
    // controller.servo(CUBE_GRIPPER, CUBE_GRIPPER_OPEN);
    // controller.backward(60, 100);
    // controller.servo(CUBE_GRIPPER, CUBE_GRIPPER_CLOSED);
    //
    //
    // controller.servo(CUBE_ARM, CUBE_ARM_UP);
    // controller.servo(CAR_ARM, CAR_HOLDON);
    // controller.servo(BACK_ARM, BACK_ARM_UP);
    // controller.forward(-70,90);
    // controller.servo(BACK_ARM, BACK_ARM_DOWN);
    // controller.forward(-30, 90);
    // controller.servo(BACK_ARM, BACK_ARM_UP);
    // //over first pipe
    //
    // controller.forward(-45,90);
    // controller.servo(CAR_ARM, CAR_RELEASE);
    // controller.forward(-40,90);
    // //mav(1,900);
    // //msleep(500);
    // //arrived at second pipe
    // controller.servo(BACK_ARM, BACK_ARM_DOWN);
    // //mav(0,900);
    // //msleep(500);
    // controller.forward(-20,90);
    // controller.servo(BACK_ARM, BACK_ARM_UP);
    // controller.forward(-80,90);

}

void test() {
    controller = new_controller(0, 1, 14.5, 5.0);
    controller.enable_servos();
    controller.servo(CUBE_GRIPPER, CUBE_GRIPPER_OPEN);
    msleep(300);
    controller.servo(CUBE_ARM, CUBE_ARM_DOWN);
    msleep(300);
    controller.forward(20, 100);
    controller.left(25, 100, 100);
    /*
    controller.servo(CUBE_ARM, CUBE_ARM_UP - 700);
    motor(1, 100);
    motor(0, 100);
    msleep(3000);
    controller.servo(CUBE_ARM, CUBE_ARM_DOWN + 100);
    msleep(300);
    controller.servo(CUBE_ARM, CUBE_ARM_DOWN - 200);
    msleep(3000);
    */
}

int main()
{
    enum RunType type = ACTUAL;

    if(type == ACTUAL) actual();
    else test();

    return 0;
}
