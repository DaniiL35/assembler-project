
test 1 last project 
MAIN:   mov  @r3, LENGTH
LOOP:   jmp  L1
		macr m1
		sub  @r1, @r4
		bne  END
		endmacr
		prn  -5
		bne  LOOP
		m1
L1:     inc  K
		bne  LOOP
END:    stop
STR:    .string "abcdef"
LENGTH: .data 6,-9,15
K:      .data 22







Test 2 (Current Project)
MAIN:   add  r3, LIST
LOOP:   prn  #48
        lea  STR, r6
        inc  r6
        mov  *r6, K
        sub  r1, r4
        cmp  r3, #-6
        bne  END
        dec  K
        jmp  LOOP
END:    stop
STR:    .string "abcd"
LIST:   .data 6, -9
.data  -100
K:      .data 31
