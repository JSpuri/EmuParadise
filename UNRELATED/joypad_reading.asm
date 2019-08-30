;Hardware info ----------------------------------------------------------------
JOYPAD1 = $4016
JOYPAD2 = $4017

ReadJoy1:
    LDA buttons1
    STA last_buttons1   ;stores the last input for directional safety
    LDA #$01
    STA JOYPAD1
    STA buttons1
    LSR a
    STA JOYPAD1

    CLC
    LDX #$00
loop_RJ1:
    LDA JOYPAD1         ;read joypad 1 input bit by bit
    LSR a
    ROL buttons1        ;and store in buttons 1
    INX
    CPX #$08
    BNE loop_RJ1

    LDA buttons1
    AND #%00001010      ;compare up and left ...
    LSR a
    AND buttons1        ;... to down and right
    BEQ not_updown      
    
    LDA buttons1        ;if there are opposite directions, use last input
    EOR last_buttons1
    AND #%11110000
    EOR last_buttons1
    STA buttons1

not_updown:
    RTS
