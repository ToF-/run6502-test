
loop:
    jsr $FFCC
    cmp #$2E
    beq end
    clc
    cmp #$30
    bcc loop
    cmp #$3A
    bcs loop
    jsr $FFEE
    jmp loop
end:
    rts
