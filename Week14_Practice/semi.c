#include <stdint.h>

static int semihost_call(int service, void* opaque)
{
    register int r0 asm("r0") = service;
    register void *r1 asm("r1") = opaque;
    register int result asm("r0");
    asm volatile("bkpt 0xab" : "=r" (result) : "r" (r0), "r" (r1));
    return result;
}

enum SEMIHOST_SVC{
    SYS_WRITE = 0x05,
};

char g_message[] = "Hello World!\n";
uint32_t g_param[3];

void main(void)
{
	g_param[0] = 1;
	g_param[1] = g_message;
	g_param[2] = sizeof(g_message);
    //char message[] = "Hello World!\n";
    //uint32_t param[] = {1, (uint32_t)g_message, sizeof(g_message)};
    semihost_call(SYS_WRITE, (void*)g_param);
    while(1);
}
