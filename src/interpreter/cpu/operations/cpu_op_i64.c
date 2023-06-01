#include "interpreter/cpu/cpu_op.h"

#include <assert.h>
#include <stdlib.h>

#include "interpreter/cpu/cpu_defines.h"
#include "interpreter/cpu/cpu_instructions.h"

#include "cpu_op_internals.h"

/******************
 *     Memory     *
 ******************/

/* Move */

CpuError cpu_op_i64_mov(Cpu *cpu, Register reg, int64_t value) {
  cpu->reg[reg].i64 = value;
  return CPU_ERR_OK;
}

CpuError cpu_op_i64_movr(Cpu *cpu, Register reg1, Register reg2) {
  cpu->reg[reg1].i64 = cpu->reg[reg2].i64;
  return CPU_ERR_OK;
}

CpuError cpu_op_i64_movs(Cpu *cpu, Register reg, size_t stack_offset) {
  if (cpu->rsp && cpu->rsp < stack_offset) {
    return CPU_ERR_STACK_UNDERFLOW;
  }
  cpu->reg[reg].i64 = *(int64_t *)&cpu->stack[stack_offset];
  return CPU_ERR_OK;
}

/* Push */

CpuError cpu_op_i64_push(Cpu *cpu, const int64_t value) {
  if (cpu->rsp >= CPU_STACK_SIZE) {
    return CPU_ERR_STACK_OVERFLOW;
  }
  *(int64_t *)&cpu->stack[cpu->rsp] = value;
  cpu->rsp += sizeof(int64_t);
  return CPU_ERR_OK;
}

CpuError cpu_op_i64_pushr(Cpu *cpu, const Register reg) {
  return cpu_op_i64_push(cpu, cpu->reg[reg].i64);
}

/* Stack operations */

CpuError cpu_op_i64_smov(Cpu *cpu, size_t offset, const int64_t value) {
  if (cpu->rsp && cpu->rsp < offset) {
    return CPU_ERR_STACK_UNDERFLOW;
  }
  ((Word *)&cpu->stack[cpu->rsb + offset])->i64 = value;
  return CPU_ERR_OK;
}

CpuError cpu_op_i64_smovr(Cpu *cpu, size_t offset, const Register reg) {
  return cpu_op_i64_smov(cpu, offset, cpu->reg[reg].i64);
}

CpuError cpu_op_i64_pop(Cpu *cpu, Register reg) {
  if (cpu->rsp == 0) {
    return CPU_ERR_STACK_UNDERFLOW;
  }
  cpu->rsp -= sizeof(int64_t);
  cpu->reg[reg].i64 = ((Word *)&cpu->stack[cpu->rsp])->i64;
  return CPU_ERR_OK;
}

/* Compare */

CpuError cpu_op_i64_cmp(Cpu *cpu, Register reg, int64_t value) {
  cpu->zf = (int64_t)(cpu->reg[reg].i64 - value);
  return CPU_ERR_OK;
}

CpuError cpu_op_i64_cmpr(Cpu *cpu, Register reg1, Register reg2) {
  return cpu_op_i64_cmp(cpu, reg1, cpu->reg[reg2].i64);
}

/******************
 *     Math       *
 ******************/

/* Addition */

CpuError cpu_op_i64_add(Cpu *cpu, const Register reg, const int64_t value) {
  cpu->reg[reg].i64 += value;
  return CPU_ERR_OK;
}

CpuError cpu_op_i64_addr(Cpu *cpu, const Register reg1, const Register reg2) {
  return cpu_op_i64_add(cpu, reg1, cpu->reg[reg2].i64);
}

/* Subtraction */

CpuError cpu_op_i64_sub(Cpu *cpu, const Register reg, const int64_t value) {
  cpu->reg[reg].i64 -= value;
  return CPU_ERR_OK;
}

CpuError cpu_op_i64_subr(Cpu *cpu, const Register reg1, const Register reg2) {
  return cpu_op_i64_sub(cpu, reg1, cpu->reg[reg2].i64);
}

/* Multiplication */

CpuError cpu_op_i64_mul(Cpu *cpu, const Register reg, const int64_t value) {
  cpu->reg[reg].i64 *= value;
  return CPU_ERR_OK;
}

CpuError cpu_op_i64_mulr(Cpu *cpu, const Register reg1, const Register reg2) {
  return cpu_op_i64_mul(cpu, reg1, cpu->reg[reg2].i64);
}

/* Division */

CpuError cpu_op_i64_div(Cpu *cpu, const Register reg, const int64_t value) {
  if (value == 0) {
    return CPU_ERR_ZERO_DIVISION;
  }
  cpu->reg[reg].i64 /= value;
  return CPU_ERR_OK;
}

CpuError cpu_op_i64_divr(Cpu *cpu, const Register reg1, const Register reg2) {
  return cpu_op_i64_div(cpu, reg1, cpu->reg[reg2].i64);
}
