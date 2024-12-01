
        ldx #$20
loop:
        txa
        jsr color
        txa
        jsr $FFEE
        inx
        cpx #$7F
        bne loop
        lda #$0D
        jsr $FFEE
        lda #$0A
        jsr $FFEE
        jsr resetcol
        brk
        brk

color:
        tay
        lda #$1B
        jsr $FFEE
        lda #$5B
        jsr $FFEE
        lda #$33
        jsr $FFEE
        clc
        tya
        and #$07
        adc #$30
        jsr $FFEE
        lda #$6D
        jsr $FFEE
        rts

resetcol:
        lda #$1B
        jsr $FFEE
        lda #$5B
        jsr $FFEE
        lda #$30
        jsr $FFEE
        lda #$6D
        jsr $FFEE
        rts

