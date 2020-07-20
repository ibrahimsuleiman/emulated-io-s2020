#ifndef GPIO_H
#define GPIO_H

//#include "pin.h"


#define LOW 0
#define HIGH 1

#define GPIO_IN 0
#define GPIO_OUT 1
#define NUM_PINS 32





enum pin_mode {
	IN,
	OUT,
};

enum port_label {
	PORT_A,
	PORT_B,
	PORT_C,
	PORT_D,
};


enum io_control_code {
	IOC_RESET_GPIO,
	IOC_INIT_PORT,
	IOC_SET_PORT_OUT,
	IOC_SET_PORT_IN,

};	


#ifdef USE_PUBLIC_API
int init_port(enum port_label x);
/* set all pins in a port to IN/OUT */
int set_port(enum port_label x, enum pin_mode mode);
/* setup a particular GPIO pin*/
/* read 8 bits from a single port connected to external input e.g. ADC*/
int8_t read_port(enum port_label x);
/* write 8-bits to a port*/
int write_port(enum port_label x, uint8_t value);
/* reset all gpio pins*/
void reset_gpio();
/* functions to support individual pin manipulation (not implemented)
void set_up(unsigned channel, enum pin_mode mode);
void write_pin(unsigned channel, uint8_t value);
int read_pin(unsigned channel);
*/
#endif
#endif
