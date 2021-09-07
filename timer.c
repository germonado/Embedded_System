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
}

void vk_timer_irq_disable(void)
{
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
		break;
	default: break;
	}
}
void vh_timer_interrupt_handler(void)
{
	vk_timer_irq_disable();
	vh_save_thread_ctx(vk_timer_save_stk);
	
	// timer interrupt clear & enable
	
	vk_sched_save_tcb_ptr = (unsigned int)vk_timer_save_stk;
	vk_timer_flag = 1;

	++(vk_current_thread->cpu_tick);
	if(vk_sched_lock==0) {
		vk_swi_scheduler();
	}
}
