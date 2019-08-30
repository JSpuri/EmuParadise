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

    .enum $0000             ;background graphics variables

backgroundLo:   .dsb 1
backgroundHi:	.dsb 1
counterLo:  	.dsb 1
counterHi:		.dsb 1

    .ende

    .enum $0010

buttons1        .db 0      ;used to read input from controller 1
last_buttons1   .db 0      ;last input read from controller 1

player_cur_health   .db MAX_HEALTH
player_max_health   .db MAX_HEALTH    ;player health

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

Reset: ; ----------------------------------------------------------------------

    SEI         ;Set Interrupt Disable
    CLD         ;Clear Decimal Mode
    LDX #$40
    STX $4017   ;disable APU frame IRQ
    LDX #$01FF
    TXS         ;Set up stack at $01FF
    INX         ;now X = 0
    STX $2000   ;disable NMI
    STX $2001   ;disable rendering
    STX $4010   ;disable DMC IRQs
    

vblankwait1:    ;First wait for vblank to make sure PPU is ready
    BIT $2002
    BPL vblankwait1

clrmem:         ;memory cleaner
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
    LDA $2002   ;read PPU status to reset the high/low latch
    LDA #$3F
    STA $2006   ;write the high byte of $3F00 address
    LDA #$00
    STA $2006   ;write the low byte of $3F00 address
    LDX #$00

vblankwait2:    ;Second wait for vblank, PPU is ready after this
    BIT $2002
    BPL vblankwait2

LoadPalettesLoop:
    LDA palette, x          ;load palette byte
    STA $2007               ;write to PPU
    INX                     ;set index to next byte
    CPX #$20            
    BNE LoadPalettesLoop    ;if x = $20, 32 bytes copied, all done


LoadSpritesBattle:              ;loads basic elements of battle (heart and hp)
    LDX #$00
LoadSpritesBattleLoop:
    LDA sprites_rom, x          ;load data from address (sprites_rom +  x)
    STA sprites, x              ;store into RAM address ($0200 + x)
    INX
    CPX #$18                    ; each sprite multiplies by 4
    BNE LoadSpritesBattleLoop   ; Branch while not all sprites were loaded


LoadBackground:
    LDA $2002   ; read PPU status to reset the high/low latch
    LDA #$20
    STA $2006   ; write the high byte of $2000 address
    LDA #$00
    STA $2006   ; write the low byte of $2000 address
    LDX #$00    ; start out at 0

	; #tiles on screen = 32 * 30 = 960 bytes = $03C0
	; need two bytes to represent (a low and a high)

	LDA #<background_rom    ;load low bytes of address ($XX[XX])
	STA backgroundLo
	LDA #>background_rom    ;load high bytes of address ($[XX]XX)
	STA backgroundHi	
	LDA #$C0				
	STA counterLo
	LDA #$03
	STA counterHi

	LDY #$00
LoadBackgroundLoop:         ;keep writing background tiles to PPU
	LDA (backgroundLo), y
	STA $2007
	LDA backgroundLo
	CLC
	ADC #$01
	STA backgroundLo
	LDA backgroundHi
	ADC #$00				; add 0, but if there is a carry, add 1
	STA backgroundHi		

	LDA counterLo
	SEC
	SBC #$01
	STA counterLo
	LDA counterHi
	SBC #$00
	STA counterHi
	SBC #$00
	STA counterHi

	LDA counterLo           ; load the low byte
    CMP #$00                ; see if it is zero, if not loop
    BNE LoadBackgroundLoop
    LDA counterHi
    CMP #$00                ; see if the high byte is zero, if not loop
    BNE LoadBackgroundLoop  ; if the loop counter isn't 0000, keep copying

LoadAttribute:
    LDA $2002               ; read PPU status to reset the high/low latch
    LDA #$23
    STA $2006               ; write the high byte of $23C0 address
    LDA #$C0
    STA $2006               ; write the low byte of $23C0 address
    LDX #$00                ; start out at 0
LoadAttributeLoop:
    LDA attribute, x        ; load data from address (attribute + value in x)
    STA $2007               ; write to PPU
    INX                     ; X = X + 1
    CPX #$08                ; Compare X to hex $08, decimal 8 - copying 8 bytes
    BNE LoadAttributeLoop   ; branch while attributes arent fully loaded

    LDA #%10010000          ; enable NMI, sprites from Pattern Table 0,
                            ; background from Pattern Table 1
    STA $2000
    LDA #%00011110          ; enable sprites, enable background
    STA $2001               ; no clipping on left side of screen (8 pixels)


Forever:
    JMP Forever     ;jump back to Forever, infinite loop


NMI: ; ------------------------------------------------------------------------

    LDA #$00        ;set ram address to print sprites on PPU ($0200)
    STA $2003       ;low address
    LDA #$02
    STA $4014       ;high address (activates memory copy to PPU(?))
    JSR ReadJoy1    ;read data from controller

    ;;This is the PPU clean up section, so rendering the next frame starts properly.
    LDA #%10010000  ; enable NMI, sprites from Pattern Table 0
    STA $2000       ; background from Pattern Table 1

    LDA #%00011110  ; enable sprites, enable background, no clipping on left side
    STA $2001
    LDA #$00        ;;tell the ppu there is no background scrolling
    STA $2005
    STA $2005

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
    .org $E000
.include background_sprites.asm

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

.incbin "unrelated_chars.chr"   ;includes 8KB graphics file from SMB1
