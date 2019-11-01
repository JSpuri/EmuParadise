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

buttons1        .db 0
last_buttons1   .db 0

p1direction         .dsb 1  ;flag for direction after initial pass
p1firstpassdir      .dsb 1  ;flag for direction in initial pass
onWait              .dsb 1  ;determines how long to wait before next sprite loads
firstPass           .dsb 1  ;flag that determines first pass
patternResetCount   .dsb 1

max_fireballs   .dsb 1

num_oam .dsb 1  ;number of dynamic sprites

spriteCounter .dsb 1

;Player Health
player_cur_health   .dsb 1
player_max_health   .dsb 1
player_state        .dsb 1 ;1 - Start Menu, 2 - Battle, 3 - Battle Menu

player_invencibility    .dsb 1

sound_ptr .dsb 2
current_song .db 2	
	.ende

   .enum $0200
sprites
   .ende


   .enum $02E4
hp_d1   .dsb 4
hp_d2   .dsb 4
    .ende
	
	.enum $02EC
heart   .dsb #$10
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

	;.include "sound_engine_asm6f.asm"
;----- second 8k bank of PRG-ROM    
    ;bank 1
    ;.base $a000
    ;.org $A000
     ;.pad $c000
	
;----- third 8k bank of PRG-ROM    
    ;.bank 2
	;.base $c000
    ;.org $C000

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

; ----------------------------- Toriel's Battle ------------------------------
LoadVariables:
    LDA #$42
    STA player_cur_health   ;set curr health to 20
    STA player_max_health   ;set max health to 20

    LDA #$98
    STA max_fireballs 

    LDA #$00
    STA num_oam ;set number of sprites on screen (heart and hp)

    LDA #$03
    STA player_state    ;set start state to "Battle Menu"

LoadPalettesLoop:
    LDA palette, x          ;load palette byte
    STA $2007               ;write to PPU
    INX                     ;set index to next byte
    CPX #$20            
    BNE LoadPalettesLoop    ;if x = $20, 32 bytes copied, all done

LoadSpritesBattle:              ;loads basic elements of battle (heart and hp)

    LDX #$00
LoadHPLoop:                 ;HP has 2 tiles: D1 and D2, first digit and second digit
    LDA sprite_hp, x        ;in total, 8 bytes
    STA hp_d1, x
    INX
    CPX #$08
    BNE LoadHPLoop

    LDX #$00
LoadHeartLoop:
    LDA sprite_coracao, x          ;load data from address (sprites_rom +  x)
    STA heart, x              ;store into RAM address ($heart+ x)
    INX
    CPX #$10                    ; each sprite multiplies by 4
    BNE LoadHeartLoop   ; Branch while not all sprites were loaded


LoadBackground:
    LDA $2002   ; read PPU status to reset the high/low latch
    LDA #$20
    STA $2006   ; write the high byte of $2000 address
    LDA #$00
    STA $2006   ; write the low byte of $2000 address
    LDX #$00    ; start out at 0

	; #tiles on screen = 32 * 30 = 960 bytes = $03C0
	; need two bytes to represent (a low and a high)

	LDA #<background_playing_rom    ;load low bytes of address ($XX[XX])
	STA backgroundLo
	LDA #>background_playing_rom    ;load high bytes of address ($[XX]XX)
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
    LDA attributePlaying, x        ; load data from address (attribute + value in x)
    STA $2007               ; write to PPU
    INX                     ; X = X + 1
    CPX #$08                ; Compare X to hex $08, decimal 8 - copying 8 bytes
    BNE LoadAttributeLoop   ; branch while attributes arent fully loaded

    ;LDA #%10010000          ; enable NMI, sprites from Pattern Table 0,
                            ; background from Pattern Table 1
    ;STA $2000
    ;LDA #%00011110          ; enable sprites, enable background
    ;STA $2001               ; no clipping on left side of screen (8 pixels)
    
;Enable sound channels
    ;jsr sound_init
    
    ;lda #$02
    ;sta current_song
    ;jsr sound_load
    
    lda #$88
    sta $2000   ;enable NMIs
    lda #$18
    sta $2001   ;turn PPU on
	
	;lda current_song
    ;jsr sound_load

StartBattle:

    ;LDA player_state

    ;CMP #$02
    ;BEQ TorielTurn

BattleMenu:
    ;.include battle_menu_handler.asm

TorielTurn:
    .include fireballs.asm
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

    JSR CheckCollision

	;jsr sound_play_frame    ;run our sound engine after all drawing code is done.
                            ;this ensures our sound engine gets run once per frame.

    RTI             ;Return from Interrupt


IRQ:

    ;NOTE: IRQ code goes here 

; Subroutines -----------------------------------------------------------------

; ---------------------- Joycontroller1 reading
.include joypad_reading.asm


; ---------------------- Player Movement
.include player_movement.asm


; ---------------------- Collision Handler
CheckCollision:
                        ;lets try to debug this

    LDX #$00            ;if the array of fireballs is empty,
    CMP num_oam         ;there is nothing to do
    BEQ its_empty
    LDX #$04            ;but if it is not, we need to check only the bottom half
check_collision_loop:

check_upper_border:
    LDA heart
    CMP sprites, x
    BCS check_lower_border
    JMP next_fireball

check_lower_border:
    LDA sprites, x
    CLC
   ; ADC #$07
    CMP heart
    BCS check_left_border
    JMP next_fireball

check_left_border:
    LDA heart + $03
    CMP sprites + $03, x
    BCS check_right_border
    JMP next_fireball

check_right_border:
    LDA sprites + $03, x
    CLC
   ; ADC #$07
    CMP heart + $03
    BCC thats_a_hit
    JMP next_fireball

thats_a_hit:

    LDY hp_d2 + 1
    DEY

    CPY #$01
    STY hp_d2 + 1
    BEQ dec_d1
    JMP next_fireball
    
dec_d1:
    LDY hp_d1 + 1
    CPY #$02
    BEQ end_game

    LDY #$0B
    STY hp_d2 + 1

    LDY hp_d1 + 1
    DEY
    CMP #$02
    BEQ end_game

    STY hp_d1 + 1

next_fireball:
    TXA
    CLC
    ADC #$04
    TAX
    CMP num_oam
    BCC check_collision_loop

its_empty:
    RTS

end_game:
    JMP Reset
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
.base $0000
    ;.org $0000
	;.base $0000
.incbin "unrelated_chars.chr"   ;includes 8KB graphics file from SMB1
.pad $7ff0
