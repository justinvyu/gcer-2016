//
//  Animal.h
//

#include <kipr/botball.h>
#include "Controller.h"

typedef struct Animal {
    // inherit
    Controller controller;

    void (*init)();
    void (*deinit)();
} Animal;

extern Animal new_animal();

Animal wallaby;
