BUILD_DIR = build

LINKER_SCRIPT   = link.ld

AS              = nasm
KERNEL_AS_FLAGS = -f elf32
CC              = clang
KERNEL_CC_FLAGS = -m32
LD              = ld
KERNEL_LD_FLAGS = -m elf_i386 -T ${LINKER_SCRIPT}

KERNEL_ASM_SRC = ${wildcard kernel/asm/*.s}
KERNEL_C_SRC   = ${wildcard kernel/src/*.c}

KERNEL_ASM_OBJ = ${KERNEL_ASM_SRC:kernel/asm/%=${BUILD_DIR}/kernel/%.o}
KERNEL_C_OBJ   = ${KERNEL_C_SRC:kernel/src/%=${BUILD_DIR}/kernel/%.o}

KERNEL_BIN     = ${BUILD_DIR}/kernel/kernel

run: ${KERNEL_BIN}
	qemu-system-i386 -kernel ${KERNEL_BIN}

${KERNEL_BIN}: CREATE_OUT_STRUCTURE ${KERNEL_ASM_OBJ} ${KERNEL_C_OBJ}
	${LD} ${KERNEL_LD_FLAGS} -o $@ ${KERNEL_ASM_OBJ} ${KERNEL_C_OBJ}


${BUILD_DIR}/kernel/%.s.o: kernel/asm/%.s
	${AS} ${KERNEL_AS_FLAGS} -o $@ $^

${BUILD_DIR}/kernel/%.c.o: kernel/src/%.c
	${CC} ${KERNEL_CC_FLAGS} -c -o $@ $^

CREATE_OUT_STRUCTURE:
	mkdir -p ${BUILD_DIR}/kernel/

