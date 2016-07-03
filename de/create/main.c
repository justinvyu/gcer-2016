#include <kipr/botball.h>
#include "Geminiq.h"

int main()
{
    gemini = new_gemini(5, 4);
    gemini.init();

    // First Part: go forward, go to middle

    gemini.create.forward(5, 55);
    gemini.create.left(68, 0, 55);
    msleep(1000);	// wait for light here
    gemini.create.forward(7, 380);
    gemini.create.right(65, 69, 380);
    gemini.create.forward_until_bump(100);
  	//gemini.create.backward(3, 20);
  	//gemini.create.left(2, 0, 40);
    // gemini.create.right(60, 37, 360);
    msleep(1000);

    // (float dist, int lspeed, int rspeed)
    // little link goes off ramp

    /*
    while(analog10(5) < 825) {
        gemini.controller.forward_with_speed(1, 150, 150);
        printf("%d\n",analog10(5));
    }
    */

    gemini.controller.forward_with_speed(65, 100, 100);
    gemini.controller.servo(ARM_SERVO, ARM_DOWN);
    msleep(200);

    gemini.controller.forward_with_speed(20, 100, 70);

    // gemini.controller.right(5, 0, 80);
    gemini.line_follow_until_et(1100);

    gemini.controller.forward(25, 50);
    gemini.controller.backward(3, 70);

    int i;
    for(i = 0; i < 5; i++) {
        gemini.controller.right(2, 0, -80);
        gemini.controller.left(2, 0, 80);
    }
    gemini.controller.right(6, 0, 80);

    for(i = 0; i < 6; i++) {
        gemini.line_follow(320, 750);
        gemini.controller.backward(6, 40);
    }
  	gemini.controller.right(10, 0, 20);
  	gemini.controller.left(10, 0, 20);
    create.disconnect();
    ao();

    return 0;
}
