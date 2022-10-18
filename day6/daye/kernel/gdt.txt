

**Q1：关于设置GDTR汇编代码**
---

	_load_gdtr:	;void load_gdtr(int limit,int addr);
		MOV	AX,[SP+4]
		MOV	[SP+6],AX
		LGDT	[SP+6]
		RET
首先GDTR是一个48位寄存器，低16位是段上限，高32位是段基址。
假设C中调用函数时传入的参数：load_gdtr(0x0000ffff,0x00270000);所以目的就是将GDTR赋值成：00270000/ffff。
SP指针是这样的：	
		
	+-------+----->低
	|  FF	|<-SP+4
	+-------+			
	|  FF	|			
	+-------+
	|  00	|<-SP+6			
	+-------+
	|  00	|
	+-------+
	|  00	|
	+-------+
	|  00	|
	+-------+
	|  27	|
	+-------+
	|  00	|
	+-------+---->高	
AX是一个2字节的寄存器，所以 MOV	AX,[SP+4] 后，AX寄存器存储的是FFFF；
执行 MOV	    [SP+6],AX 后，栈是这样的：

	+-------+----->低
	|  FF	|<-SP+4
	+-------+			
	|  FF	|			
	+-------+
	|  FF	|<-SP+6			
	+-------+
	|  FF	|
	+-------+
	|  00	|
	+-------+
	|  00	|
	+-------+
	|  27	|
	+-------+
	|  00	|
	+-------+---->高	
所以再将以SP+6为首地址的6字节赋值给GDTR则完成了GDTR的赋值：LGDT	[SP+6]


**Q2：初始化GDT时C代码的解释**
---
    typedef struct SEGMENT_DESCRIPTOR {
	short limit_low, base_low;
	char base_mid, access_right;
	char limit_high, base_high;
	}SEGMENT_DESCRIPTOR;

	void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar)
	{
	if (limit > 0xfffff) {
		ar |= 0x8000; /* G_bit = 1 */
		limit /= 0x1000;
	}
	sd->limit_low    = limit & 0xffff;
	sd->base_low     = base & 0xffff;
	sd->base_mid     = (base >> 16) & 0xff;
	sd->access_right = ar & 0xff;
	sd->limit_high   = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0);
	sd->base_high    = (base >> 24) & 0xff;
	return;
	}


首先来看一下SEGMENT_DESCRIPTOR的8字节字段：


1. 段基址分为三段：2字节的base_low,1字节的base_mid,1字节的base_high。之所以分成三段是为了与8286时代的程序兼容。


2. 段上限分为两段：2字节的limit_low,1字节的limit_high。但是limit_high的高四位存放的段属性，所以只有20位来存储段上限，
20位最大只能指定2^20(1MB)的段上限，解决办法是，在段属性里面设置一个标志位：Gbit，当这个标志位是1时，limit的单位就变成了页（4KB），
此时最大指定的段上限：1MB*4KB=4GB。


3. 段属性，也是段的访问权属性；1字节的access_right。（加上limit_high高四位，共12位）
我们看到程序里面传入的参数ar(access right的缩写)是int类型，但我们处理起来是把ar当做如下的16位构成来处理的：
xxxx0000xxxxxxxx(x可以是0也可以是1，就是段属性12位的内容啦)。
这样程序就很容易理解了：
access_right就是ar低8位，limit_high低4位就是传入的limit右移16位后的低4位，limit_high高4位就是ar右移8位后的高4位。