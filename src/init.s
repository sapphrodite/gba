@ See GBA manual, pages 170-171, for ROM header description
.GLOBAL     _ROM_HEADER
_ROM_HEADER:
    b start          @ First instruction must be a branch
    .fill 176,1,0    @ For now, let's ignore the other fields!

.ALIGN 4
start:
    @ Initialize interrupt stack pointer
    mov     r0, #0x12
    msr     cpsr, r0
    ldr     sp, = __sp_irq

    @ Initialize ARM stack pointer
    mov     r0, #0x1f
    msr     cpsr, r0
    ldr     sp, = __sp_usr

    @ Enter Thumb mode
    add    r0,pc, #1
    bx     r0
    .THUMB

    @ Call main
    ldr     r3,=main
    bx      r3

