#include "interpreter/cpu/cpu_op.h"

#include <assert.h>
#include <stdlib.h>

#include "interpreter/cpu/cpu_defines.h"
#include "interpreter/cpu/cpu_instructions.h"

#include "cpu_op_internals.h"

CpuError cpu_step(Cpu *cpu, const CpuInstruction *inst) {
  switch (inst->operation) {
  case CPU_OP_NOP:
    return CPU_ERR_OK;

  /*********************
   *      Generic      *
   *********************/

  /* Program flow */
  case CPU_OP_JMP:
    return cpu_op_jmp(cpu, inst->value1.size_t);
  case CPU_OP_JE:
    return cpu_op_je(cpu, inst->value1.size_t);
  case CPU_OP_JL:
    return cpu_op_jl(cpu, inst->value1.size_t);
  case CPU_OP_JLE:
    return cpu_op_jle(cpu, inst->value1.size_t);
  case CPU_OP_JG:
    return cpu_op_jg(cpu, inst->value1.size_t);
  case CPU_OP_JGE:
    return cpu_op_jge(cpu, inst->value1.size_t);

    /* Utils */

  case CPU_OP_SYSCALL:
    return cpu_op_syscall(cpu, inst->value1.i64);
  case CPU_OP_DEBUG:
    cpu_dump(stdout, cpu);
    return CPU_ERR_OK;

  case CPU_OP_EXIT:
    return cpu_op_exit(cpu, inst->value1);

  /*********************
   *        I64        *
   *********************/

  /* Memory */
  case CPU_OP_I64_MOV:
    return cpu_op_i64_mov(cpu, inst->value1.r, inst->value2.i64);
  case CPU_OP_I64_MOVR:
    return cpu_op_i64_movr(cpu, inst->value1.r, inst->value2.r);
  case CPU_OP_I64_MOVS:
    return cpu_op_i64_movs(cpu, inst->value1.r, inst->value2.size_t);
  case CPU_OP_I64_PUSHI:
    return cpu_op_i64_push(cpu, inst->value1.i64);
  case CPU_OP_I64_PUSHR:
    return cpu_op_i64_pushr(cpu, inst->value1.r);
  case CPU_OP_I64_SMOVI:
    return cpu_op_i64_smov(cpu, inst->value1.r, inst->value2.i64);
  case CPU_OP_I64_SMOVR:
    return cpu_op_i64_smovr(cpu, inst->value1.size_t, inst->value2.r);
  case CPU_OP_I64_POP:
    return cpu_op_i64_pop(cpu, inst->value1.r);

  case CPU_OP_I64_CMPI:
    return cpu_op_i64_cmp(cpu, inst->value1.r, inst->value2.i64);
  case CPU_OP_I64_CMPR:
    return cpu_op_i64_cmpr(cpu, inst->value1.r, inst->value2.r);

  /* Math */
  case CPU_OP_I64_ADDI:
    return cpu_op_i64_add(cpu, inst->value1.r, inst->value2.i64);
  case CPU_OP_I64_ADDR:
    return cpu_op_i64_addr(cpu, inst->value1.r, inst->value2.r);
  case CPU_OP_I64_SUBI:
    return cpu_op_i64_sub(cpu, inst->value1.r, inst->value2.i64);
  case CPU_OP_I64_SUBR:
    return cpu_op_i64_subr(cpu, inst->value1.r, inst->value2.r);
  case CPU_OP_I64_MULI:
    return cpu_op_i64_mul(cpu, inst->value1.r, inst->value2.i64);
  case CPU_OP_I64_MULR:
    return cpu_op_i64_mulr(cpu, inst->value1.r, inst->value2.r);
  case CPU_OP_I64_DIVI:
    return cpu_op_i64_div(cpu, inst->value1.r, inst->value2.i64);
  case CPU_OP_I64_DIVR:
    return cpu_op_i64_divr(cpu, inst->value1.r, inst->value2.r);

  /*********************
   *        I8        *
   *********************/

  /* Memory */
  case CPU_OP_I8_MOV:
    return cpu_op_i8_mov(cpu, inst->value1.r, inst->value2.i8);
  case CPU_OP_I8_MOVR:
    return cpu_op_i8_movr(cpu, inst->value1.r, inst->value2.r);
  case CPU_OP_I8_MOVS:
    return cpu_op_i8_movs(cpu, inst->value1.r, inst->value2.size_t);
  case CPU_OP_I8_PUSHI:
    return cpu_op_i8_push(cpu, inst->value1.i8);
  case CPU_OP_I8_PUSHR:
    return cpu_op_i8_pushr(cpu, inst->value1.r);
  case CPU_OP_I8_SMOVI:
    return cpu_op_i8_smov(cpu, inst->value1.r, inst->value2.i8);
  case CPU_OP_I8_SMOVR:
    return cpu_op_i8_smovr(cpu, inst->value1.i8, inst->value2.r);
  case CPU_OP_I8_POP:
    return cpu_op_i8_pop(cpu, inst->value1.r);

  case CPU_OP_I8_CMPI:
    return cpu_op_i8_cmp(cpu, inst->value1.r, inst->value2.i8);
  case CPU_OP_I8_CMPR:
    return cpu_op_i8_cmpr(cpu, inst->value1.r, inst->value2.r);

  /* Math */
  case CPU_OP_I8_ADDI:
    return cpu_op_i8_add(cpu, inst->value1.r, inst->value2.i8);
  case CPU_OP_I8_ADDR:
    return cpu_op_i8_addr(cpu, inst->value1.r, inst->value2.r);
  case CPU_OP_I8_SUBI:
    return cpu_op_i8_sub(cpu, inst->value1.r, inst->value2.i8);
  case CPU_OP_I8_SUBR:
    return cpu_op_i8_subr(cpu, inst->value1.r, inst->value2.r);
  case CPU_OP_I8_MULI:
    return cpu_op_i8_mul(cpu, inst->value1.r, inst->value2.i8);
  case CPU_OP_I8_MULR:
    return cpu_op_i8_mulr(cpu, inst->value1.r, inst->value2.r);
  case CPU_OP_I8_DIVI:
    return cpu_op_i8_div(cpu, inst->value1.r, inst->value2.i8);
  case CPU_OP_I8_DIVR:
    return cpu_op_i8_divr(cpu, inst->value1.r, inst->value2.r);
  }

  return CPU_ERR_INVALID_INSTRUCTION;
}

const char *cpu_err_to_cstr(const CpuError err) {
  switch (err) {
  case CPU_ERR_OK:
    return "No error";
  case CPU_ERR_INSTRUCTION_POINTER_OVERFLOW:
    return "Instruction Pointer overflow";
  case CPU_ERROR_EXIT:
    return "Exit";
  case CPU_ERR_INVALID_INSTRUCTION:
    return "Invalid instruction";
  case CPU_ERR_STACK_OVERFLOW:
    return "Stack overflow";
  case CPU_ERR_STACK_UNDERFLOW:
    return "Stack underflow";
  case CPU_ERR_ZERO_DIVISION:
    return "Division by zero";
  }
  assert(0 && "Unreachable!");
  return "Unkown Error";
}
