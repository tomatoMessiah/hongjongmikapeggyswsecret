#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "interface.h"


/******************** Initialization & Background function ************************************/
int ticks_img=0;

void button_init(void){		//initialize mainboard buttons

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}

void led_init(void){		//initialize LED

    GPIO_InitTypeDef LED_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
    LED_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    LED_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    LED_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOG, &LED_InitStructure);
	GPIO_SetBits(GPIOG, GPIO_Pin_6);                   // Set Red LED off at default init state


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	LED_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	LED_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	LED_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOG, &LED_InitStructure);
	GPIO_SetBits(GPIOG, GPIO_Pin_7);


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	LED_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	LED_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	LED_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOD, &LED_InitStructure);
	GPIO_SetBits(GPIOD, GPIO_Pin_7);
}

void init_all(){
	ticks_init(); // for delay_ms() and for time sync
	button_init();	//init button
	led_init();		//init led
	tft_init(0, WHITE, BLACK, RED);		//initialize LCD screen
	xbc_init(0);	//initialize Xbox controller
}

bool refresh_ticks()	//refresh ticks and update xbc. unchecked
{
	if(ticks_img==get_ticks())
		return 0;
	ticks_img=get_ticks();
	if(ticks_img%20==0)		//update xbox input
		xbc_update();
	return 1;
}
/*
if(!refresh_ticks())
	continue;
main while loop
 */

int calibrate_joy(int val)	//calibration for joystick on Xbox
{
	val/=30;		//scale down
	if(val<-1000)		//max value=1000
		return -1000;
	if(val>1000)
		return 1000;
	if(val<200 && val>-200)		//compensate offset error, should be at least 100
		return 0;
	return val;
}

void delay(int ms)
{
	_delay_ms(ms);
}

/*************************************** Input *************************************************/

bool buttonClicked(char dir)	//u,d,l,r,s
{
	switch (dir)
	{
	case 's':
		if(!GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_6))
			return 1;
		break;
	case 'u':
		if(!GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_7))
			return 1;
		break;
	case 'd':
		if(!GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_8))
			return 1;
		break;
	case 'l':
		if(!GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_9))
			return 1;
		break;
	case 'r':
		if(!GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_10))
			return 1;
		break;
	default:
		break;
	}
	return 0;
}

bool xbc_buttonClicked(char c)	// returns state of each button
{
	if (ticks_img%50==0 && ticks_img!=get_ticks() )//unchecked
	{
		ticks_img=get_ticks();
		xbc_update();
	}

	switch (c)
	{
	case 'a':
		if((xbc_digital & XBC_A))
			return 1;
		break;
	case 'b':
		if((xbc_digital & XBC_B))
			return 1;
		break;
	case 'x':
		if((xbc_digital & XBC_X))
			return 1;
		break;
	case 'y':
		if((xbc_digital & XBC_Y))
			return 1;
		break;
	case 'u':
		if((xbc_digital & XBC_UP))
			return 1;
		break;
	case 'd':
		if((xbc_digital & XBC_DOWN))
			return 1;
		break;
	case 'l':
		if((xbc_digital & XBC_LEFT))
			return 1;
		break;
	case 'r':
		if((xbc_digital & XBC_RIGHT))
			return 1;
		break;
	case 's':
		if((xbc_digital & XBC_START))
			return 1;
		break;


	case 'X':
		if((xbc_digital & XBC_XBOX))
			return 1;
		break;
	case 'B':
		if((xbc_digital & XBC_BACK))
			return 1;
		break;
	case 'L':
		if((xbc_digital & XBC_L_JOY))
			return 1;
		break;
	case 'R':
		if((xbc_digital & XBC_R_JOY))
			return 1;
		break;


	default:
		break;
	}
	return 0;
}


/*************************************** Output *************************************************/

void led_on(char color)	//r,g,b
{
	switch(color)
	{
	case 'r':
		GPIO_ResetBits(GPIOG, GPIO_Pin_6);break;
	case 'g':
		GPIO_ResetBits(GPIOG, GPIO_Pin_7);break;
	case 'b':
		GPIO_ResetBits(GPIOD, GPIO_Pin_7);break;
	default:
		break;
	}
}

void led_off(char color)	//r,g,b
{
	switch(color)
	{
	case 'r':
		GPIO_SetBits(GPIOG, GPIO_Pin_6);break;
	case 'g':
		GPIO_SetBits(GPIOG, GPIO_Pin_7);break;
	case 'b':
		GPIO_SetBits(GPIOD, GPIO_Pin_7);break;
	default:
		break;
	}
}


/****************************** Hardware control **********************************************/
void joystick_control(int* l_dutyCycle, int* r_dutyCycle)//control two motor speed with joystick
{
	int x = calibrate_joy(xbc_joy[2])/10;	//joystick left side x-axs
	int y = calibrate_joy(xbc_joy[3])/10;	//joystick left side y-axis

	if(y>=0)
	{
		(*l_dutyCycle)=y+x/2;//just a proposed method!
		(*r_dutyCycle)=y-x/2;
	}
	else
	{
		(*l_dutyCycle)=y-x/2;
		(*r_dutyCycle)=y+x/2;
	}
	return;
}


void set_dir(int id, bool state) //id = 1,2,3,4 //f=0, b=1 as direction
{
	switch(id)
	{
	case 1:
		GPIO_WriteBit(GPIOG, GPIO_Pin_2,state);break;
	case 2:
		GPIO_WriteBit(GPIOG, GPIO_Pin_3,state);break;//unchecked
	case 3:
		GPIO_WriteBit(GPIOG, GPIO_Pin_4,state);break;//unchecked
	case 4:
		GPIO_WriteBit(GPIOG, GPIO_Pin_5,state);break;//unchecked
	default:
		break;
	}
}

void run(int id, int dutyCycle)	//id=1,2,3,4 // from 0 to 100
{
	switch(id)
	{
	case 1:
		TIM_SetCompare1(TIM4, dutyCycle);break;
	case 2:
		TIM_SetCompare1(TIM4, dutyCycle);break;//unknown
	case 3:
		TIM_SetCompare1(TIM4, dutyCycle);break;//unknown
	case 4:
		TIM_SetCompare1(TIM4, dutyCycle);break;//unknown
	default:
		break;
	}
}

void stop(int id)	//id = 1,2,3,4	motor
{
	switch(id)
	{
	case 1:
		TIM_SetCompare1(TIM4, 0);break;
	case 2:
		TIM_SetCompare1(TIM4, 0);break;//unknown
	case 3:
		TIM_SetCompare1(TIM4, 0);break;//unknown
	case 4:
		TIM_SetCompare1(TIM4, 0);break;//unknown
	default:
		break;
	}
}
