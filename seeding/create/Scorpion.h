//
//  Scorpion.h
//  Source written by Justin Yu
//

#include <kipr/botball.h>
#include "Create.h"
#include "Controller.h"
#include "Camera.h"

enum ServoPositions {
    ARM_SERVO = 0,
    CLAW_SERVO = 1,
    BACK_SERVO = 2,

    ARM_UP = 50,
    ARM_PARTIAL = 1000,
    ARM_DOWN = 2020,

    CLAW_OPEN = 1050,
    CLAW_PARTIAL = 800,
    CLAW_CLOSED = 0,

    BACK_UP = 600,
    BACK_DOWN = 2000
};

enum Sensor {
    LIGHT_SENSOR = 0,
    CALIB_TOUCH_SENSOR = 0
};

#define SERVO_DURATION 1.2

enum Channel {
    RED_CHANNEL = 0,
    GREEN_CHANNEL = 1
};

enum Case {
    PILE_ONE = 0,
    PILE_TWO = 1,
    PILE_THREE = 2
};

typedef struct Scorpion {
    // Inheriting from Create
    Create create;
    Controller controller;
    Camera camera;

    int tophat_port;
    int touch_port;

    // Custom properties/instance methods
    void (*grab_tribbles)();
    void (*grab_tribbles_slow)();
    void (*grab_tribbles_async)();
    void (*shake_arm)();
    enum Channel (*track)();
    enum Channel (*get_tribble_color)();

    void (*determine_action)(int *red, int *green);
    void (*isolate_tribbles)();

    void (*init)();
    void (*deinit)();

    void (*lower_arm)();
    void (*raise_arm)();
    void (*raise_arm_slow)(float time);
    void (*set_arm_to_position)(int position);

    void (*close_claw)();
    void (*close_claw_slow)(float time);
    void (*open_claw)();
    void (*open_claw_slow)(float time);
    void (*set_claw_to_position)(int position);

    // async calls
    void (*lower_arm__open_claw)();

    void (*lift_basket)();
    void (*drop_basket)();
    void (*lift_basket_slow)();
    void (*drop_basket_slow)();

    // tophat
    int (*get_tophat_value)();
    void (*forward_until_black)(int speed);
    void (*left_until_black)(int speed);
    void (*left_until_white)(int speed);
    void (*right_until_black)(int speed);
    void (*right_until_white)(int speed);

    void (*line_follow)(int create_distance);
    void (*line_follow_backward)(int create_distance);

    // digital
    int (*get_touch_value)();
    void (*forward_until_touch)(int speed);

    // subroutines
    void (*move_botguy)();
    void (*score)(enum Case tribble_pile);
} Scorpion;

extern Scorpion new_scorpion(int tophat, int touch); // constructor

// Global scorpion object
Scorpion scorpion;
