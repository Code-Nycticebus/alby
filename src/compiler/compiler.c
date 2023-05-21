#include "compiler.h"

// For 'mkstemp'
#define _GNU_SOURCE

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "interpreter/cpu/cpu_defines.h"
#include "lexer/lexer.h"
#include "parser.h"
#include "result.h"

#define BUFFER_SIZE 1024

int compile(const char *in_filename, const char *out_filename) {
  char buffer[BUFFER_SIZE] = {0};

  srand(time(NULL));
  char temp_name[5 + 8 + 1] = {0};
  sprintf(temp_name, "alby-%x", rand() * time(NULL));
  
  rename(out_filename, temp_name);
  FILE *input = fopen(in_filename, "rb");
  if (input == NULL) {
    fprintf(stderr, "Error: Could not open file '%s': %s\n", in_filename,
            strerror(errno));
    exit(1);
  }

  FILE *output = fopen(out_filename, "wb");
  if (output == NULL) {
    fprintf(stderr, "Error: Could not open file '%s': %s\n", out_filename,
            strerror(errno));
    exit(1);
  }

  // Writing binary
  fprintf(output, "#!./.build/bin/alby\n");

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
                    "Error: An error occured while writing the file: %s: %s",
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
    fprintf(stderr, "Error: An error occured while reading the file: %s: %s",
            in_filename, strerror(errno));
    goto ERROR;
  }

  fclose(input);
  fclose(output);
  remove(temp_name);
  return 0;

ERROR:
  rename(temp_name, out_filename);
  fclose(input);
  fclose(output);
  return 1;
}
