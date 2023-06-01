
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

CpuError cpu_op_i8_mov(Cpu *cpu, Register reg, int8_t value) {
  cpu->reg[reg].i8 = value;
  return CPU_ERR_OK;
}

CpuError cpu_op_i8_movr(Cpu *cpu, Register reg1, Register reg2) {
  cpu->reg[reg1].i8 = cpu->reg[reg2].i8;
  return CPU_ERR_OK;
}

CpuError cpu_op_i8_movs(Cpu *cpu, Register reg, size_t stack_offset) {
  if (cpu->rsp && cpu->rsp < stack_offset) {
    return CPU_ERR_STACK_UNDERFLOW;
  }
  cpu->reg[reg].i8 = *(int8_t *)&cpu->stack[stack_offset];
  return CPU_ERR_OK;
}

/* Push */

CpuError cpu_op_i8_push(Cpu *cpu, const int8_t value) {
  if (cpu->rsp >= CPU_STACK_SIZE) {
    return CPU_ERR_STACK_OVERFLOW;
  }
  *(int8_t *)&cpu->stack[cpu->rsp] = value;
  cpu->rsp += sizeof(int8_t);
  return CPU_ERR_OK;
}

CpuError cpu_op_i8_pushr(Cpu *cpu, const Register reg) {
  return cpu_op_i8_push(cpu, cpu->reg[reg].i64);
}

/* Stack operations */

CpuError cpu_op_i8_smov(Cpu *cpu, size_t offset, const int8_t value) {
  if (cpu->rsp && cpu->rsp < offset) {
    return CPU_ERR_STACK_UNDERFLOW;
  }
  ((Word *)&cpu->stack[cpu->rsb + offset])->i8 = value;
  return CPU_ERR_OK;
}

CpuError cpu_op_i8_smovr(Cpu *cpu, size_t offset, const Register reg) {
  return cpu_op_i8_smov(cpu, offset, cpu->reg[reg].i64);
}

CpuError cpu_op_i8_pop(Cpu *cpu, Register reg) {
  if (cpu->rsp == 0) {
    return CPU_ERR_STACK_UNDERFLOW;
  }
  cpu->rsp -= sizeof(int8_t);
  cpu->reg[reg].i8 = ((Word *)&cpu->stack[cpu->rsp])->i64;
  return CPU_ERR_OK;
}

/* Compare */

CpuError cpu_op_i8_cmp(Cpu *cpu, Register reg, int8_t value) {
  cpu->zf = (int8_t)(cpu->reg[reg].i8 - value);
  return CPU_ERR_OK;
}

CpuError cpu_op_i8_cmpr(Cpu *cpu, Register reg1, Register reg2) {
  return cpu_op_i8_cmp(cpu, reg1, cpu->reg[reg2].i64);
}

/******************
 *     Math       *
 ******************/

/* Addition */

CpuError cpu_op_i8_add(Cpu *cpu, const Register reg, const int8_t value) {
  cpu->reg[reg].i8 += value;
  return CPU_ERR_OK;
}

CpuError cpu_op_i8_addr(Cpu *cpu, const Register reg1, const Register reg2) {
  return cpu_op_i8_add(cpu, reg1, cpu->reg[reg2].i64);
}

/* Subtraction */

CpuError cpu_op_i8_sub(Cpu *cpu, const Register reg, const int8_t value) {
  cpu->reg[reg].i8 -= value;
  return CPU_ERR_OK;
}

CpuError cpu_op_i8_subr(Cpu *cpu, const Register reg1, const Register reg2) {
  return cpu_op_i8_sub(cpu, reg1, cpu->reg[reg2].i64);
}

/* Multiplication */

CpuError cpu_op_i8_mul(Cpu *cpu, const Register reg, const int8_t value) {
  cpu->reg[reg].i8 *= value;
  return CPU_ERR_OK;
}

CpuError cpu_op_i8_mulr(Cpu *cpu, const Register reg1, const Register reg2) {
  return cpu_op_i8_mul(cpu, reg1, cpu->reg[reg2].i64);
}

/* Division */

CpuError cpu_op_i8_div(Cpu *cpu, const Register reg, const int8_t value) {
  if (value == 0) {
    return CPU_ERR_ZERO_DIVISION;
  }
  cpu->reg[reg].i8 /= value;
  return CPU_ERR_OK;
}

CpuError cpu_op_i8_divr(Cpu *cpu, const Register reg1, const Register reg2) {
  return cpu_op_i8_div(cpu, reg1, cpu->reg[reg2].i64);
}
