#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<unistd.h>

#define USE_PUBLIC_API


#include "gpio.h"



int main()
{
	int8_t v = 0;
	
	while(1) {
		
		v = read_port(PORT_A);
		
		printf("%d\n", v);
		
		sleep(1);
		
	}
}
		
