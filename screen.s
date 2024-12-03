    lda #$20
    ldy #$00
loop_row:
    ldx #$00
loop_col:
    jsr $FFBB
    jsr $FFB0
    inc
    cmp #$7F
    bne next
    lda #$20
next:
    inx
    cpx #$40
    bne loop_col
    iny
    cpy #$20
    bne loop_row
    rts
