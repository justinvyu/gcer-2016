#include <kipr/botball.h>

#include "Scorpion.h"

enum GameType {
    SEEDING = 0,
    DOUBLE_ELIM = 1
};


// ----------- SEEDING -----------


void seeding() {
    scorpion.create.backward(15, 120);

    // msleep(4500); // wait for wallaby

    scorpion.create.forward(105, 350);
    scorpion.create.right(90, 0, 250);

    scorpion.create.backward(20, 200);
    scorpion.create.forward(8, 300);
    scorpion.create.left(60, 0, 300);
    scorpion.left_until_black(120);

    scorpion.open_claw();
    scorpion.lower_arm();

    // Get the first tribble pile

    scorpion.create.forward(30, 280);
    scorpion.grab_tribbles_slow();
    scorpion.set_arm_to_position(ARM_PARTIAL);

    scorpion.create.right(90, 0, 300);
    scorpion.create.backward(18, 250);
    scorpion.create.forward(4, 250);
    scorpion.create.right(50, 0, 300);
    scorpion.right_until_black(100);
    scorpion.right_until_white(50);

    scorpion.controller.servo(BACK_SERVO, BACK_DOWN);
    msleep(250);

    // pick up basket

    scorpion.create.backward_with_speed(10, 220, 208);
    scorpion.create.backward(25, 200);
    scorpion.create.forward(10, 150);
    msleep(250);
    scorpion.create.backward(10, 120);

    scorpion.create.right(16, 0, 100);
    scorpion.create.left(6, 0, 50);
    scorpion.create.forward(20, 150);
    scorpion.create.backward(25, 120);

    scorpion.lift_basket_slow();

    scorpion.create.backward(28, 200);

    // Get second tribble pile

    scorpion.create.forward(15, 250);
    scorpion.create.left(8, 0, 200);
    scorpion.create.forward(32, 300);

    scorpion.raise_arm();
    scorpion.set_claw_to_position(CLAW_OPEN - 200);
    scorpion.set_arm_to_position(ARM_DOWN - 180);

    scorpion.create.forward(22, 150);
    scorpion.grab_tribbles();

    scorpion.raise_arm_slow(0.6);
    msleep(300);
    scorpion.set_claw_to_position(CLAW_PARTIAL);

    scorpion.create.backward(5, 150);

    // Get third tribble pile

    scorpion.create.left(155, 0, 250);
    scorpion.create.backward(15, 200);

    scorpion.lower_arm();
    scorpion.open_claw();

    scorpion.create.forward(20, 250);
    scorpion.grab_tribbles();

    scorpion.create.backward(10, 300);

    scorpion.raise_arm_slow(0.6);
    msleep(300);
    scorpion.set_claw_to_position(CLAW_PARTIAL);

    // squaring robot

    scorpion.create.right(65, 0, 300);
    scorpion.create.backward(40, 280);
    scorpion.create.forward(10, 200);
    scorpion.create.right(60, 0, 300);
    scorpion.right_until_black(100);

    scorpion.create.backward(60, 300);
    scorpion.create.backward(15, 200);
    scorpion.create.forward(6, 300);
    scorpion.create.left(55, 0, 300);
    scorpion.left_until_black(100);
    scorpion.left_until_white(50);

    // grabbing 4th pile

    scorpion.create.forward(8, 250);
    scorpion.set_claw_to_position(CLAW_PARTIAL + 100);
    scorpion.lower_arm();
    scorpion.create.forward(5, 250);
    scorpion.grab_tribbles();
    msleep(400);

    // move out from under the ridge to score tribbles

    scorpion.set_arm_to_position(ARM_PARTIAL + 600);
    scorpion.create.forward(45, 250);
    scorpion.raise_arm_slow(0.5);
    scorpion.set_claw_to_position(CLAW_PARTIAL);
    scorpion.set_arm_to_position(ARM_PARTIAL);

    // squaring in the center of the field

    scorpion.create.forward(10, 300);
    scorpion.create.right(90, 0, 250);
    scorpion.create.backward(20, 250);
    scorpion.raise_arm();
    scorpion.set_claw_to_position(CLAW_PARTIAL + 300);

    // move block (red/green) out of the way

    scorpion.set_arm_to_position(1800); // not parallel to tribbles
    scorpion.create.forward(30, 250);
    scorpion.close_claw();
    scorpion.set_arm_to_position(ARM_PARTIAL);
    scorpion.create.left(90, 0, 150);
    scorpion.set_arm_to_position(ARM_DOWN - 300);
    scorpion.set_claw_to_position(CLAW_OPEN - 400);
    scorpion.set_arm_to_position(ARM_PARTIAL);
    scorpion.create.right(90, 0, 150);
    scorpion.lower_arm();

    // grabbing 6th pile

    scorpion.create.forward(15, 300);
    scorpion.grab_tribbles();
    scorpion.raise_arm_slow(0.5);
    scorpion.open_claw();
    msleep(500);
    scorpion.close_claw();
}

int main() {
    scorpion = new_scorpion(0);
    scorpion.init();

    enum GameType game_type = SEEDING;

    // scorpion.controller.light_start(0);
    // scorpion.controller.shut_down_in(119);

  /*
    char *arr[3] = { "a", "b", "c" };
    int array_size = sizeof(arr) / sizeof(arr[0]);
    menu = new_menu(arr, array_size);

    menu.show();
  */

    if(game_type == SEEDING) seeding();

    scorpion.deinit();
  	return 0;
}
