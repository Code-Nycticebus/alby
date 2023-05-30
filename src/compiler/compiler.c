#include "compiler.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interpreter/cpu/cpu.h"
#include "interpreter/cpu/cpu_defines.h"
#include "lexer/lexer.h"
#include "parser.h"
#include "result.h"

#define BUFFER_SIZE 1024

#if defined(_WIN32) || defined(_WIN64)
#define HASH_FORMAT "%llx"
#else
#define HASH_FORMAT "%lx"
#endif

static inline uint64_t hash(const char *str) {
  size_t len = strlen(str);
  uint64_t result = 1;
  for (uint64_t i = 0; i < len; ++i) {
    result *= len + str[i];
  }
  return result;
}

int compile(const char *in_filename, const char *out_filename) {
  char buffer[BUFFER_SIZE] = {0};

  char backup_filename[] = "alby-XXXXXXXXXXXXXXXX"; // Storage for temp filename
  snprintf(backup_filename, sizeof(backup_filename) - 1, "alby-" HASH_FORMAT,
           hash(out_filename));
  rename(out_filename, backup_filename);

  FILE *input = fopen(in_filename, "rb");
  FILE *output = fopen(out_filename, "wb");

  if (input == NULL) {
    fprintf(stderr, "Error: Could not open file '%s': %s\n", in_filename,
            strerror(errno));
    goto ERROR;
  }

  if (output == NULL) {
    fprintf(stderr, "Error: Could not open file '%s': %s\n", out_filename,
            strerror(errno));
    goto ERROR;
  }

  Cpu cpu = {0};
  fwrite(&cpu, sizeof(cpu), 1, output);

  size_t bytes_read;
  while ((bytes_read = fread(buffer, sizeof(char), BUFFER_SIZE, input))) {
    Parser parser = {
        .lexer = lexer_init(bytes_read, buffer),
        .filename = in_filename,
        .eof = false,
    };

    while (!parser.eof) {
      ParserResult res = parser_next(&parser);
      if (res.ok) {
        size_t bytes_written =
            fwrite(&res.value.ok, sizeof(CpuInstruction), 1, output);
        if (bytes_written < sizeof(CpuInstruction)) {
          if (ferror(input)) {
            fprintf(stderr,
                    "Error: An error occured while writing the file: '%s': %s",
                    out_filename, strerror(errno));
            goto ERROR;
          }
        }

      } else {
        parser_error_print(&res.value.err);
        goto ERROR;
      }
    }
  }

  if (ferror(input)) {
    fprintf(stderr, "Error: An error occured while reading the file: '%s': %s",
            in_filename, strerror(errno));
    goto ERROR;
  }

  remove(backup_filename); // Remove the old version of the bytecode file
  fclose(input);
  fclose(output);
  return 0;

ERROR:
  rename(backup_filename,
         out_filename); // Restore the backup file if parsing failed
  fclose(input);
  fclose(output);
  return 1;
}
