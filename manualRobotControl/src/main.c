#include "interface.h"



int main(void)
{
	init_all();

	while(1)
	{
		if(buttonClicked('u'))
		{
			led_on('b');
			while(buttonClicked('u'));
			led_off('b');
		}
		if(buttonClicked('d'))
		{
			led_on('g');
			while(buttonClicked('d'));
			led_off('g');
		}
		if(buttonClicked('r'))
		{
			led_on('r');
			while(buttonClicked('r'));
			led_off('r');
		}





	}

	return 0;
}
