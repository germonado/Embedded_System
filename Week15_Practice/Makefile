CROSS_COMPILE ?= arm-none-eabi-
CC := $(CROSS_COMPILE)gcc
CFLAGS = -fno-common -O0 -std=gnu99 \
	-mcpu=cortex-m3 -mthumb \
	-T countdown.ld -nostartfiles -g \

TARGET = countdown.bin
all: $(TARGET)

$(TARGET): countdown.c startup.c
	$(CC) $(CFLAGS) $^ -o countdown.elf
	$(CROSS_COMPILE)objcopy -Obinary countdown.elf countdown.bin
	$(CROSS_COMPILE)objdump -S countdown.elf > countdown.list

qemu: $(TARGET)
	@qemu-system-arm -M ? | grep lm3s811evb >/dev/null || exit
	@echo "Press Ctrl-A and then X to exit QEMU"
	@echo
	qemu-system-arm -M lm3s811evb -semihosting -nographic -kernel countdown.bin

gdb: $(TARGET)
	@qemu-system-arm -M ? | grep lm3s811evb >/dev/null || exit
	@echo "Press Ctrl-A and then X to exit QEMU"
	@echo
	qemu-system-arm -M lm3s811evb -s -S -semihosting -nographic -kernel countdown.bin

clean:
	rm -f *.o *.bin *.elf *.list
