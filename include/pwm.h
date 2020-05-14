#ifndef PWM_H
#define PWM_H

#include "colors.h"

typedef enum {
    RED, 
    GREEN,
    BLUE
} channel_t;

void set_PWM(channel_t channel, int duty_cycle) {
    switch (channel) {
        case RED:
        analogWrite(D5, duty_cycle);
        break;

        case GREEN:
        analogWrite(D6, duty_cycle);
        break;

        case BLUE:
        analogWrite(D7, duty_cycle);
        break;

    }
}

void set_all_PWM(color_t color) {
    set_PWM(RED, color.red);
    set_PWM(GREEN, color.green);
    set_PWM(BLUE, color.blue);
}

#endif