PREFIX=arm-none-eabi-
CC=${PREFIX}gcc 
LD=${PREFIX}ld 
OBJCOPY=${PREFIX}objcopy 
CFLAGS+= -D_ROM=128k -D_RAM=64k -D_ROM_OFF=0x08000000 -D_RAM_OFF=0x20000000
CFLAGS+= -mcpu=cortex-m4 -mthumb -DSTM32F4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS+= -ICMSIS/Include/ -I/usr/include -Icmsis_core/Include/ -DSTM32F407xx
CFLAGS+= -std=c99 -O2

CSRC+=main.c
CSRC+=sdcard.c
CSRC+=system_stm32f4xx.c


ASMSRC+=startup_stm32f407xx.s
ASMOBJ+=${ASMSRC:.s=.o}

.PHONY: flash all asm 
all: main.bin 
%.o: %.s
	arm-none-eabi-as $< -g -o $@

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@

main.elf: ${ASMOBJ} ${CSRC:.c=.o}
	$(LD) -Tlink.ld $^ -o $@

%.bin: %.elf
	$(OBJCOPY) $^ $@ -Obinary

flash:
	st-flash --reset --format binary --flash=128k write main.bin 0x08000000


clean:
	rm -rf *.o *.elf *.bin

