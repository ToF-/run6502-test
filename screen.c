#include <stdio.h>
#include <stdlib.h>

#include "lib6502.h"
#include "screen_bin.h"

/* Emulated OS functions. */

#define SCRN    0xFFBB  /* print a in screen at x y */
#define COLR    0xFFB0  /* put color a in screen at x y */
#define RDCH    0xFFCC  /* read stdin char in accumulator */
#define WRCH    0xFFEE  /* Write accumulator to stdout. */
#define RAND    0x00FE  /* random byte value */

/* Write the accumulator to stdout.  This function will be invoked
 * when the emulated program calls 0xFFEE.
 */
int wrch(M6502 *mpu, uint16_t address, uint8_t data)
{
  int pc;

  /* Write the character.
   */
  putchar(mpu->registers->a);

  /* We arrived here from a JSR instruction.  The stack contains the
   * saved PC.  Pop it off the stack.
   */
  pc  = mpu->memory[++mpu->registers->s + 0x100];
  pc |= mpu->memory[++mpu->registers->s + 0x100] << 8;

  /* The JSR instruction pushes the value of PC before it has been
   * incremented to point to the instruction after the JSR.  Return PC
   * + 1 as the address for the next insn.  Returning non-zero
   * indicates that we handled the 'subroutine' ourselves, and the
   * emulator should pretend the original 'JSR' neveer happened at
   * all.
   */
  return pc + 1;  /* JSR pushes next insn addr - 1 */
}

int rdch(M6502 *mpu, uint16_t address, uint8_t data)
{
    int pc;
    mpu->registers->a = (uint8_t)getchar();
    pc  = mpu->memory[++mpu->registers->s + 0x100];
    pc |= mpu->memory[++mpu->registers->s + 0x100] << 8;
    return pc + 1;
}

/* Exit gracefully.  We arrange for this function to be called when
 * the emulator tries to transfer control to address 0.
 */
int done(M6502 *mpu, uint16_t address, uint8_t data)
{
  exit(0);
}

uint8_t screen[0x1000];

int scrn(M6502 *mpu, uint16_t address, uint8_t data)
{

    int col = mpu->registers->y % 0x40;
    int row = mpu->registers->x % 0x40;
    int value = mpu->registers->a;
    screen[row * col] = value;
    printf("\033[%d;%dH%c",col+1,row+1,value);
    int pc;
    pc  = mpu->memory[++mpu->registers->s + 0x100];
    pc |= mpu->memory[++mpu->registers->s + 0x100] << 8;
    return pc + 1;
}

uint8_t colors[0x1000];
int colr(M6502 *mpu, uint16_t address, uint8_t data)
{
    int col = mpu->registers->y % 0x40;
    int row = mpu->registers->x % 0x40;
    int color = mpu->registers->a & 0x0F;
    int value = screen[row * col];
    colors[row * col] = color;
    int num = color < 8 ? color + 30 : color + 82;
    printf("\033[%dm█", num);
    printf("\033[%d;%dH%c",col+1,row+1, value);
    int pc;
    pc  = mpu->memory[++mpu->registers->s + 0x100];
    pc |= mpu->memory[++mpu->registers->s + 0x100] << 8;
    return pc + 1;

}
int main()
{
  M6502    *mpu = M6502_new(0, 0, 0);   /* Make a 6502 */
  unsigned  pc  = 0x1000;       /* PC for 'assembly' */

  /* install the assembly code into ram */
  for(int i=0; i<BINARY_SIZE; i++) {
      mpu->memory[pc++] = (uint8_t)binary[i];
  }

  /* Install the three callback functions defined above.
   */
  M6502_setCallback(mpu, call, WRCH, wrch); /* Calling FFEE -> wrch() */
  M6502_setCallback(mpu, call, RDCH, rdch); /* Calling FFCC -> rdch() */
  M6502_setCallback(mpu, call, SCRN, scrn); /* Calling FFBB -> scrn() */
  M6502_setCallback(mpu, call, COLR, colr); /* Calling FFB0 -> scrn() */
  M6502_setCallback(mpu, call,    0, done); /* Calling 0 -> done() */

  /* Point the RESET vector at the first instruction in the assembled
   * program.
   */
  M6502_setVector(mpu, RST, 0x1000);

  /* Reset the 6502 and run the program.
   */
  M6502_reset(mpu);
  M6502_run(mpu);
  M6502_delete(mpu);    /* We never reach here, but what the hey. */

  return 0;
}
