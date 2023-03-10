#include "compiler.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FOR TESTING
#include "../interpreter/cpu/cpu_inst.h"

int compile(const char *filename) {
  FILE *input = fopen(filename, "rb");
  if (input == NULL) {
    fprintf(stderr, "Error: Could not open file '%s': %s\n", filename,
            strerror(errno));
    exit(1);
  }
  fclose(input);

  CpuInstruction program[] = {
      cpu_inst_debug(),
      cpu_inst_exit(0),
  };
  const size_t program_size = 2;

  FILE *output = fopen("main.lsd", "wb");
  if (output == NULL) {
    fprintf(stderr, "Error: Could not open file '%s': %s\n", "main.lsd",
            strerror(errno));
    exit(1);
  }

  fprintf(output, "#!./.build/bin/alby\n");
  (void)fwrite(program, sizeof(CpuInstruction), program_size, output);

  fclose(output);
  return 0;
}
