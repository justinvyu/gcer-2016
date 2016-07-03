//
//  Animal.c
//

#include "Animal.h"

static void init() {

}

static void deinit() {

}

Animal new_animal() {
     Animal instance = {
         .init = &init,
         .deinit = &deinit
     };
     instance.controller = new_controller(1, 0, 14.5, 5.0);
     return instance;
}
