#include "cpu_op.h"

#include <assert.h>
#include <stdlib.h>

#include "cpu.h"
#include "cpu_defines.h"
#include "cpu_inst.h"

/* Internal definitions */

/* Memory */

static inline CpuError cpu_op_movi(Cpu *cpu, Register reg, Word value) {
  cpu->reg[reg] = value;
  return CPU_ERR_OK;
}

static inline CpuError cpu_op_movr(Cpu *cpu, Register reg1, Register reg2) {
  cpu->reg[reg1] = cpu->reg[reg2];
  return CPU_ERR_OK;
}

static inline CpuError cpu_op_movs(Cpu *cpu, Register reg,
                                   size_t stack_offset) {
  if (cpu->rsp && cpu->rsp < stack_offset) {
    return CPU_ERR_STACK_UNDERFLOW;
  }
  cpu->reg[reg] = cpu->stack[stack_offset];
  return CPU_ERR_OK;
}

static inline CpuError cpu_op_pushi(Cpu *cpu, const Word value) {
  if (cpu->rsp >= CPU_STACK_SIZE) {
    return CPU_ERR_STACK_OVERFLOW;
  }
  *(Word *)&cpu->stack[cpu->rsp] = value;
  cpu->rsp += sizeof(Word);
  return CPU_ERR_OK;
}

static inline CpuError cpu_op_pushr(Cpu *cpu, const Register reg) {
  return cpu_op_pushi(cpu, cpu->reg[reg]);
}

static inline CpuError cpu_op_smovi(Cpu *cpu, size_t offset, const Word value) {
  if (cpu->rsp && cpu->rsp < offset) {
    return CPU_ERR_STACK_UNDERFLOW;
  }
  *(Word *)&cpu->stack[offset] = value;
  return CPU_ERR_OK;
}

static inline CpuError cpu_op_smovr(Cpu *cpu, size_t offset,
                                    const Register reg) {
  return cpu_op_smovi(cpu, offset, cpu->reg[reg]);
}

static inline CpuError cpu_op_pop(Cpu *cpu, Register reg) {
  if (cpu->rsp == 0) {
    return CPU_ERR_STACK_UNDERFLOW;
  }
  cpu->reg[reg] = cpu->stack[--cpu->rsp];
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

static inline CpuError cpu_op_cmpi(Cpu *cpu, Register reg, Word value) {
  cpu->zf = (int64_t)(cpu->reg[reg] - value);
  return CPU_ERR_OK;
}

static inline CpuError cpu_op_cmpr(Cpu *cpu, Register reg1, Register reg2) {
  return cpu_op_cmpi(cpu, reg1, cpu->reg[reg2]);
}

/* Math */

static inline CpuError cpu_op_addi(Cpu *cpu, const Register reg,
                                   const Word value) {
  cpu->reg[reg] += value;
  return CPU_ERR_OK;
}

static inline CpuError cpu_op_addr(Cpu *cpu, const Register reg1,
                                   const Register reg2) {
  return cpu_op_addi(cpu, reg1, cpu->reg[reg2]);
}

static inline CpuError cpu_op_subi(Cpu *cpu, const Register reg,
                                   const Word value) {
  cpu->reg[reg] -= value;
  return CPU_ERR_OK;
}

static inline CpuError cpu_op_subr(Cpu *cpu, const Register reg1,
                                   const Register reg2) {
  return cpu_op_subi(cpu, reg1, cpu->reg[reg2]);
}

static inline CpuError cpu_op_muli(Cpu *cpu, const Register reg,
                                   const Word value) {
  cpu->reg[reg] *= value;
  return CPU_ERR_OK;
}

static inline CpuError cpu_op_mulr(Cpu *cpu, const Register reg1,
                                   const Register reg2) {
  return cpu_op_muli(cpu, reg1, cpu->reg[reg2]);
}

static inline CpuError cpu_op_divi(Cpu *cpu, const Register reg,
                                   const Word value) {
  if (value == 0) {
    return CPU_ERR_ZERO_DIVISION;
  }
  cpu->reg[reg] /= value;
  return CPU_ERR_OK;
}

static inline CpuError cpu_op_divr(Cpu *cpu, const Register reg1,
                                   const Register reg2) {
  return cpu_op_divi(cpu, reg1, cpu->reg[reg2]);
}

/* Utils */

static inline CpuError cpu_op_syscall(Cpu *cpu, const Word syscall) {
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
    return cpu_op_movi(cpu, inst->value1, inst->value2);
  case CPU_OP_MOVR:
    return cpu_op_movr(cpu, inst->value1, inst->value2);
  case CPU_OP_MOVS:
    return cpu_op_movs(cpu, inst->value1, inst->value2);
  case CPU_OP_PUSHI:
    return cpu_op_pushi(cpu, inst->value1);
  case CPU_OP_PUSHR:
    return cpu_op_pushr(cpu, inst->value1);
  case CPU_OP_SMOVI:
    return cpu_op_smovi(cpu, inst->value1, inst->value2);
  case CPU_OP_SMOVR:
    return cpu_op_smovr(cpu, inst->value1, inst->value2);
  case CPU_OP_POP:
    return cpu_op_pop(cpu, inst->value1);

    /* Program flow */
  case CPU_OP_JMP:
    return cpu_op_jmp(cpu, inst->value1);
  case CPU_OP_JE:
    return cpu_op_je(cpu, inst->value1);
  case CPU_OP_JL:
    return cpu_op_jl(cpu, inst->value1);
  case CPU_OP_JLE:
    return cpu_op_jle(cpu, inst->value1);
  case CPU_OP_JG:
    return cpu_op_jg(cpu, inst->value1);
  case CPU_OP_JGE:
    return cpu_op_jge(cpu, inst->value1);
  case CPU_OP_CMPI:
    return cpu_op_cmpi(cpu, inst->value1, inst->value2);
  case CPU_OP_CMPR:
    return cpu_op_cmpr(cpu, inst->value1, inst->value2);

  /* Math */
  case CPU_OP_ADDI:
    return cpu_op_addi(cpu, inst->value1, inst->value2);
  case CPU_OP_ADDR:
    return cpu_op_addr(cpu, inst->value1, inst->value2);
  case CPU_OP_SUBI:
    return cpu_op_subi(cpu, inst->value1, inst->value2);
  case CPU_OP_SUBR:
    return cpu_op_subr(cpu, inst->value1, inst->value2);
  case CPU_OP_MULI:
    return cpu_op_muli(cpu, inst->value1, inst->value2);
  case CPU_OP_MULR:
    return cpu_op_mulr(cpu, inst->value1, inst->value2);
  case CPU_OP_DIVI:
    return cpu_op_divi(cpu, inst->value1, inst->value2);
  case CPU_OP_DIVR:
    return cpu_op_divr(cpu, inst->value1, inst->value2);

    /* Utils */

  case CPU_OP_SYSCALL:
    return cpu_op_syscall(cpu, inst->value1);
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
