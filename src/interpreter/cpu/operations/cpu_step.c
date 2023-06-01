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

    /* Memory */
  case CPU_OP_MOVI:
    return cpu_op_i64_mov(cpu, inst->value1.r, inst->value2.i64);
  case CPU_OP_MOVR:
    return cpu_op_i64_movr(cpu, inst->value1.r, inst->value2.u64);
  case CPU_OP_MOVS:
    return cpu_op_i64_movs(cpu, inst->value1.r, inst->value2.size_t);
  case CPU_OP_PUSHI:
    return cpu_op_i64_push(cpu, inst->value1.i64);
  case CPU_OP_PUSHR:
    return cpu_op_i64_pushr(cpu, inst->value1.r);
  case CPU_OP_SMOVI:
    return cpu_op_i64_smov(cpu, inst->value1.r, inst->value2.i64);
  case CPU_OP_SMOVR:
    return cpu_op_i64_smovr(cpu, inst->value1.u64, inst->value2.r);
  case CPU_OP_POP:
    return cpu_op_i64_pop(cpu, inst->value1.r);

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
  case CPU_OP_CMPI:
    return cpu_op_i64_cmp(cpu, inst->value1.r, inst->value2.i64);
  case CPU_OP_CMPR:
    return cpu_op_i64_cmpr(cpu, inst->value1.r, inst->value2.r);

  /* Math */
  case CPU_OP_ADDI:
    return cpu_op_i64_add(cpu, inst->value1.r, inst->value2.i64);
  case CPU_OP_ADDR:
    return cpu_op_i64_addr(cpu, inst->value1.r, inst->value2.r);
  case CPU_OP_SUBI:
    return cpu_op_i64_sub(cpu, inst->value1.r, inst->value2.i64);
  case CPU_OP_SUBR:
    return cpu_op_i64_subr(cpu, inst->value1.r, inst->value2.r);
  case CPU_OP_MULI:
    return cpu_op_i64_mul(cpu, inst->value1.r, inst->value2.i64);
  case CPU_OP_MULR:
    return cpu_op_i64_mulr(cpu, inst->value1.r, inst->value2.r);
  case CPU_OP_DIVI:
    return cpu_op_i64_div(cpu, inst->value1.r, inst->value2.i64);
  case CPU_OP_DIVR:
    return cpu_op_i64_divr(cpu, inst->value1.r, inst->value2.r);

    /* Utils */

  case CPU_OP_SYSCALL:
    return cpu_op_syscall(cpu, inst->value1.i64);
  case CPU_OP_DEBUG:
    cpu_dump(stdout, cpu);
    return CPU_ERR_OK;

  case CPU_OP_EXIT:
    return cpu_op_exit(cpu, inst->value1);
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
