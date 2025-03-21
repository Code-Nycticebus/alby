#include "alby_vm.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu/cpu.h"
#include "cpu/cpu_instructions.h"

int alby_vm(const char *filename) {
  FILE *input = fopen(filename, "rb");
  if (input == NULL) {
    fprintf(stderr, "Error: Could not open file '%s': %s\n", filename,
            strerror(errno));
    exit(1);
  }

  Cpu cpu = {0};
  fread(&cpu, sizeof(Cpu), 1, input);

  fseek(input, 0, SEEK_END);
  const size_t file_size = (size_t)ftell(input) - sizeof(Cpu);
  fseek(input, sizeof(Cpu), SEEK_SET);

  assert(file_size % sizeof(CpuInstruction) == 0); // TODO: real runtime checks
  const size_t program_size = file_size / sizeof(CpuInstruction);
  CpuInstruction program[program_size];

  size_t read_size =
      fread(program, sizeof(CpuInstruction), program_size, input);
  if (program_size != read_size) {
    fprintf(stderr,
            "Error: bytes read is not same as the filesize: %ld -> %ld\n",
            program_size, read_size); // TODO figure
                                      // out way of
                                      // getting error
    exit(1);
  }

  fclose(input);

  return cpu_run_program(&cpu, program, program_size);
}
