#include "cpu_op.h"

#include <assert.h>
#include <stdlib.h>

#include "cpu.h"
#include "cpu_defines.h"
#include "cpu_inst.h"

/* Internal definitions */

/* Memory */

static inline CpuError cpu_op_i64_mov(Cpu *cpu, Register reg, int64_t value) {
  cpu->reg[reg].i64 = value;
  return CPU_ERR_OK;
}

static inline CpuError cpu_op_i64_movr(Cpu *cpu, Register reg1, Register reg2) {
  cpu->reg[reg1] = cpu->reg[reg2];
  return CPU_ERR_OK;
}

static inline CpuError cpu_op_i64_movs(Cpu *cpu, Register reg,
                                       size_t stack_offset) {
  if (cpu->rsp && cpu->rsp < stack_offset) {
    return CPU_ERR_STACK_UNDERFLOW;
  }
  cpu->reg[reg] = *(Word *)&cpu->stack[stack_offset];
  return CPU_ERR_OK;
}

static inline CpuError cpu_op_i64_push(Cpu *cpu, const int64_t value) {
  if (cpu->rsp >= CPU_STACK_SIZE) {
    return CPU_ERR_STACK_OVERFLOW;
  }
  *(int64_t *)&cpu->stack[cpu->rsp] = value;
  cpu->rsp += sizeof(int64_t);
  return CPU_ERR_OK;
}

static inline CpuError cpu_op_i64_pushr(Cpu *cpu, const Register reg) {
  return cpu_op_i64_push(cpu, cpu->reg[reg].i64);
}

static inline CpuError cpu_op_i64_smov(Cpu *cpu, size_t offset,
                                       const int64_t value) {
  if (cpu->rsp && cpu->rsp < offset) {
    return CPU_ERR_STACK_UNDERFLOW;
  }
  ((Word *)&cpu->stack[cpu->rsb + offset])->i64 = value;
  return CPU_ERR_OK;
}

static inline CpuError cpu_op_i64_smovr(Cpu *cpu, size_t offset,
                                        const Register reg) {
  return cpu_op_i64_smov(cpu, offset, cpu->reg[reg].i64);
}

static inline CpuError cpu_op_i64_pop(Cpu *cpu, Register reg) {
  if (cpu->rsp == 0) {
    return CPU_ERR_STACK_UNDERFLOW;
  }
  cpu->rsp -= sizeof(int64_t);
  cpu->reg[reg].i64 = ((Word *)&cpu->stack[cpu->rsp])->i64;
  return CPU_ERR_OK;
}

/* Program flow */

static inline CpuError cpu_op_jmp(Cpu *cpu, size_t ptr) {
  cpu->ip = ptr;
  return CPU_ERR_OK;
}

static inline CpuError cpu_op_je(Cpu *cpu, size_t ptr) {
  if (cpu->zf == 0) {
    cpu->ip = ptr;
  }
  return CPU_ERR_OK;
}

static inline CpuError cpu_op_jl(Cpu *cpu, size_t ptr) {
  if (cpu->zf < 0) {
    cpu->ip = ptr;
  }
  return CPU_ERR_OK;
}
static inline CpuError cpu_op_jle(Cpu *cpu, size_t ptr) {
  if (cpu->zf <= 0) {
    cpu->ip = ptr;
  }
  return CPU_ERR_OK;
}

static inline CpuError cpu_op_jg(Cpu *cpu, size_t ptr) {
  if (0 < cpu->zf) {
    cpu->ip = ptr;
  }
  return CPU_ERR_OK;
}
static inline CpuError cpu_op_jge(Cpu *cpu, size_t ptr) {
  if (0 <= cpu->zf) {
    cpu->ip = ptr;
  }
  return CPU_ERR_OK;
}

static inline CpuError cpu_op_i64_cmp(Cpu *cpu, Register reg, int64_t value) {
  cpu->zf = (int64_t)(cpu->reg[reg].i64 - value);
  return CPU_ERR_OK;
}

static inline CpuError cpu_op_i64_cmpr(Cpu *cpu, Register reg1, Register reg2) {
  return cpu_op_i64_cmp(cpu, reg1, cpu->reg[reg2].i64);
}

/* Math */

static inline CpuError cpu_op_i64_add(Cpu *cpu, const Register reg,
                                      const int64_t value) {
  cpu->reg[reg].i64 += value;
  return CPU_ERR_OK;
}

static inline CpuError cpu_op_i64_addr(Cpu *cpu, const Register reg1,
                                       const Register reg2) {
  return cpu_op_i64_add(cpu, reg1, cpu->reg[reg2].i64);
}

static inline CpuError cpu_op_i64_sub(Cpu *cpu, const Register reg,
                                      const int64_t value) {
  cpu->reg[reg].i64 -= value;
  return CPU_ERR_OK;
}

static inline CpuError cpu_op_i64_subr(Cpu *cpu, const Register reg1,
                                       const Register reg2) {
  return cpu_op_i64_sub(cpu, reg1, cpu->reg[reg2].i64);
}

static inline CpuError cpu_op_i64_mul(Cpu *cpu, const Register reg,
                                      const int64_t value) {
  cpu->reg[reg].i64 *= value;
  return CPU_ERR_OK;
}

static inline CpuError cpu_op_i64_mulr(Cpu *cpu, const Register reg1,
                                       const Register reg2) {
  return cpu_op_i64_mul(cpu, reg1, cpu->reg[reg2].i64);
}

static inline CpuError cpu_op_i64_div(Cpu *cpu, const Register reg,
                                      const int64_t value) {
  if (value == 0) {
    return CPU_ERR_ZERO_DIVISION;
  }
  cpu->reg[reg].i64 /= value;
  return CPU_ERR_OK;
}

static inline CpuError cpu_op_i64_divr(Cpu *cpu, const Register reg1,
                                       const Register reg2) {
  return cpu_op_i64_div(cpu, reg1, cpu->reg[reg2].i64);
}

/* Utils */

static inline CpuError cpu_op_syscall(Cpu *cpu, const int64_t syscall) {
  (void)cpu;
  switch (syscall) {
  default:
    break;
  }
  assert(0 && "Unhandled Syscall");
  return CPU_ERR_INVALID_INSTRUCTION;
}

static inline CpuError cpu_op_exit(Cpu *cpu, const Word code) {
  cpu->reg[CPU_R1] = code;
  return CPU_ERROR_EXIT;
}

/* !Internal definitions */

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

/* ! Internal definitions */
