#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef CPU_STACK_SIZE
#define CPU_STACK_SIZE 512
#endif // !CPU_STACK_SIZE

typedef uint64_t Word;
#define CPU_ARCHITECTURE (sizeof(Word) * 8)

typedef struct CpuInstruction CpuInstruction;

typedef enum CpuOp {
  CPU_OP_NOP,

  /* Memory */
  CPU_OP_MOVI,
  CPU_OP_MOVR,
  CPU_OP_MOVS,

  CPU_OP_PUSHI,
  CPU_OP_PUSHR,

  CPU_OP_SMOVI,
  CPU_OP_SMOVR,

  CPU_OP_POP,

  /* Program flow */
  CPU_OP_JMP,
  CPU_OP_JE,
  CPU_OP_JL,
  CPU_OP_JLE,
  CPU_OP_JG,
  CPU_OP_JGE,
  CPU_OP_CMPI,
  CPU_OP_CMPR,

  /* Math */
  CPU_OP_ADDI,
  CPU_OP_ADDR,

  CPU_OP_SUBI,
  CPU_OP_SUBR,

  CPU_OP_MULI,
  CPU_OP_MULR,

  CPU_OP_DIVI,
  CPU_OP_DIVR,

  /* Util */
  CPU_OP_SYSCALL,
  CPU_OP_DEBUG,

  CPU_OP_EXIT,
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
  CPU_RSP,
  CPU_RSB,
  CPU_R_COUNT,
} Register;
