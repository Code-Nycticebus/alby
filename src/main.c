#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "compiler/compiler.h"
#include "interpreter/alby_vm.h"

static const char *shift(int *argc, const char ***argv) {
  const char *arg = **argv;
  if (*argc > 0) {
    *argv += 1;
    *argc -= 1;
  }
  return arg;
}

static void usage(FILE *output, const char *program) {
  fprintf(output, "Usage: %s <file> [-r, -c, -d] [output]", program);
  fprintf(output, "\n");
  fprintf(output, "<file>,   Input file\n");
  fprintf(output, "Optional\n");
  fprintf(output, "  Tools:\n");
  fprintf(output, "    -r,    Runs bytecode file (default) (NOT IMPL)\n");
  fprintf(output, "    -c,    Compiles into bytecode (NOT IMPLEMENTED)\n");
  fprintf(output, "    -d,    Disassembles bytecode (NOT IMPLEMENTED)\n");
  fprintf(output, "  output,  Specifies the output name (NOT IMPLEMENTED)\n");
}

static void usage_error(const char *program, const char *msg,
                        const char *flag) {
  usage(stderr, program);
  fprintf(stderr, "Error: There was a problem with the '%s' flag: %s\n", flag,
          msg);
  exit(1);
}

int main(int argc, const char **argv) {
  const char *program = shift(&argc, &argv);
  if (argc < 1) {
    usage_error(program, "Not enough arguments provided", "Tool");
  }

  const char *input_file = shift(&argc, &argv);
  printf("FILE: %s\n", input_file);
  const char *option = shift(&argc, &argv);
  if (option == NULL) {
    option = "-r";
  }
  if (option[0] != '-') {
    usage_error(program, "Expected a - argument", option);
  }

  switch (option[1]) {
  case 'c':
    return compile(input_file);
  case 'd':
    assert(0 && "NOT IMPLEMENTED");
    break;
  case 'r':
    return alby_vm(input_file);
  default:
    usage_error(program, "Option has to be one of these (-r, -c, -d)", option);
  }

  return 0;
}
