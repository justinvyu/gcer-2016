#include <kipr/botball.h>

#include "Animal.h"
#define tophat_right 5
#define Dis 2
#define THRESHOLD 500

enum Side {
    LEFT = 0,
    RIGHT = 1
};

static void PID(int distance){
  
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
    int tp = 1000; // speed of motors at error = 0
    float integral = 0; // running total of errors
    float last_error = 0;
    float derivative = 0; // current error - last error
  
    int sensor_value, turn;
    float error;
  
    // calibration
    
    /*printf("Place tophat sensor above black line.\n");
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
    }*/
  
  	low = 730;
  	high = 1030;
    offset = (float)(low + high) / 2.;
    msleep(1000);
    
    printf("Place tophat sensor above white space.\n");
  
    //while(digital(0) == 0);
  
  	cmpc(1);
    while(1) {
        sensor_value = analog10(tophat);
        error = sensor_value - offset;
        integral = (7. / 8.) * integral + error;
        derivative = error - last_error;
        turn = (int)(kp * error + ki * integral + kd * derivative);
        mav(1, tp - turn);
        mav(0, tp + turn);
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
    
   
  if(1==1){ 
    wallaby.controller.servo(0,1300);
    wallaby.controller.forward(5,80);
	wallaby.controller.backward(50,80);
    //wallaby.controller.right(1,0,50);
    wallaby.controller.backward(50,80);
    wallaby.controller.servo(0,100);
    for(i=0; i<5; i++){
   	 	wallaby.controller.forward(1,50);
    	wallaby.controller.left(10,0,50);
  	}
    for(i=0; i<9; i++){
    	wallaby.controller.right(2,0,-80);
    	wallaby.controller.left(2,0,80);
  	}
    wallaby.controller.servo(0,200);
  	wallaby.controller.backward(15,50);
    wallaby.controller.servo(0,1600);
    wallaby.controller.forward(25,50);
  	wallaby.controller.left(120,0,80);
    wallaby.controller.forward(15,50);
    //after pushing pipe
    
    
    wallaby.controller.right(105,5,-80);
    
    while(1){
      int j = analog10(Dis);
      printf("%d\n", j);
      if(j<300){
        printf("%d\n", j);
        break;
      }
      wallaby.controller.forward(1,80);
    }
    
    //wallaby.controller.forward(25,50);
    wallaby.controller.right(105,5,80);
    wallaby.controller.forward(15,50);
    //wallaby.controller.servo(0,1000);
    //wallaby.controller.forward(25,50);
    wallaby.controller.left(110,5,80);
    wallaby.controller.forward(80,50);
    //wallaby.controller.backward(5,80);
    wallaby.controller.left(105,0,-80);
    wallaby.controller.backward(15,80);
    wallaby.controller.forward(50,50);
    PID(5000);
    wallaby.controller.right(140,0,80);
    wallaby.controller.backward(25,80);
    wallaby.controller.servo(0,200);
    msleep(200);
    wallaby.controller.left(55,0,80);
    PID(9900);
	
    for(i=0; i<9; i++){
    	wallaby.controller.right(2,0,-80);
    	wallaby.controller.left(2,0,80);
  	}
    wallaby.controller.forward(20,80);
    wallaby.controller.right(115,0,-80);
    
    wallaby.controller.backward(60,80);
    wallaby.controller.servo(0,2407);
    wallaby.controller.forward(65,80);
    wallaby.controller.left(135,0,80);
    wallaby.controller.backward(40,80);
    wallaby.controller.forward(90,80);
    wallaby.controller.left(95,0,-80);
    wallaby.controller.forward(150,80);
    wallaby.controller.forward(40,40);
    
    
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
  }else{
    wallaby.controller.enable_servos();
    wallaby.controller.servo(0,0);
    wallaby.controller.servo(3,0);
    msleep(300);
    wallaby.controller.backward(10,40);
    wallaby.controller.servo(3,1800);
    msleep(500);
    //wallaby.controller.servo(0,1000);
    msleep(3000);
  }
  
  /*
  wallaby.controller.servo(0,1300);
  wallaby.controller.backward(5,80); // square up to staring pos
  
  wallaby.controller.right(30,20,80);
  wallaby.controller.forward(40,80);
  wallaby.controller.left(40,20,80);
  wallaby.controller.forward(30,80);
  wallaby.controller.backward(3,80);
  
  for(i=0; i<9; i++){
    wallaby.controller.right(2,0,80);
    wallaby.controller.left(2,0,-80);
  }
  msleep(200);
  wallaby.controller.backward(29,80);
  for(i=0; i<9; i++){
    wallaby.controller.right(2,0,80);
    wallaby.controller.left(2,0,-80);
  }
  wallaby.controller.backward(10,80);
  wallaby.controller.servo(0,0);
  msleep(200);
  for(i=0; i<5; i++){
   	 wallaby.controller.forward(1,50);
     wallaby.controller.left(10,0,50);
  }
  wallaby.controller.left(60,0,50);
  wallaby.controller.backward(15,50);
  wallaby.controller.left(120,0,80);
  */
  /*
  wallaby.controller.backward(33,80);
  
  for(i=0; i<5; i++){
    wallaby.controller.right(2,0,-80);
    wallaby.controller.left(2,0,80);
  }
  //wallaby.controller.servo(0,1300);
  for(i=0; i<4; i++){
    wallaby.controller.right(2,0,80);
    wallaby.controller.left(2,0,-80);
  }
  
  wallaby.controller.forward(36,80);
  wallaby.controller.right(105,5,80);
  wallaby.controller.forward(50,80);
  
  
  
  wallaby.controller.left(95,1,-80); // fancy turn
  msleep(200);
  wallaby.controller.backward(170,80); // up the ramp
  msleep(2000);
  //wallaby.controller.right(2,0,80);
  //wallaby.controller.forward(,40);
  for(i=0; i<9; i++){
    wallaby.controller.right(2,0,-80);
    wallaby.controller.left(2,0,80);
  }
  wallaby.controller.forward(27,80);
  wallaby.controller.backward(55,30);
  wallaby.controller.forward(5,80);
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
  wallaby.controller.slow_servo(0,100,0.8);
  msleep(1000);
  //line_follow_R();
  //motor(wallaby.controller.motor_left,90);
  //motor(wallaby.controller.motor_right,100);
  //wallaby.controller.forward(40, 80);*/
  return 0;
}
