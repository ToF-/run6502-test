echo:
    ldy #$00
input:
    jsr $FFCC
    cmp #$0A
    beq end_input
    sta buffer,y
    iny
    bpl input
end_input:
    cpy #$00
    beq end_prog
    sty length
    ldy #$00
print:
    cpy length
    beq end_print
    lda buffer,y
    jsr $FFEE
    iny
    jmp print
end_print:
    lda #$0D
    jsr $FFEE
    lda #$0A
    jsr $FFEE
    ldy length
    bne echo
end_prog:
    rts

buffer

.dsb 255,0 

length

.byte 0

