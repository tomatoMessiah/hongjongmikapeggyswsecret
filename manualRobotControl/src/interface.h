/*
 * gpio_def.h
 *
 *  Created on: 2014. 10. 04.
 *      Author: CHOI, Hong Joon
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <delay.h>
#include <ticks.h>
#include <stm32f10x_tim.h>
#include "lcd.h"
#include "stm32f10x.h"
#include "servo_tutorial.h"
#include "motor_pwm_tutorial.h"
#include "encoder.h"
#include "xbc_mb.h"
#ifndef INTERFACE_H_
#define INTERFACE_H_

u32 volatile xbc_digital;
int ticks_img;//ticks

void button_init(void);
void led_init(void);
void init_all(void);

bool refresh_ticks(void);
int calibrate_joy(int);
void delay(int);

bool buttonClicked(char);
bool xbc_buttonClicked(char);

void led_on(char);
void led_off(char);

void joystick_control(int*, int*);
void set_dir(int, bool);
void run(int, int);
void stop(int);

#endif /* GPIO_DEF_H_ */
