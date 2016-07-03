//
//  Geminiq.h
//  Source written by Justin Yu
//

#include "Controller.h"
#include "Create.h"

enum Servo {
    ARM_SERVO = 0,
    ARM_UP = 680,
    ARM_DOWN = 1980
};

typedef struct Gemini {
    int tophat_port;
    int et_port;

    Controller controller;
    Create create;

    void (*line_follow)(int base_speed, int distance);
    void (*line_follow_until_et)(int speed);

    void (*init)();
    void (*deinit)();

    int (*get_et_value)();
    int (*is_outer_space)();

    void (*lower_arm)();
} Gemini;

Gemini new_gemini(int tophat_port, int et_port);

Gemini gemini;
