    ldy #$00
loop:
    tya 
    clc
    adc #$20
    cmp #$7F
    bcs increment
    sta $0200,y
    sta $0300,y
    sta $0400,y
    sta $0500,y
    jsr $FFBB
increment:
    inc
    iny
    cpy #$80
    bne loop
    rts
