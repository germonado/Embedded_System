#ifndef __REG_H__
#define __REG_H__

#define __REG_TYPE     volatile uint32_t
#define __REG          __REG_TYPE *

/* SysTick Memory Map */
#define SYSTICK        ((__REG_TYPE) 0xE000E010)
#define SYSTICK_CTRL   ((__REG) (SYSTICK + 0x00))
#define SYSTICK_LOAD   ((__REG) (SYSTICK + 0x04))
#define SYSTICK_VAL    ((__REG) (SYSTICK + 0x08))

#endif


