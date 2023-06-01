#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef CPU_STACK_SIZE
#define CPU_STACK_SIZE 512
#endif // !CPU_STACK_SIZE

typedef struct CpuInstruction CpuInstruction;

typedef enum CpuOp {
  /****************
   *   Generic    *
   ****************/

  CPU_OP_NOP,

  /* Program flow */
  CPU_OP_JMP,
  CPU_OP_JE,
  CPU_OP_JL,
  CPU_OP_JLE,
  CPU_OP_JG,
  CPU_OP_JGE,

  /* Util */

  CPU_OP_SYSCALL,
  CPU_OP_DEBUG,

  CPU_OP_EXIT,

  /****************
   *     I64      *
   ****************/

  /* Memory */

  CPU_OP_I64_MOV,
  CPU_OP_I64_MOVR,
  CPU_OP_I64_MOVS,

  CPU_OP_I64_PUSHI,
  CPU_OP_I64_PUSHR,

  CPU_OP_I64_SMOVI,
  CPU_OP_I64_SMOVR,

  CPU_OP_I64_POP,

  CPU_OP_I64_CMPI,
  CPU_OP_I64_CMPR,

  /* Math */
  CPU_OP_I64_ADDI,
  CPU_OP_I64_ADDR,

  CPU_OP_I64_SUBI,
  CPU_OP_I64_SUBR,

  CPU_OP_I64_MULI,
  CPU_OP_I64_MULR,

  CPU_OP_I64_DIVI,
  CPU_OP_I64_DIVR,

  /****************
   *     I8      *
   ****************/

  /* Memory */

  CPU_OP_I8_MOV,
  CPU_OP_I8_MOVR,
  CPU_OP_I8_MOVS,

  CPU_OP_I8_PUSHI,
  CPU_OP_I8_PUSHR,

  CPU_OP_I8_SMOVI,
  CPU_OP_I8_SMOVR,

  CPU_OP_I8_POP,

  CPU_OP_I8_CMPI,
  CPU_OP_I8_CMPR,

  /* Math */
  CPU_OP_I8_ADDI,
  CPU_OP_I8_ADDR,

  CPU_OP_I8_SUBI,
  CPU_OP_I8_SUBR,

  CPU_OP_I8_MULI,
  CPU_OP_I8_MULR,

  CPU_OP_I8_DIVI,
  CPU_OP_I8_DIVR,
} CpuOp;

typedef enum Register {
  CPU_R1,
  CPU_R2,
  CPU_R3,
  CPU_R4,
  CPU_R5,
  CPU_R6,
  CPU_R7,
  CPU_R8,
  CPU_REGISTER_COUNT,
} Register;
