/*
* Emulated GPIO implementation
* Actions are forwarded to the emulated gpio driver module
* loaded in the kernel. The Emulated GPIO module is modeled as 4 ports (A, B, C and D)
* each consisting of 8 GPIO pins and represented by the character devices at /dev/egpio*
* where * ranges from 0 to 3
*/


#include<sys/types.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<stdint.h>

#define USE_PUBLIC_API

#include"gpio.h"

#define A0	0
#define A7	7
#define B0	8
#define B7	15
#define C0	16
#define C7	23
#define D0	24
#define D7	31


static int get_port_start(unsigned i)
{
	if(i >= A0 && i <= A7)
		return A0;
	else if(i >= B0 && i <= B7)
		return B0;
	else if(i >= C0 && i <= C7)
		return C0;
	else if(i >= D0 && i <= D7)
		return D0;
	else 
		return -1;
}


/* to intialize a gpio port: use*/
int init_port(enum port_label x)
{
	int fd;
	int r1;
	
	
	switch(x) {

	case PORT_A:
		fd = open("/dev/egpio0", O_RDWR);
		if(fd < 0) goto err;
		r1 = ioctl(fd, IOC_INIT_PORT, NULL);
		break;
	case PORT_B:
		fd = open("/dev/egpio1", O_RDWR);
		if(fd < 0) goto err;
		r1 = ioctl(fd, IOC_INIT_PORT, NULL);
		break;
	case PORT_C:
		fd = open("/dev/egpio2", O_RDWR);
		if(fd < 0) goto err;
		r1 = ioctl(fd, IOC_INIT_PORT, NULL);
		break;
	case PORT_D:
		fd = open("/dev/egpio3", O_RDWR);
		if(fd < 0) goto err;
		r1 = ioctl(fd, IOC_INIT_PORT, NULL);
		break;

	default:
		break;

	}
	
	if(r1 < 0)
		goto err;
	
	close(fd);
	return 0;
	
err: close(fd);
	return -1;

}
		
/* set all pins in a port to IN/OUT. This is useful because
* the emulated sensor module sends 8 bits of data at a time, and we
* use all 8 GPIO pins in a port to recieve the input
*/
int set_port(enum port_label x, enum pin_mode mode)
{

	int fd;
	int r1;
	
	switch(x) {
	
		case PORT_A:
			fd = open("/dev/egpio0", O_RDWR);
			if(fd < 0) goto err;
			r1 = ioctl(fd, mode ? IOC_SET_PORT_IN : IOC_SET_PORT_OUT, NULL);
			break;
		case PORT_B:
			fd = open("/dev/egpio1", O_RDWR);
			if(fd < 0) goto err;
			r1 = ioctl(fd, mode ? IOC_SET_PORT_IN : IOC_SET_PORT_OUT, NULL);
			break;
		case PORT_C:
			fd = open("/dev/egpio2", O_RDWR);
			if(fd < 0) goto err;
			r1 = ioctl(fd, mode ? IOC_SET_PORT_IN : IOC_SET_PORT_OUT, NULL);
			break;
		case PORT_D:
			fd = open("/dev/egpio3", O_RDWR);
			if(fd < 0) goto err;
			r1 = ioctl(fd, mode ? IOC_SET_PORT_IN : IOC_SET_PORT_OUT, NULL);
			break;

		default:
			break;
	
	}
	
	
	if(r1 < 0)
		goto err;

	return 0;
		
err:	close(fd);
	return -1;
	

}

/* read 8 bits from a single port connected to external input e.g. ADC*/
int8_t read_port(enum port_label x)
{

	int fd;
	int8_t val = 0;
	
	switch(x) {
		case PORT_A:
			fd = open("/dev/egpio0", O_RDWR);
			break;
		case PORT_B:
			fd = open("/dev/egpio1", O_RDWR);
			break;
		case PORT_C:
			fd = open("/dev/egpio2", O_RDWR);
			break;
		case PORT_D:
			fd = open("/dev/egpio3", O_RDWR);
			break;
		default:
			fd = -1;
			break;
	}	
	
	
	if(fd < 0)
		goto read_error;

	
	if(read(fd, &val, sizeof(val)))
		goto read_error;
	
	
	
	return val;

read_error: return -1;

}


/* write 8-bits to a port*/
int write_port(enum port_label x, uint8_t value)
{

	int fd;
		
	switch(x) {
	case PORT_A:
		fd = open("/dev/egpio0", O_RDWR);
		break;
	case PORT_B:
		fd = open("/dev/egpio1", O_RDWR);
		break;
	case PORT_C:
		fd = open("/dev/egpio2", O_RDWR);
		break;
	case PORT_D:
		fd = open("/dev/egpio3", O_RDWR);
		break;
	default:
		fd = -1;
		break;
	}
	
	if(fd < 0)
		goto write_error;

	
	if(write(fd, &value, sizeof(value)))
		goto write_error;
	
	
	
	return 0;

write_error: return -1;


}
/* reset all gpio pins*/
void reset_gpio()
{

	int fd, r1;
	fd = open("/dev/egpio0", O_RDWR);
	r1 = ioctl(fd, IOC_RESET_GPIO, NULL);
	close(fd);

}
