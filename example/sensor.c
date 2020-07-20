
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#define USE_PUBLIC_API
#include "gpio.h"



int main()
{
	srand(time(NULL));
	init_port(PORT_A);
	set_port(PORT_A, GPIO_IN);
	
	int8_t v = 0;
	
	while(1) {
		
		v = rand() % 128;
		write_port(PORT_A, v);
		
		sleep(1);
		
		//printf("%d\n", read_port(PORT_A));
		
	}	
	
}	
