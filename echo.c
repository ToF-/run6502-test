#include <stdio.h>
#include <stdlib.h>

#include "lib6502.h"

/* Emulated OS functions. */

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
  char buffer[64];

  /* Dump the internal state of the processor.
   */
  M6502_dump(mpu, buffer);

  /* Print a cute message and quit.
   */
  printf("\nBRK instruction\n%s\n", buffer);
  exit(0);
}

#define ASSEMBLY_SIZE 55
uint8_t assembly[ASSEMBLY_SIZE] = {
  0xa0, 0x00, 0x20, 0xcc, 0xff, 0xc9, 0x0a, 0xf0, 0x06, 0x99, 0x37, 0x10, 0xc8, 0x10, 0xf3, 0xc0
, 0x00, 0xf0, 0x23, 0x8c, 0x36, 0x11, 0xa0, 0x00, 0xcc, 0x36, 0x11, 0xf0, 0x0a, 0xb9, 0x37, 0x10
, 0x20, 0xee, 0xff, 0xc8, 0x4c, 0x18, 0x10, 0xa9, 0x0d, 0x20, 0xee, 0xff, 0xa9, 0x0a, 0x20, 0xee
, 0xff, 0xac, 0x36, 0x11, 0xd0, 0xca, 0x60 };
        
int main()
{
  M6502    *mpu = M6502_new(0, 0, 0);   /* Make a 6502 */
  unsigned  pc  = 0x1000;       /* PC for 'assembly' */

  /* install the assembly code into ram */
  for(int i=0; i<ASSEMBLY_SIZE; i++) {
      mpu->memory[pc++] = (uint8_t)assembly[i];
  }

  /* Install the three callback functions defined above.
   */
  M6502_setCallback(mpu, call, WRCH, wrch); /* Calling FFEE -> wrch() */
  M6502_setCallback(mpu, call, RDCH, rdch); /* Calling FFCC -> rdch() */
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
