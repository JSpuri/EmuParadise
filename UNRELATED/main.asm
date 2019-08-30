;------------------------------------------------------------------------------
;.-. .-..-. .-..----. .----..-.     .--.  .---. .----..----. 
;| { } ||  `| || <3  }| {_  | |    / <3 \{_   _}| {_  | <3  \
;| {_} || |\  || .-. \| {__ | `--./  /\  \ | |  | {__ |     /
;`-----'`-' `-'`-' `-'`----'`----'`-'  `-' `-'  `----'`----' 
;
;made by EmuParadise group
;------------------------------------------------------------------------------


;------------------------------------------------------------------------------
; Constants
;------------------------------------------------------------------------------

;iNES Header Constants --------------------------------------------------------

PRG_COUNT = $01 ;PRG ROM size (1 -> 16KiB, 2 -> 32KiB)
CHR_COUNT = $01 ;CHR ROM size (1-> 8KiB, 2 -> 16KiB)

;Byte 6 - Flags

;first bit (%0000000[0])
MIRR = %00000000 ;mirroring mode (0 -> horizontal, 1 -> vertical)

;second bit (%000000[0]0)

BATT = %00000000 ;batteryi-backed PRG RAM (0 -> no, 1 -> yes)

;third bit (%00000[0]00)
TRNR = %00000000 ;trainer (0 -> no, 1 -> yes)

;fourth bit (%0000[0]000)
FSCN = %00000000 ;four screen mirroring mode (0 -> no, 1 -> yes)

;fifth to eighth bits (%[0000]0000)
MAPN = %00000000 ;mapper number (D0 to D3) (0 is NROM)

;Hardware info ----------------------------------------------------------------
JOYPAD1 = $4016
JOYPAD2 = $4017

BUTTON_A      = 1 << 7
BUTTON_B      = 1 << 6
BUTTON_SELECT = 1 << 5
BUTTON_START  = 1 << 4
BUTTON_UP     = 1 << 3
BUTTON_DOWN   = 1 << 2
BUTTON_LEFT   = 1 << 1
BUTTON_RIGHT  = 1 << 0

;Game info --------------------------------------------------------------------

;player max health
MAX_HEALTH EQU #$14

;heart sprite information
heart_init_pos_x EQU #$80
sprite_heart_n EQU #$00
heart_init_pos_y EQU #$80
heart_step_size EQU #$02     ;player speed

;player states ----------------------------------------------------------------
; _______________
;|A      8      9|
;|               |
;|2      0      1|
;|               |
;|6______4______5|
; ______   ______
;|F_____| |F_____|


FREE_MOV    EQU %00000000 ;0x00
MIN_X_MIN_Y EQU %00001010 ;0x0A
MIN_X       EQU %00000010 ;0x02
MIN_Y       EQU %00001000 ;0x08
MAX_X       EQU %00000001 ;0x01
MAX_Y       EQU %00000100 ;0x04
MIN_X_MAX_Y EQU %00000110 ;0x06
MAX_X_MIN_Y EQU %00001001 ;0x09
MAX_X_MAX_Y EQU %00000101 ;0x05
MENU_MODE   EQU %00001111 ;0x0F
;Box info ---------------------------------------------------------------------
;Conta começa do zero
;H - 32, V - 30

;Vertical
;Tile 14 -> Primeiro pixel (112)
;Tile 23 -> Ultimo pixel (191)

;Horizontal
;9 -> Primeiro Pixel (72)
;22 -> Ultimo pixel (183)

box_y0 EQU #$70
box_yf EQU #$BF

box_x0 EQU #$48
box_xf EQU #$B7

;------------------------------------------------------------------------------
; Variables - Stored in internal RAM [$0000,$0800) 
;------------------------------------------------------------------------------

    .enum $0010
beg_ram
buttons1        .dsb 1      ;used to read input from controller 1
last_buttons1   .dsb 1      ;last input read from controller 1

player_state        .dsb 1
player_cur_health   .dsb 1
player_max_health   .dsb 1     ;player health

box_border_y0   .dsb 1
box_border_yf   .dsb 1
box_border_x0   .dsb 1
box_border_xf   .dsb 1

num_oam         .dsb 1

    .ende

    .enum $0200

sprites

    .ende

;------------------------------------------------------------------------------
; iNES header 
;------------------------------------------------------------------------------

    .db "NES", $1a  ;identification of the iNES header -> "NES<EOF>"

    .db PRG_COUNT   ;number of PGR ROM banks (1 -> 16KiB)
    .db CHR_COUNT   ;number of CHR ROM banks (1 -> 8KiB)

    .db MAPN|FSCN|TRNR|BATT|MIRR   ;byte 6

    .dsb 9, $00     ;clear the remaining flags


;------------------------------------------------------------------------------
; Program Bank - The code goes here
;------------------------------------------------------------------------------

    .base $10000-(PRG_COUNT*$4000)   ;$C000 to $FFFA in ROM with 16KiB
                                    ;$8000 to $FFFF in ROM with 32KiB

Reset:

    SEI             ;Set Interrupt Disable
    CLD             ;Clear Decimal Mode
    LDX #$40
    STX $4017       ; disable APU frame IRQ
    LDX #$01FF
    TXS             ; Set up stack
    INX             ; now X = 0
    STX $2000       ; disable NMI
    STX $2001       ; disable rendering
    STX $4010       ; disable DMC IRQs
    

vblankwait1:        ; First wait for vblank to make sure PPU is ready
    BIT $2002
    BPL vblankwait1

clrmem:
    LDA #$00
    STA $0000, x
    STA $0100, x
    STA $0200, x
    STA $0400, x
    STA $0500, x
    STA $0600, x
    STA $0700, x
    LDA #$FE
    STA $0200, x
    INX
    BNE clrmem

LoadPalettes:
    LDA $2002       ; read PPU status to reset the high/low latch
    LDA #$3F
    STA $2006       ; write the high byte of $3F00 address
    LDA #$00
    STA $2006       ; write the low byte of $3F00 address
    LDX #$00

vblankwait2:        ; Second wait for vblank, PPU is ready after this
    BIT $2002
    BPL vblankwait2


LoadPalettesLoop:
    LDA palette, x          ;load palette byte
    STA $2007               ;write to PPU
    INX                     ;set index to next byte
    CPX #$20            
    BNE LoadPalettesLoop    ;if x = $20, 32 bytes copied, all done

LoadInitRam:
    LDX init_ram
    LDY #$01

load_init_ram_loop:
    LDA init_ram, y
    STA $10 - 1, y

    DEX
    BNE load_init_ram_loop

    LDX #$00
    LDY num_oam

load_heart_loop:
    LDA heart_sprite, x     ;load data from address (sprites + x)
    STA $0200, x            ;store into RAM address ($0400 + x)
    INX                     ;x = x + 1
    CPX #$10                ;check if all 16 bytes of heart were loaded
    BNE load_heart_loop


    LDA #%10000000  ; enable NMI, sprites from Pattern Table 0
    STA $2000
    LDA #%00010000  ;enable sprites
    STA $2001

Forever:

state_changer:
    JSR UpdateState     ;check position and update state

    JMP Forever     ;jump back to Forever, infinite loop


NMI:

    LDA #$00        ;set ram address to print sprites on PPU ($0200)
    STA $2003       ;low address
    LDA #$02
    STA $4014       ;high address (activates memory copy to PPU(?))
    JSR ReadJoy1    ;read data from controller

    LDA player_state    ;load current state
    ;CMP MENU_MODE       ;check if it's in menu mode
    ;BCC battle_turn     ;if not, it's in battle mode
                        ;menu move should go in here

battle_turn:
    JSR MvHeartBattle   ;move player

    RTI             ;Return from Interrupt


IRQ:

    ;NOTE: IRQ code goes here 

; Subroutines -----------------------------------------------------------------

; ---------------------- Joycontroller1 reading
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
    LDA JOYPAD1         ;read joypad 1 input bit by bit (?)
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

; ---------------------- Player State move-checker
CheckMoveRight:         ;check if player can move to the right
    LDX player_state    ;load player state to check impossible movements

    CPX MAX_X_MIN_Y     ;compare state
    BEQ move_left       ;jump to next possible movement

    CPX MAX_X           ;compare state
    BEQ move_left       ;jump to next possible movement

    CPX MAX_X_MAX_Y     ;compare state
    BEQ move_left       ;jump to next possible movement

    JMP right_movement  ;right movement is possible, jump to handle it

CheckMoveLeft:          ;same as CheckMoveRight
    LDX player_state

    CPX MIN_X_MIN_Y
    BEQ move_down

    CPX MIN_X
    BEQ move_down

    CPX MIN_X_MAX_Y
    BEQ move_down

    JMP left_movement

CheckMoveDown:          ;same as CheckMoveRight
    LDX player_state

    CPX MIN_X_MAX_Y
    BEQ move_up

    CPX MAX_Y
    BEQ move_up

    CPX MAX_X_MAX_Y
    BEQ move_up

    JMP down_movement

CheckMoveUp:            ;same as CheckMoveRight
    LDX player_state

    CPX MIN_X_MIN_Y
    BEQ move_done

    CPX MIN_Y
    BEQ move_done

    CPX MAX_X_MIN_Y
    BEQ move_done

    JMP up_movement

; ---------------------- Player Movement
MvHeartBattle:
    LDA buttons1        ;load pressed buttons byte

move_right:
    CLC
    ROR                 ;rotate right (last bit becomes Carry)
    BCC move_left       ;check if carry is set (button is pressed)

    ;JMP CheckMoveRight  ;check if right movement is possible
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

    ;JMP CheckMoveLeft
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

    ;JMP CheckMoveDown
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

    ;JMP CheckMoveUp
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

; ---------------------- Player State Updater
UpdateState:

    LDA #$0f

upper_limit:
    PHA
    LDA sprites
    
    SEC
    SBC heart_step_size
    CMP box_border_y0

    PLA
    ASL
    ADC #$00

lower_limit:
    PHA
    LDA box_border_yf

    SEC
    SBC heart_step_size + #$0F
    CMP sprites

    PLA
    ASL
    ADC #$00

left_limit:
    PHA
    LDA sprites + #$03

    SEC
    SBC heart_step_size
    CMP box_border_x0

    PLA
    ASL
    ADC #$00

right_limit:
    PHA
    LDA box_border_xf

    SEC
    SBC heart_step_size + #$0F
    CMP sprites + #$03

    PLA
    ASL
    ADC #$00

    EOR #$FF
    STA player_state

    RTS

; Init RAM --------------------------------------------------------------------
    .org $D000
init_ram:
    .db #$0A
    .db #$00, #$00
    .db FREE_MOV, MAX_HEALTH, MAX_HEALTH
    .db box_y0, box_yf, box_x0, box_xf
    .db #$04

; Graphics information --------------------------------------------------------
  .org $E000        ;palette set
palette:
    .db $0F,$31,$32,$33,$0F,$35,$36,$37,$0F,$39,$3A,$3B,$0F,$3D,$3E,$0F
    .db $0F,$1C,$15,$14,$0F,$02,$38,$3C,$0F,$1C,$15,$14,$0F,$02,$38,$3C

heart_sprite:
    .db heart_init_pos_x, sprite_heart_n, #$00, heart_init_pos_y
    .db heart_init_pos_x + 8, sprite_heart_n + 2, #$00, heart_init_pos_y
    .db heart_init_pos_x, sprite_heart_n + 1, #$00, heart_init_pos_y + 8
    .db heart_init_pos_x + 8, sprite_heart_n + 3, #$00, heart_init_pos_y + 8

;------------------------------------------------------------------------------
    ; Interrupt vectors
;------------------------------------------------------------------------------

    .org $fffa

    .dw NMI
    .dw Reset
    .dw IRQ

;------------------------------------------------------------------------------
; CHR-ROM bank
;------------------------------------------------------------------------------

    .incbin "mario.chr" ;placeholder graphics
