# Compilation macros
CC = gcc
CFLAGS = -ansi -Wall -pedantic -g -Iinclude # Flags
#GLOBAL_DEPS = include/globals.h # Dependencies for everything
EXE_DEPS = assembler.o preprocessor.o validation.o  # Deps for exe

## Executable
assembler: $(EXE_DEPS)
    $(CC) -g $(EXE_DEPS) $(CFLAGS) -o $@

assembler.o: src/assembler.c include/assembler.h $(EXE_DEPS)
    $(CC) -c src/assembler.c $(CFLAGS) -o $@

preprocessor.o: src/preprocessor.c include/preprocessor.h $(EXE_DEPS)
    $(CC) -c src/preprocessor.c $(CFLAGS) -o $@

validation.o: src/validation.c include/validation.h $(EXE_DEPS)
    $(CC) -c src/validation.c $(CFLAGS) -o $@


clean:
    rm -rf *.o assembler