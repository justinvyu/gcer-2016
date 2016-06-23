#include <kipr/botball.h>

#include "Scorpion.h"

enum GameType {
    SEEDING = 0,
    DOUBLE_ELIM = 1
};


long _start_time;
void start()
{
	_start_time = systime();
}
float curr_time()
{
    return (systime()-_start_time)/1000.0;
}
void now()
{
    printf("now %f\n",curr_time());
}
void nowstr(char *s)
{
	printf("%s %f\n",s,curr_time());
}


void wait_till(float t)
{
	now();
    msleep((long)((t-curr_time())*1000));
}

// RANDOM THREAD STUFF

void third_tribble_pile_thread() {
    scorpion.raise_arm();
    msleep(200);
    scorpion.controller.slow_servo(CLAW_SERVO, CLAW_PARTIAL, 0.5);
}

// ----------- SEEDING -----------


void seeding() {

    // vars

    thread tid;
    enum Channel color;

    // beginning

    start();

    msleep(1000);

    scorpion.create.forward(110, 400);
    scorpion.create.right(83, 0, 350);

    scorpion.create.backward(20, 250);
    scorpion.create.forward(7, 300);

    scorpion.create.left(60, 0, 300);
    scorpion.left_until_black(100);

    // scorpion.open_claw();
    // scorpion.lower_arm();
    scorpion.lower_arm();
    scorpion.set_claw_to_position(CLAW_PARTIAL + 100);

    // Get the first tribble pile

    scorpion.create.forward(20, 350);
    scorpion.grab_tribbles();
    scorpion.set_arm_to_position(ARM_PARTIAL);
    scorpion.create.forward(21, 300);

    scorpion.create.right(90, 0, 300);
    scorpion.create.backward(15, 220);
    scorpion.create.forward(5, 280);
    scorpion.create.right(50, 0, 300);
    scorpion.right_until_black(100);
    scorpion.right_until_white(50);

    scorpion.controller.servo(BACK_SERVO, BACK_DOWN);

    now();

    // pick up basket

    scorpion.create.backward_with_speed(24, 220, 204);
    scorpion.create.backward(10, 200);
    scorpion.create.forward(15, 180);
    msleep(100);
    scorpion.create.backward(20, 180);

    scorpion.create.right(18, 0, 180);
    scorpion.create.left(6, 0, 180);
    scorpion.create.forward(15, 200);
    scorpion.create.backward(17, 180);
    scorpion.create.forward(5, 150);
    scorpion.create.backward(5, 150);

    scorpion.lift_basket_slow();

    scorpion.create.backward(25, 200);

    now();

    // Get second tribble pile

    scorpion.create.forward(20, 300);
    scorpion.create.left(45, 0, 300);

    scorpion.raise_arm_slow(0.6);
    scorpion.controller.slow_servo(CLAW_SERVO, CLAW_OPEN - 200, 0.5);
    // scorpion.set_arm_to_position(ARM_DOWN - 200);

    scorpion.lower_arm__open_claw();
    // scorpion.create.forward(22, 250);
    scorpion.grab_tribbles();

    scorpion.create.right(57, 27, 300);

    scorpion.raise_arm();
    msleep(200);

    scorpion.controller.slow_servo(CLAW_SERVO, CLAW_OPEN - 200, 0.5);

    now();

    // Get third tribble pile

    scorpion.set_claw_to_position(CLAW_OPEN - 200);
    scorpion.set_arm_to_position(ARM_DOWN - 110);
    scorpion.create.forward_with_speed(22, 300, 278);
    scorpion.set_arm_to_position(ARM_DOWN);
    scorpion.grab_tribbles();

    now();

    // squaring robot

    tid = thread_create(third_tribble_pile_thread);
    thread_start(tid);
        scorpion.create.left(75, 0, 300);
        scorpion.create.backward(40, 280);
    thread_wait(tid);
    thread_destroy(tid);

    scorpion.create.forward(10, 200);
    scorpion.create.right(60, 0, 300);
    scorpion.right_until_black(100);

    scorpion.create.backward(80, 400);
    scorpion.create.backward(25, 280);
    scorpion.create.forward(5, 300);
    scorpion.create.left(55, 0, 300);
    scorpion.left_until_black(100);
    scorpion.left_until_white(50);

    now();

    // grabbing 4th pile

    scorpion.lower_arm__open_claw();
    scorpion.create.forward(12, 250);
    scorpion.grab_tribbles_slow();

    now();

    // move out from under the ridge to score tribbles

    scorpion.set_arm_to_position(ARM_PARTIAL + 600);
    scorpion.create.forward(30, 300);
    scorpion.forward_until_black(150);
    scorpion.create.forward(14, 300);

    scorpion.raise_arm_slow(0.6);
    scorpion.controller.slow_servo(CLAW_SERVO, CLAW_OPEN - 200, 0.5);

    now();

    // squaring in the center of the field

    scorpion.create.right(88, 0, 280);
    scorpion.create.backward(20, 250);

    scorpion.set_arm_to_position(1750); // not parallel to tribbles
    scorpion.set_claw_to_position(CLAW_PARTIAL - 180);

    now();

    // move block (red/green) out of the way

    scorpion.create.forward(15, 300);
    scorpion.set_claw_to_position(CLAW_CLOSED / 2);
    scorpion.set_arm_to_position(ARM_PARTIAL);
    scorpion.create.left(90, 0, 300);
    scorpion.set_arm_to_position(ARM_DOWN - 300);
    scorpion.set_claw_to_position(CLAW_OPEN - 200);
    scorpion.raise_arm();
    scorpion.create.right(60, 0, 300);
    scorpion.right_until_black(100);

    now();

    // grabbing 5th pile

    scorpion.create.forward(20, 300);
    scorpion.lower_arm__open_claw();
    scorpion.grab_tribbles();
    scorpion.raise_arm();
    scorpion.controller.slow_servo(CLAW_SERVO, CLAW_OPEN, 0.5);

    now();

    // get botguy out of way

    scorpion.move_botguy();
    now();

    // grabbing 6th pile

    scorpion.create.forward(20, 300);
    // scorpion.lower_arm__open_claw();
    scorpion.create.forward(5, 300);
    scorpion.grab_tribbles();
    scorpion.raise_arm();
    msleep(300);
    scorpion.controller.slow_servo(CLAW_SERVO, CLAW_OPEN, 0.5);
    now();

    // square on other side

    scorpion.create.forward(30, 300);
    scorpion.create.right(83, 0, 300);
    scorpion.create.backward(15, 300);
    scorpion.create.backward(10, 250);
    scorpion.create.forward(10, 300);
    scorpion.create.right(60, 0, 300);
    scorpion.right_until_black(100);
    scorpion.right_until_white(100);

    tid = thread_create(scorpion.close_claw);
    thread_start(tid);
        scorpion.create.backward(45, 350);
        scorpion.create.backward(30, 200);
    thread_wait(tid);
    thread_destroy(tid);

    now();

    // check for case #1

    scorpion.create.forward(6, 300);
    scorpion.create.right(83, 0, 300);
    scorpion.create.backward(25, 250);
    scorpion.create.forward(28, 300);
    //scorpion.create.left(80, 45, 300);

    now();

    color = scorpion.get_tribble_color();
    if(color == GREEN_CHANNEL) {
        scorpion.score(PILE_ONE);
        now();
        return;
    }

    // check for case #2

    scorpion.create.forward(40, 300);
    scorpion.create.left(93, 12, 300);
    scorpion.create.backward(10, 300);

    now();

    color = scorpion.get_tribble_color();
    scorpion.create.forward(10, 300);

    if(color == GREEN_CHANNEL) {
        scorpion.score(PILE_TWO);
        now();
        return;
    }

    scorpion.create.forward(40, 300);
    scorpion.score(PILE_THREE);
    now();

}

int main() {
    scorpion = new_scorpion(0, 0);
    scorpion.init();

    enum GameType game_type = SEEDING;

    // scorpion.controller.light_start(0);
    scorpion.controller.shut_down_in(119);

    if(game_type == SEEDING) seeding();

    scorpion.deinit();
  	return 0;
}
