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

;Game info --------------------------------------------------------------------

;player max health
MAX_HEALTH EQU #$14

;heart sprite information
heart_init_pos_x EQU #$80
sprite_heart_n = $00
heart_init_pos_y EQU #$80
heart_step_size EQU #$02     ;player speed

;------------------------------------------------------------------------------
; Variables - Stored in internal RAM [$0000,$0800) 
;------------------------------------------------------------------------------

    .enum $0010
buttons1        .db 0      ;used to read input from controller 1
last_buttons1   .db 0      ;last input read from controller 1

player_cur_health   .db MAX_HEALTH
player_max_health   .db MAX_HEALTH    ;player health

num_oam         .db #$04

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

    LDX #$00
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
    JMP Forever     ;jump back to Forever, infinite loop


NMI:

    LDA #$00        ;set ram address to print sprites on PPU ($0200)
    STA $2003       ;low address
    LDA #$02
    STA $4014       ;high address (activates memory copy to PPU(?))
    JSR ReadJoy1    ;read data from controller

battle_turn:
    JSR MvHeartBattle   ;move player inside box

    RTI             ;Return from Interrupt


IRQ:

    ;NOTE: IRQ code goes here 

; Subroutines -----------------------------------------------------------------

; ---------------------- Joycontroller1 reading
.include joypad_reading.asm

; ---------------------- Player Movement
.include player_movement.asm

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
