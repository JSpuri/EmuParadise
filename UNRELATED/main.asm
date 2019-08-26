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

PRG_COUNT = $02 ;PRG ROM size (1 -> 16KiB, 2 -> 32KiB)
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

;Octave 1
A1 = $00    ;"1" means octave 1.
As1 = $01   ;"s" means "sharp"
Bb1 = $01   ;"b" means "flat".  A# == Bb
B1 = $02

;Octave 2
C2 = $03
Db2 = $04
D2 = $05
Eb2 = $06
E2 = $07
F2 = $08
Gb2 = $09
G2 = $0A
Ab2 = $0B
A2 = $0C
Bb2 = $0D
B2 = $0E

;Octave 3
C3 = $0F
Db3 = $10
D3 = $11
Eb3 = $12
E3 = $13
F3 = $14
Gb3 = $15
G3 = $16
Ab3 = $17
A3 = $18
Bb3 = $19
B3 = $1A

;Octave 4
C4 = $1B
Db4 = $1C
D4 = $1D
Eb4 = $1E
E4 = $1F
F4 = $20
Gb4 = $21
G4 = $22
Ab4 = $23
A4 = $24
Bb4 = $25
B4 = $26

;Octave 5
C5 = $27
Db5 = $28
D5 = $29
Eb5 = $2A
E5 = $2B
F5 = $2C
Gb5 = $2D
G5 = $2E
Ab5 = $2F
A5 = $30
Bb5 = $31
B5 = $32

;Octave 6
C6 = $33
Db6 = $34
D6 = $35
Eb6 = $36
E6 = $37
F6 = $38
Gb6 = $39
G6 = $3A
Ab6 = $3B
A6 = $3C
Bb6 = $3D
B6 = $3E

;Octave 7
C7 = $3F
Db7 = $40
D7 = $41
Eb7 = $42
E7 = $43
F7 = $44
Gb7 = $45
G7 = $46
Ab7 = $47
A7 = $48
Bb7 = $49
B7 = $4A

;------------------------------------------------------------------------------
; Variables - Stored in internal RAM [$0000,$0800) 
;------------------------------------------------------------------------------

    .enum $0000

    ;NOTE: declare variables using the DSB and DSW directives, like this:

    ;MyVariable0 .dsb 1
    ;MyVariable1 .dsb 3

    .ende

    ;NOTE: you can also split the variable declarations into individual pages, like this:

    ;.enum $0100
    ;.ende

    ;.enum $0200
    ;.ende

;------------------------------------------------------------------------------
; iNES header 
;------------------------------------------------------------------------------

    .db "NES", $1a  ;identification of the iNES header -> "NES<EOF>"

    .db PRG_COUNT   ;number of PGR ROM banks (2 -> 32KiB)
    .db CHR_COUNT   ;number of CHR ROM banks (1 -> 8KiB)

    .db MAPN|FSCN|TRNR|BATT|MIRR   ;byte 6

    .dsb 9, $00     ;clear the remaining flags


;------------------------------------------------------------------------------
; Program Bank - The code goes here
;------------------------------------------------------------------------------

    .org $10000-(PRG_COUNT*$4000)   ;$C000 to $FFFA in ROM with 16KiB
                                    ;$8000 to $FFFF in ROM with 32KiB
	.include "sound_engine.asm"
									
Reset:

  SEI          ; disable IRQs
  CLD          ; disable decimal mode
  LDX #$40
  STX $4017    ; disable APU frame IRQ
  LDX #$FF
  TXS          ; Set up stack
  INX          ; now X = 0
  STX $2000    ; disable NMI
  STX $2001    ; disable rendering
  STX $4010    ; disable DMC IRQs

vblankwait1:       ; First wait for vblank to make sure PPU is ready
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
  STA $0300, x
  INX
  BNE clrmem
   
vblankwait2:      ; Second wait for vblank, PPU is ready after this
  BIT $2002
  BPL vblankwait2
  
  LDA #%10000000   ;intensify blues
  STA $2001
  
  
  jsr sound_init
  
  lda #%00000111  ;enable Sq1, Sq2 and Tri channels
  sta $4015
 
  ;Square 1
  lda #%00111000        ;Duty 00, Volume 8 (half volume)
  sta $4000
  lda #Db5
  asl a                 ;shift once because we are indexing into a table of words 
  tay
  lda note_table, y
  sta $4002             ;low 8 bits of period
  lda note_table+1, y 
  sta $4003             ;high 3 bits of period
 
  ;Square 2
  lda #%01110110  ;Duty 01, Volume 6
  sta $4004
  lda #E5        
  asl a
  tay
  lda note_table, y 
  sta $4006
  lda note_table+1, y
  sta $4007
 
  ;Triangle
  lda #%10000001  ;Triangle channel on
  sta $4008
  lda #Ab5        
  asl a
  tay
  lda note_table, y
  sta $400A
  lda note_table+1, y
  sta $400B
Forever:
  JMP Forever     ;infinite loop

NMI:

    ;NOTE: NMI code goes here
	
	jsr sound_play_frame

IRQ:

    ;NOTE: IRQ code goes here 


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
