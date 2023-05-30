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
  fprintf(output, "Usage: %s <file> [-r, -c, -d] [output_file]\n", program);
}

static void help(FILE *output, const char *program) {
  usage(output, program);
  fprintf(output, "<file>,   Input file\n");
  fprintf(output, "Optional\n");
  fprintf(output, "  Tools:\n");
  fprintf(output, "    -r,    Runs bytecode file (default)\n");
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
    usage_error(program, "Not enough arguments provided", "file");
    return 1;
  }

  const char *input_file = shift(&argc, &argv);
  if (input_file[0] == '-') {
    if (input_file[1] == 'h') {
      help(stdout, program);
      return 0;
    }
    usage_error(program, "Expected a <file> argument", input_file);
    return -1;
  }

  const char *option = shift(&argc, &argv);
  if (option == NULL) {
    option = "-r";
  }
  if (option[0] != '-') {
    usage_error(program, "Expected argument like: -c -f", option);
    return -1;
  }
  printf("%s %s\n", input_file, option);

  switch (option[1]) {
  case 'c': {
    const char *output_file = shift(&argc, &argv);
    if (!output_file) {
      usage_error(program, "Expected an output file, but none was provided",
                  "output");
    }
    return compile(input_file, output_file);
  }
  case 'r': {
    const char *output_file = shift(&argc, &argv);
    if (output_file) {
      if (compile(input_file, output_file) != 0) {
        return -1;
      }
      return alby_vm(output_file);
    }
    return alby_vm(input_file);
  }

  default:
    usage_error(program, "Option has to be one of these (-r, -c)", option);
    return -1;
  }

  return 0;
}
