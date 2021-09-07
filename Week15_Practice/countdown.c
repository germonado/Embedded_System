#include <stdint.h>
#include "reg.h"

static int semihost_call(int service, void *opaque){
	register int r0 asm("r0") = service;
	register void *r1 asm("r1") = opaque;
	register int result asm("r0");
	asm volatile("bkpt 0xab"
			: "=r" (result) : "r" (r0), "r" (r1));
	return result;
}

enum SEMIHOST_SVC {
	SYS_WRITE = 0x05,
};

void main(void){
	/* SysTick configuration */
	*SYSTICK_LOAD = 5000000;
	*SYSTICK_VAL = 0;
	*SYSTICK_CTRL = 0x07;

	while(1);
}

int systick_handler(void){
	char seconds[] = {'5','4','3','2','1','0'};
	static int counts = 5;
	uint32_t param[] = {1, (uint32_t)&seconds[counts], 1};
	semihost_call(SYS_WRITE, (void*) param);
	counts = counts < 0 ? 5 : counts - 1;
}
