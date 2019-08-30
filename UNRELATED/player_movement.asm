;Box info ---------------------------------------------------------------------
box_ymin EQU #$64   ;box upper limit
box_ymax EQU #$B9   ;box lower limit

box_xmin EQU #$46   ;box left limit
box_xmax EQU #$BA   ;box right limit


MvHeartBattle:
    LDA buttons1        ;load pressed buttons byte

move_right:
    CLC
    ROR                 ;rotate right (last bit becomes Carry)
    BCC move_left       ;check if carry is set (button is pressed)

CheckMoveRight:                 ;check if player can move to the right
    PHA
    LDA sprites + 3             ;loads X position of first sprite
    CLC
    ADC #$10 + heart_step_size
    CLC
    CMP box_xmax                ;verifies if new position will pass border
    PLA
    BCC right_movement          ;move
    JMP move_left

right_movement:
    PHA                 ;push pressed buttons to stack

    LDX #$00
right_movement_loop:
    LDA sprites + 3, x  ;load x position of heart
    CLC
    ADC heart_step_size ;add position in x axis
    STA sprites + 3, x
    TAY
    TXA
    CLC
    ADC #$04
    TAX
    TYA
    CPX #$10
    BNE right_movement_loop
    PLA                 ;pull pressed buttons from stack


move_left:              ;same process as move_right
    CLC
    ROR
    BCC move_down

CheckMoveLeft:
    PHA
    LDA sprites + 3
    SEC
    SBC #$01 + heart_step_size
    CLC
    CMP box_xmin
    PLA
    BCC move_down
    JMP left_movement

left_movement:
    PHA

    LDX #$00
left_movement_loop:
    LDA sprites + 3, x
    SEC
    SBC heart_step_size
    STA sprites + 3, x
    TAY
    TXA
    CLC
    ADC #$04
    TAX
    TYA
    CPX #$10
    BNE left_movement_loop
    PLA

move_down:              ;same process as move_right
    CLC
    ROR
    BCC move_up

CheckMoveDown:
    PHA
    LDA sprites
    CLC
    ADC #$10 + heart_step_size
    CLC
    CMP box_ymax
    PLA
    BCC down_movement
    JMP move_up

down_movement:
    PHA
    LDX #$00
down_movement_loop:
    LDA sprites, x
    CLC
    ADC heart_step_size
    STA sprites, x
    TAY
    TXA
    CLC
    ADC #$04
    TAX
    TYA
    CPX #$10
    BNE down_movement_loop
    PLA

move_up:                ;same process as move_right
    CLC
    ROR
    BCC move_done

CheckMoveUp:
    PHA
    LDA sprites
    SEC
    SBC #$01 + heart_step_size
    CLC
    CMP box_ymin
    PLA
    BCC move_done
    JMP up_movement

up_movement:
    PHA
    LDX #$00
up_movement_loop:
    LDA sprites, x
    SEC
    SBC heart_step_size
    STA sprites, x
    TAY
    TXA
    CLC
    ADC #$04
    TAX
    TYA
    CPX #$10
    BNE up_movement_loop
    PLA

move_done:
    RTS

