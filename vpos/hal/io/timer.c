#include "vh_cpu_hal.h"
#include "vh_io_hal.h"
#include "timer.h"
#include "printk.h"
#include "dd.h"
#include "hwi_handler.h"
#include "scheduler.h"
#include "thread.h"
#include "queue.h"
#include "semaphore.h"
#include "recoplay.h"

extern int vk_sched_lock;
extern vk_thread_t *vk_current_thread;


void vk_timer_irq_enable(void)
{	
	// 1. Clear all the Timer4 related bits of TCON to 0
	vh_TCON &= 0xff8fffff;
	// 2. Auto Reload On & Manual Update in TCON
	vh_TCON |= 0x00600000;
	// 3. Clear all the Timer4 related bits of TCON to 0
	vh_TCON &= 0xff8fffff;
	// 4. Auto Reload On & Timer4 Start in TCON
	vh_TCON |= 0x00500000;
	// 5. Timer4 Interrupt Enable & Timer4 Interrupt Status Bit Clear in TINT_CSTAT
	vh_TINT_CSTAT = 0x00000210;

}


void vk_timer_irq_disable(void)
{
	// 1. Clear all the Timer4 related bits of TCON to 0
	vh_TCON = 0xff8fffff;
	// 2. Auto Reload On & Manual Update in TCON
	vh_TCON |= 0x00600000;
	// 3. Timer4 Interrupt Disable & Timer4 Interrupt Status Bit Clear in TINT_CSTAT
	vh_TINT_CSTAT |= 0x00000200;

}

void vh_timer_init(void)
{
	// we should set interrupt frequency 1 second, clock source will be set in the UCON1 reg > 66MHz 	
	vk_timer_flag = 0;
	// TCFG0 set to 255 for Timer4 prescaler
	vh_TCFG0 = 0xff00;
	// TCFG1 set to divider value of Timer 4
	vh_TCFG1 = 0x40000;
	// TCNTB4 set to 1 interrupt per 1 second 
	// TCNTB4 will count set value and scaled clock
	vh_TCNTB4 = 0x3ef1;

}

void vh_timer_irq_enable(int timer)
{
	
	switch(timer){
	case 0: 
		 break;
	case 1: 
		 break;
	case 2:
		 break;
	case 3:	
		 break;
	case 4:
		 // 1. save ISR address in VIC0VECTADDR25 register
		 vh_VIC0VECTADDR25 = (unsigned int)&vh_timer_interrupt_handler;
		 // 2. Enable Timer4 interrupt in VIC0INTENABLE register
		 vh_VIC0INTENABLE |= vh_VIC_TIMER4_bit;
		 // 3. Set Timer4 interrupt to IRQ in VIC0INTSELECT register
		 vh_VIC0INTSELECT &= !vh_VIC_TIMER4_bit;
		 // 4. Mask all VIC0SWPRIORITYMASK register
		 vh_VIC0SWPRIORITYMASK = 0xffffffff; 
		break;
	default: break;
	}
}
void vh_timer_interrupt_handler(void)
{
	vk_timer_irq_disable();
	vh_save_thread_ctx(vk_timer_save_stk);
	
	// timer interrupt clear & enable
	vh_VIC0INTENCLEAR |= vh_VIC_TIMER4_bit;
	vh_VIC0INTENABLE |= vh_VIC_TIMER4_bit;
	
	vk_sched_save_tcb_ptr = (unsigned int)vk_timer_save_stk;
	vk_timer_flag = 1;

	++(vk_current_thread->cpu_tick);
	if(vk_sched_lock==0) {
		vk_swi_scheduler();
	}
}
