LENGTH: .data 6,-9,15
prn #48
END: stop
K: .data 22
LOOP: jmp L1
STR: .string "abcdef"
L1: inc K
MAIN: mov r3, LENGTH
bne LOOP