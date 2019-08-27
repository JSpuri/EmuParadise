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

;heart sprite information
heart_init_pos_x EQU #$80
sprite_heart_n EQU #$00
heart_init_pos_y EQU #$80
heart_step_size EQU #$02     ;player speed

;Box info ---------------------------------------------------------------------
;Conta começa do zero
;Tile 14 -> Primeiro pixel
;Tile 23 -> Ultimo pixel
;Horizontal
;9 -> Primeiro Pixel
;22 -> Ultimo pixel

;------------------------------------------------------------------------------
; Variables - Stored in internal RAM [$0000,$0800) 
;------------------------------------------------------------------------------

    .enum $0000

buttons1        .dsb 1      ;used to read input from controller 1
last_buttons1   .dsb 1      ;last input read from controller 1
    .ende

    .enum $0100

;States: 
;   0 - Free movement on axis
;   1 - Cannot add value on axis
;   2 - Cannot subtract value on axis
;   3 - Menu mode
heart_state_x   .db $0      ;heart state x
heart_state_y   .db $0      ;heart state y

player_health   .db $14     ;player health

    .ende

    .enum $0200
heart_y         .dsb 1
    .ende

    .enum $0203
heart_x         .dsb 1
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

    .org $10000-(PRG_COUNT*$4000)   ;$C000 to $FFFA in ROM with 16KiB
                                    ;$8000 to $FFFF in ROM with 32KiB

Reset:

    SEI             ;Set Interrupt Disable
    CLD             ;Clear Decimal Mode
    LDX #$40
    STX $4017       ; disable APU frame IRQ
    LDX #$FF
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
    LDA $2002    ; read PPU status to reset the high/low latch
    LDA #$3F
    STA $2006    ; write the high byte of $3F00 address
    LDA #$00
    STA $2006    ; write the low byte of $3F00 address
    LDX #$00

vblankwait2:      ; Second wait for vblank, PPU is ready after this
    BIT $2002
    BPL vblankwait2


LoadPalettesLoop:
    LDA palette, x          ;load palette byte
    STA $2007               ;write to PPU
    INX                     ;set index to next byte
    CPX #$20            
    BNE LoadPalettesLoop    ;if x = $20, 32 bytes copied, all done


    LDX #$00                ;start at zero
load_heart:
    LDA heart_sprite, x     ;load data from address (sprites + x)
    STA $0200, x            ;store into RAM address ($0200 + x)
    INX                     ;x = x + 1
    CPX #$04                ;check if all 4 bytes of heart were loaded
    BNE load_heart


    LDA #%10000000  ; enable NMI, sprites from Pattern Table 0
    STA $2000
    LDA #%00010000  ;enable sprites
    STA $2001

Forever:
    LDY last_buttons1
    JMP Forever     ;jump back to Forever, infinite loop


NMI:

    LDA #$00        ;set ram address to print sprites on PPU ($0200)
    STA $2003       ;low address
    LDA #$02
    STA $4014       ;high address

    JSR ReadJoy1    ;read data from controller

    JSR MoveHeart   ;move player

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

; --------------------- Player Movement
MoveHeart:
    LDA buttons1
move_right:
    CLC
    ROR
    BCC move_left
    PHA
    LDA heart_x
    CLC
    ADC heart_step_size
    STA heart_x
    PLA

move_left:
    CLC
    ROR
    BCC move_down
    PHA
    LDA heart_x
    SEC
    SBC heart_step_size
    STA heart_x
    PLA

move_down:
    CLC
    ROR
    BCC move_up
    PHA
    LDA heart_y
    CLC
    ADC heart_step_size
    STA heart_y
    PLA

move_up:
    CLC
    ROR
    BCC move_done
    PHA
    LDA heart_y
    SEC
    SBC heart_step_size
    STA heart_y
    PLA

move_done:
    RTS



; Graphics information --------------------------------------------------------
  .org $E000        ;palette set
palette:
  .db $0F,$31,$32,$33,$0F,$35,$36,$37,$0F,$39,$3A,$3B,$0F,$3D,$3E,$0F
  .db $0F,$1C,$15,$14,$0F,$02,$38,$3C,$0F,$1C,$15,$14,$0F,$02,$38,$3C

heart_sprite:
    .db heart_init_pos_x, sprite_heart_n, $00, heart_init_pos_y

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
