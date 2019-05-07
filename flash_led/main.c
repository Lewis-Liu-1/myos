#include <stdio.h>
#include "serial.h"
#include "lcdlib.h"
#include "s3c24xx.h"

volatile int sys_1_s_tick = 0;

/*
 * LED1-4对应GPB5、GPB6、GPB7、GPB8
 */
#define GPB5_out        (1<<(5*2))      // LED1
#define GPB6_out        (1<<(6*2))      // LED2
#define GPB7_out        (1<<(7*2))      // LED3
#define GPB8_out        (1<<(8*2))      // LED4

void init_led(void)
{
    GPBCON = GPB5_out | GPB6_out | GPB7_out | GPB8_out ;
}

int main() {
	char c;

	init_led();

	uart0_init();   // 波特率115200，8N1(8个数据位，无校验位，1个停止位)
	printf("\r\n##### Test TFT LCD #####\r\n");

#if 1
	while (1) {
		if (sys_1_s_tick) {
			GPBDAT = ~(GPBDAT & (0xf << 5));
			sys_1_s_tick = 0;
		}
	}
#endif

	while (1) {
		printf("\r\n##### Test TFT LCD #####\r\n");
		printf("[1] TFT320240 8Bit\n\r");
		printf("[2] TFT320240 16Bit\n\r");
		printf("Enter your selection: ");

		c = getc();
		printf("%c\n\r", c);
		switch (c) {
		case '1': {
			Test_Lcd_Tft_8Bit_320240();
			break;
		}

		case '2': {
			Test_Lcd_Tft_16Bit_320240();
			break;
		}

		default:
			break;
		}
	}

	return 0;
}
