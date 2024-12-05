
buf = $10
rdch = $FFE0
wrch = $FFE2


    jsr reads
    jsr writes
    rts
reads
    lda #<s_buf
    sta buf
    lda #>s_buf
    sta buf+1
    ldy #$00
loop_reads
    jsr rdch
    cmp #$0A
    beq end_reads
    sta (buf),y
    iny
    bne loop_reads
end_reads
    sty s_len
    lda #$00
    sta s_pos
    rts
writes
    lda #<s_buf
    sta buf
    lda #>s_buf
    sta buf+1
    ldy #$00
loop_writes
    lda (buf),y
    jsr wrch
    iny
    cpy s_len
    bne loop_writes
end_writes
    rts
s_len
    .byte 0
s_pos
    .byte 0
s_buf
    .dsb 255,0
