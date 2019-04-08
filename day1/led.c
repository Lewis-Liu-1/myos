#define GPBCON   	(*(volatile unsigned long *)0x56000010)
#define GPBDAT		(*(volatile unsigned long *)0x56000014)
void main(void)
{
	GPBCON=0x01<<10;	//set GPB5 as output pin
	GPBDAT=0<<5;		//GPB5 pin output 0
}
