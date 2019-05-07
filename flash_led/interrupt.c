#include "s3c24xx.h"
#include "2440addr.h"

//extern void I2CIntHandle(void);
#define	enable_isr(bit)		rINTMSK &= ~(bit)
#define	disable_isr(bit)		rINTMSK |= (bit)

/*
 * LED1-4对应GPB5、GPB6、GPB7、GPB8
 */
#define GPB5_out        (1<<(5*2))      // LED1
#define GPB6_out        (1<<(6*2))      // LED2
#define GPB7_out        (1<<(7*2))      // LED3
#define GPB8_out        (1<<(8*2))      // LED4

/*
 * K1-K4对应GPG0、GPG3、GPG5、GPG6
 */
#define GPG0_eint      	(2<<(0*2))      // K1,EINT8
#define GPG3_eint       (2<<(3*2))      // K2,EINT11
#define GPG5_eint       (2<<(5*2))      // K3,EINT13
#define GPF6_eint       (2<<(6*2))      // K4,EINT14

#define FCLK 400000000
#define HCLK (FCLK/4)
#define PCLK (HCLK/2)

void (*isr_handle_array[50])(void);

extern volatile int sys_1_s_tick;
volatile int MiliSecCount = 0;

void timer4_isr(void) {

	if (++MiliSecCount == 1000) {
		sys_1_s_tick = 1;
		MiliSecCount = 0;
	}
}

void isr_dummy(void) {
	while (1)
		;
}

void init_timer4(void) {

	rTCON &= 0xff8fffff;  //clear manual update bit, stop Timer1
	rTCFG0 &= 0xffff00ff;   //clear Timer 2,3 & 4 prescaler 1
	rTCFG0 |= 0xf00;        //set prescaler = 15+1 = 16
	rTCFG1 &= 0xfff0ffff;   //set Timer4 1/2 Mux
	rTCFG1 |= 0x10000;      //set Timer4 MUX 1/4
	rTCNTB4 = (PCLK / (4*15*1000)) - 1; // 1 mSec Time value
	rTCON |= 0x200000;      //   manual updata
	rTCON &= 0xff8fffff;    // down the manual update flag
	rTCON |= 0x500000;  // interval mode auto reload and start

	clear_pending_isr(BIT_TIMER4);
	enable_isr(BIT_TIMER4);
}

void init_ext_interrupt() {
	GPGCON = GPG0_eint | GPG3_eint | GPG5_eint | GPF6_eint;

	// 对于EINT8、11，需要在EINTMASK寄存器中使能它们
	EINTMASK &= (~(1 << 8)) & (~(1 << 11)) & (~(1 << 13)) & (~(1 << 14));

	/*
	 * 设定优先级：
	 * ARB_SEL0 = 00b, ARB_MODE0 = 0: REQ1 > REQ3，即EINT0 > EINT2
	 * 仲裁器1、6无需设置
	 * 最终：
	 * EINT0 > EINT2 > EINT11,EINT19，即K4 > K3 > K1,K2
	 * EINT11和EINT19的优先级相同
	 */
	PRIORITY = (PRIORITY & ((~0x01) | (0x3 << 7))) | (0x0 << 7);

	// EINT8_23使能
	INTMSK &= (~(1 << 5));

}

void init_irq(void) {
	int i = 0;
	for (i = 0; i < sizeof(isr_handle_array) / sizeof(isr_handle_array[0]);
			i++) {
		isr_handle_array[i] = isr_dummy;
	}

	INTMOD = 0x0;	      // 所有中断都设为IRQ模式
	INTMSK = BIT_ALLMSK;  // 先屏蔽所有中断

	init_timer4();
	//isr_handle_array[ISR_IIC_OFT]  = I2CIntHandle;
//	isr_handle_array[ISR_TIMER0_OFT] = timer0_interrupt;
	isr_handle_array[ISR_TIMER4_OFT] = timer4_isr;
	//isr_handle_array[ISR_EINT8_23_OFT] = timer4_isr;
	init_ext_interrupt();
}



void IRQ_Handle(void) {
	unsigned long oft = INTOFFSET;

	//清中断
	//if (oft == 4)
	//	EINTPEND = 1 << 7;   //EINT4-7合用IRQ4，注意EINTPEND[3:0]保留未用，向这些位写入1可能导致未知结果
	//SRCPND = 1 << oft;
	//INTPND = INTPND;
	SRCPND = 1 << oft;
	INTPND = 1 << oft;

	// 调用中断服务程序
	if (oft==5)
	{
		EINT_Handle();
	}
	else isr_handle_array[oft]();
}

void EINT_Handle() {
	unsigned long oft = INTOFFSET;
	unsigned long val;

	switch (oft) {
	// K4被按下
	case 0: {
		GPBDAT |= (0x0f << 5);   // 所有LED熄灭
		GPBDAT &= ~(1 << 8);      // LED4点亮
		break;
	}

		// K3被按下
	case 2: {
		GPBDAT |= (0x0f << 5);   // 所有LED熄灭
		GPBDAT &= ~(1 << 7);      // LED3点亮
		break;
	}

		// K1或K2被按下
	case 5: {
		GPBDAT |= (0x0f << 5);   // 所有LED熄灭

		// 需要进一步判断是K1还是K2，或是K1、K2被同时按下
		val = EINTPEND;
		if (val & (1 << 14))
			GPBDAT &= ~(1 << 8);      // K4被按下，LED4点亮
		if (val & (1 << 13))
			GPBDAT &= ~(1 << 7);      // K3被按下，LED3点亮
		if (val & (1 << 11))
			GPBDAT &= ~(1 << 6);      // K2被按下，LED2点亮
		if (val & (1 << 8))
			GPBDAT &= ~(1 << 5);      // K1被按下，LED1点亮

		break;
	}

	default:
		break;
	}

	//清中断
	if (oft == 5)
	{
		EINTPEND = (1 << 8) | (1 << 11) | (1 << 13) | (1 << 14); // EINT8_23合用IRQ5
	}

	SRCPND = 1 << oft;
	INTPND = 1 << oft;
}

