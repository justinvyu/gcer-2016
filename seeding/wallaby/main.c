#include <kipr/botball.h>

#include "Animal.h"
#define tophat_right 5
#define Dis 2
#define THRESHOLD 500

enum Side {
    LEFT = 0,
    RIGHT = 1
};

static void PID(int distance) {
    // initialize variables

    enum Side side;

    int tophat = 5;
    int et = 1, et_threshold = 1700; // greater than 1600 means that the pole is there

    // PID

    float kp = 3.2, // proportionality constant
    ki = 0.1,  // integral constant
    kd = 8.2; // derivative constant
    int low, high;
    float offset = 0; // average of white and black sensor values (660, 985) = 822.5
    int tp = 1100; // speed of motors at error = 0
    float integral = 0; // running total of errors
    float last_error = 0;
    float derivative = 0; // current error - last error

    int sensor_value, turn;
    float error;

    // calibration

    /*
    printf("Place tophat sensor above black line.\n");
    while(1) {
        if(digital(0) == 1) {
            high = analog10(tophat);
            printf("%d\n", high);
            break;
        }
        msleep(10);
    }

    msleep(1000);

    printf("Place tophat sensor above white space.\n");

    while(1) {
        if(digital(0) == 1) {
            low = analog10(tophat);
            printf("%d\n", low);
            break;
        }
        msleep(10);
    }
    */

    low = 730;
    high = 1030;
    offset = (float)(low + high) / 2.;
    msleep(1000);


    // while(digital(0) == 0);

    cmpc(1);
    while(1) {
        sensor_value = analog10(tophat);
        error = sensor_value - offset;
        integral = (7. / 8.) * integral + error;
        derivative = error - last_error;
        turn = (int)(kp * error + ki * integral + kd * derivative);
        mav(1, tp + turn);
        mav(0, tp - turn);
        last_error = error;
        msleep(1);
        printf("%d\n",gmpc(1));
        if(gmpc(1)>distance){
            break;
        }
    }
}

int main()
{
    int i = 0;
    wallaby = new_animal();
    //wallaby.controller.light_start(0);
    wallaby.controller.shut_down_in(119);
    wallaby.controller.enable_servos();

    if(1==1) {
        wallaby.controller.servo(0,1600);
        wallaby.controller.servo(1,800);
        wallaby.controller.servo(3, 900); //servo setup
        wallaby.controller.forward(5,100); //initial square up
        wallaby.controller.left(4,0,-80);
        wallaby.controller.backward(30,100);
        wallaby.controller.right(45,0,-80);

        //wallaby.controller.right(1,0,50);
        wallaby.controller.backward(65,100);//square up on the pipe
        wallaby.controller.servo(0,1000);//grab pipe

        for(i=0; i<5; i++) {
            wallaby.controller.forward(1,50);
            wallaby.controller.left(10,0,50);
        }

        /*
        for(i=0; i<8; i++){
            wallaby.controller.right(2,0,-80);
            wallaby.controller.left(2,0,80);
        }
        */

        wallaby.controller.right(80,2,-100);//rotate pipe
        //wallaby.controller.servo(0,1400);

        // wallaby.controller.backward(15,50);
        wallaby.controller.servo(0,1600);//release pipe
        wallaby.controller.forward(20,100);
        wallaby.controller.left(140,0,80);
        wallaby.controller.forward(20,50);//square up

        //after pushing pipe

        wallaby.controller.servo(1,2047);
        //set servos
        wallaby.controller.left(105,0,-80);

        while(1) { //deteect when the pipe stops
            int j = analog10(Dis);
            printf("%d\n", j);
            if(j<300) {
                printf("%d\n", j);
                break;
            }
            wallaby.controller.backward(1,80);
        }
        //wallaby.controller.servo(1,1000);
        wallaby.controller.forward(2,80);//push pipe
        //wallaby.controller.forward(2,80);
        wallaby.controller.left(75,0,-80);//turn into the vally?
        wallaby.controller.servo(0,1200);
        msleep(300);
        wallaby.controller.servo(3, 1800);
        wallaby.controller.backward(40,50);
        wallaby.controller.servo(1,1000);//grab block
        msleep(300);
        //wallaby.controller.forward(25,50);
        wallaby.controller.servo(0,1700);//lift block
        wallaby.controller.forward(10,100);
        wallaby.controller.right(110,5,-100);
        while(1){//detect pipe
            int j = analog10(Dis);
            printf("%d\n", j);
            if(j<300){
                printf("%d\n", j);
                break;
            }
            wallaby.controller.backward(1,100);
        }
        //wallaby.controller.forward(10,50);
        wallaby.controller.backward(50,100);//square up
        //wallaby.controller.backward(5,80);
        //wallaby.controller.forward(10,50);
        wallaby.controller.servo(1,1800);
        msleep(200);//drop block
        wallaby.controller.left(110,10,100);

        wallaby.controller.backward(50,100);
        wallaby.controller.forward(48,100);//square up

        wallaby.controller.servo(0,1000);
        wallaby.controller.servo(3,900);
        //wallaby.controller.backward(45,100);
        msleep(100);
        //wallaby.controller.servo(1,1800);
        //wallaby.controller.forward(50,100);
        //wallaby.controller.servo(0,1000);
        PID(5240);
        //wallaby.controller.servo(0,500);
        wallaby.controller.right(115,0,80);//face bin


        wallaby.controller.backward(25,80);
        //wallaby.controller.servo(0,300);
        msleep(200);
        wallaby.controller.servo(1,800);//grab bin
        msleep(200);
        wallaby.controller.forward(10,80);
        PID(8400);

        /*wallaby.controller.servo(1, 1200);
        msleep(100);
        for(i=0; i<8; i++){
        wallaby.controller.right(2,0,-80);
        wallaby.controller.left(2,0,80);
        }
        wallaby.controller.servo(1, 1600);
        msleep(200);
        wallaby.controller.backward(5,100);
        wallaby.controller.servo(1, 800);
        msleep(300);*/

        wallaby.controller.servo(1,1150);
        msleep(1000);

        wallaby.controller.right(90,0,-80);
        wallaby.controller.left(100,0,80);
        wallaby.controller.right(20,0,40);
        wallaby.controller.servo(1, 1700);
        wallaby.controller.backward(10,100);
        wallaby.controller.slow_servo(1, 0,3);

        //wallaby.controller.backward(60,80);// final push
        //wallaby.controller.servo(0,2407);//release
        //wallaby.controller.forward(65,80);//go back

        wallaby.controller.slow_servo(0, 1500,0.5);
        //msleep(5000)
        wallaby.controller.forward(10, 80);
        //wallaby.controller.servo(3, 1800);
        wallaby.controller.slow_servo(3, 1800,1);
        msleep(2000);
        wallaby.controller.servo(0, 1500);
        msleep(300);
        for(i=0; i<3; i++){
            wallaby.controller.slow_servo(0, 1800,0.2);
            wallaby.controller.slow_servo(0, 1500,0.2);
        }
        wallaby.controller.slow_servo(0, 1800,0.2);


        for(i=0; i<24; i++){
            wallaby.controller.right(2,0,80);
            wallaby.controller.left(2,0,-80);
        }
        wallaby.controller.servo(0, 1500);
        msleep(200);
        wallaby.controller.servo(3, 900);
        wallaby.controller.backward(40,80);//square up
        //wallaby.controller.servo(1, 1700);
        wallaby.controller.slow_servo(0, 1800,0.2);
        wallaby.controller.servo(0, 2000);
        wallaby.controller.forward(90,80);
        wallaby.controller.left(105,0,-40);
        //wallaby.controller.servo(0, 1300);


        wallaby.controller.forward(30,80);
        /*wallaby.controller.servo(1,1200);
        wallaby.controller.servo(0, 400);*/


        while(1){//detect pipe
            int j = analog10(tophat_right);
            printf("%d\n", j);
            if(j>850){
                printf("%d\n", j);
                break;
            }
            wallaby.controller.forward(1,40);
        }

        wallaby.controller.motor(0,40);
        wallaby.controller.motor(1,40);


        wallaby.controller.slow_servo(0,40,4);


        wallaby.controller.forward(120,80);
        //wallaby.controller.forward(40,40);

        wallaby.controller.servo(3, 1800);
        wallaby.controller.servo(1,2047);
        wallaby.controller.servo(0, 2000);
        msleep(300);

        for(i=0; i<9; i++){
            wallaby.controller.right(2,0,80);
            wallaby.controller.left(2,0,-80);
        }

        wallaby.controller.backward(27,80);
        wallaby.controller.forward(55,30);
        wallaby.controller.backward(5,80);
        for(i=0; i<5; i++){
            wallaby.controller.right(2,0,80);
            wallaby.controller.left(2,0,-80);
        }
        for(i=0; i<10; i++){
            wallaby.controller.right(2,0,-80);
            wallaby.controller.left(2,0,80);
        }
        for(i=0; i<5; i++){
            wallaby.controller.right(2,0,80);
            wallaby.controller.left(2,0,-80);
        }
        msleep(1000);

        return 0;
    } else { /////////////////////////////////////////////////////////////////////////
        wallaby.controller.servo(0,1000);
        wallaby.controller.servo(3,900);
        wallaby.controller.servo(1,1150);
        msleep(1000);

        wallaby.controller.right(50,0,-80);
        wallaby.controller.right(10,0,40);
        wallaby.controller.servo(1, 1700);
        wallaby.controller.backward(10,40);

        //wallaby.controller.backward(60,80);// final push
        //wallaby.controller.servo(0,2407);//release
        //wallaby.controller.forward(65,80);//go back
        wallaby.controller.slow_servo(1, 0,3);
        wallaby.controller.slow_servo(0, 1500,0.5);
        //msleep(5000)
        wallaby.controller.forward(10, 80);
        //wallaby.controller.servo(3, 1800);
        wallaby.controller.slow_servo(3, 1800,1);
        msleep(2000);
        wallaby.controller.servo(0, 1500);
        msleep(300);
        for(i=0; i<5; i++){
            wallaby.controller.servo(0, 1800);
            msleep(300);
            wallaby.controller.servo(0, 1500);
            msleep(300);
        }
        wallaby.controller.servo(1, 1700);
        msleep(50);
        wallaby.controller.servo(1, 800);
        msleep(300);


        for(i=0; i<23; i++){
            wallaby.controller.right(2,0,80);
            wallaby.controller.left(2,0,-80);
        }
        wallaby.controller.servo(3, 900);
        wallaby.controller.backward(40,80);//square up
        wallaby.controller.servo(1, 1700);
        wallaby.controller.servo(0, 2000);
        wallaby.controller.forward(90,80);
        wallaby.controller.left(110,0,-40);
        //wallaby.controller.servo(0, 1300);
        wallaby.controller.forward(50,30);
        wallaby.controller.servo(1,1200);
        wallaby.controller.servo(0, 400);

        wallaby.controller.forward(100,80);
        wallaby.controller.forward(40,40);


        for(i=0; i<9; i++){
            wallaby.controller.right(2,0,80);
            wallaby.controller.left(2,0,-80);
        }

        wallaby.controller.servo(3, 1800);
        wallaby.controller.servo(1,2047);
        wallaby.controller.servo(0, 2000);

        wallaby.controller.backward(27,80);
        wallaby.controller.forward(55,30);
        wallaby.controller.backward(5,80);
        for(i=0; i<5; i++){
            wallaby.controller.right(2,0,80);
            wallaby.controller.left(2,0,-80);
        }
        for(i=0; i<10; i++){
            wallaby.controller.right(2,0,-80);
            wallaby.controller.left(2,0,80);
        }
        for(i=0; i<5; i++){
            wallaby.controller.right(2,0,80);
            wallaby.controller.left(2,0,-80);
        }
        msleep(1000);
    }
    return 0;
}
