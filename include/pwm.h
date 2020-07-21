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
        if (!duty_cycle) digitalWrite(13, LOW);
        else analogWrite(13, duty_cycle);
        break;

        case GREEN:
        if (!duty_cycle) digitalWrite(12, LOW);
        else analogWrite(12, duty_cycle);
        break;

        case BLUE:
        if (!duty_cycle) digitalWrite(14, LOW);
        else analogWrite(14, duty_cycle);
        break;

    }
}

void set_all_PWM(color_t color) {
    set_PWM(RED, color.red);
    set_PWM(GREEN, color.green);
    set_PWM(BLUE, color.blue);
}

#endif