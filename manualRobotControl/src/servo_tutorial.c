#include "servo_tutorial.h"

static SERVO_PWM_STRUCT servo_pwm ={{TIM_Channel_1, GPIO_Pin_12, ENABLE, TIM_OC1Init},
									{TIM_Channel_2, GPIO_Pin_13, ENABLE, TIM_OC2Init},
									{TIM_Channel_3, GPIO_Pin_14, ENABLE, TIM_OC3Init},
									{TIM_Channel_4, GPIO_Pin_15, ENABLE, TIM_OC4Init}};
									
void servo_init(void){
	GPIO_InitTypeDef SERVO_GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	u8 servo_id;
	
	RCC_APB1PeriphClockCmd(SERVO_TIM_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(SERVO_GPIO_RCC | RCC_APB2Periph_AFIO, ENABLE);	// Enable bus
	
	SERVO_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				// Push-Pull Output Alternate-function
	SERVO_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);
	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;			// counter will count up (from 0 to FFFF)
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV2;					//timer clock = dead-time and sampling clock 	

	//------------------------------//
	TIM_TimeBaseStructure.TIM_Prescaler = ;								//clk=72M/(?+1)= Hz, interval=?
	TIM_TimeBaseStructure.TIM_Period = ;								//pulse cycle= ?
	//------------------------------//
	
	TIM_TimeBaseInit(SERVO_TIM, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;   	//set "high" to be effective output
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	           	//produce output when counter < CCR
	
	//------------------------------//
	TIM_OCInitStructure.TIM_Pulse = ;													// this part sets the initial CCR value
	//------------------------------//
	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  	 // this part enable the output
	
	TIM_ARRPreloadConfig(SERVO_TIM, ENABLE);
	TIM_Cmd(SERVO_TIM, ENABLE);	

	for(servo_id=0; servo_id<4; servo_id++){
		if(servo_pwm[servo_id].state==DISABLE)
			continue;
		SERVO_GPIO_InitStructure.GPIO_Pin|=servo_pwm[servo_id].servo_pin;
		servo_pwm[servo_id].oc_init_function(SERVO_TIM, &TIM_OCInitStructure);
	}
	GPIO_Init(SERVO_PORT, &SERVO_GPIO_InitStructure);	
}


/***************************************************************************************************	
	for hitec 5945 & 7955 and futaba S3010, min pulse width=900us, max pulse width=2100us
	val is the percentage of possible turning angles for the servo from 0.1% to 100%
	equation: desired_pulse_width=1/tim_clk*ccr_val
	
	example: desired_pulse_width=1232us, ccr_val=1232*1u/(1/250K)=308
		p.s. 1u (10^-6) is for normalising the scale for val since it is in 10^-6 scale
	i.e. min ccr_val=225, max ccr_val=525
	
***************************************************************************************************/

// Function for changing the PWM duty cycle  ( use when you change speed )
//  You can put your motor control function here
//  remember to declare your function in the .h file

