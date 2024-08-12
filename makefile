# Compilation macros
CC = gcc
CFLAGS = -ansi -Wall -pedantic -g -IHeader -ISourceFiles # Include Header and SourceFiles directories for headers
GLOBAL_DEPS = Header/globals.h # Dependencies for everything
EXE_DEPS = SourceFiles/assembler.o SourceFiles/preprocessor.o SourceFiles/validation.o SourceFiles/reordering.o SourceFiles/utils.o # Deps for exe

# Executable
assembler: $(EXE_DEPS) $(GLOBAL_DEPS)
	$(CC) $(EXE_DEPS) $(CFLAGS) -o $@

# Object files
SourceFiles/assembler.o: SourceFiles/assembler.c Header/globals.h
	$(CC) -c SourceFiles/assembler.c $(CFLAGS) -o $@

SourceFiles/preprocessor.o: SourceFiles/preprocessor.c Header/globals.h
	$(CC) -c SourceFiles/preprocessor.c $(CFLAGS) -o $@

SourceFiles/validation.o: SourceFiles/validation.c Header/globals.h
	$(CC) -c SourceFiles/validation.c $(CFLAGS) -o $@

SourceFiles/reordering.o: SourceFiles/reordering.c Header/globals.h
	$(CC) -c SourceFiles/reordering.c $(CFLAGS) -o $@

SourceFiles/utils.o: SourceFiles/utils.c $(GLOBAL_DEPS)
	$(CC) -c SourceFiles/utils.c $(CFLAGS) -o $@	

# Clean up
clean:
	rm -rf SourceFiles/*.o assembler
