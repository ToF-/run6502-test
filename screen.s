    lda #$00
    ldy #$00
loop_row:
    ldx #$00
loop_col:
    pha
    cmp #$20
    bcs print
    lda #$2E
print:
    jsr $FFBB
    pla
    pha
    tya
    jsr $FFB0
    pla
    inc
    cmp #$7F
    bne next
    lda #$00
next:
    inx
    cpx #$20
    bne loop_col
    iny
    cpy #$20
    bne loop_row
    rts
