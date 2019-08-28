;.base $7ff0
	.inesprg 1   ; 1x 16KB PRG code
  .ineschr 1   ; 1x  8KB CHR data
  .inesmap 0   ; mapper 0 = NROM, no bank swapping
  .inesmir 1   ; background mirroring
;.pad $8000, $00  

.enum $0000		;start variables at ram location 0 in zero page memory
	backgroundLo:
		.dsb 1
	backgroundHi:
		.dsb 1
	counterLo:
		.dsb 1
	counterHi:
		.dsb 1
.ende


;;;;;;;;;;;;;;;

    
  .base $C000
  ;.org $C000 
RESET:
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
  STA $0300, x
  STA $0400, x
  STA $0500, x
  STA $0600, x
  STA $0700, x
  LDA #$FE
  STA $0200, x
  INX
  BNE clrmem

vblankwait2:      ; Second wait for vblank, PPU is ready after this
  BIT $2002
  BPL vblankwait2


LoadPalettes:
  LDA $2002             ; read PPU status to reset the high/low latch
  LDA #$3F
  STA $2006             ; write the high byte of $3F00 address
  LDA #$00
  STA $2006             ; write the low byte of $3F00 address
  LDX #$00              ; start out at 0
LoadPalettesLoop:
  LDA palette, x        ; load data from address (palette + the value in x)
                          ; 1st time through loop it will load palette+0
                          ; 2nd time through loop it will load palette+1
                          ; 3rd time through loop it will load palette+2
                          ; etc
  STA $2007             ; write to PPU
  INX                   ; X = X + 1
  CPX #$20              ; Compare X to hex $10, decimal 16 - copying 16 bytes = 4 sprites
  BNE LoadPalettesLoop  ; Branch to LoadPalettesLoop if compare was Not Equal to zero
                        ; if compare was equal to 32, keep going down



LoadSprites:
  LDX #$00              ; start at 0
LoadSpritesLoop:
  LDA sprites, x        ; load data from address (sprites +  x)
  STA $0200, x          ; store into RAM address ($0200 + x)
  INX                   ; X = X + 1
	CPX #$78							; each sprite multiplies by 4
  BNE LoadSpritesLoop   ; Branch to LoadSpritesLoop if compare was Not Equal to zero
                        ; if compare was equal to 16, keep going down

              
              
              
LoadBackground:
  LDA $2002             ; read PPU status to reset the high/low latch
  LDA #$20
  STA $2006             ; write the high byte of $2000 address
  LDA #$00
  STA $2006             ; write the low byte of $2000 address
  LDX #$00              ; start out at 0

	; #tiles on screen = 32 * 30 = 960 bytes = $03C0
	; need two bytes to represent (a low and a high)

	LDA #<background
	STA backgroundLo
	LDA #>background
	STA backgroundHi	
	LDA #$C0							
	STA counterLo
	LDA #$03
	STA counterHi
	
	LDY #$00
LoadBackgroundLoop:
	LDA (backgroundLo), y
	STA $2007
	LDA backgroundLo
	CLC
	ADC #$01
	STA backgroundLo
	LDA backgroundHi
	ADC #$00						; add 0, but if there is a carry, add 1
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

	LDA counterLo         ; load the low byte
  CMP #$00              ; see if it is zero, if not loop
  BNE LoadBackgroundLoop
  LDA counterHi
  CMP #$00              ; see if the high byte is zero, if not loop
  BNE LoadBackgroundLoop  ; if the loop counter isn't 0000, keep copying
              
LoadAttribute:
  LDA $2002             ; read PPU status to reset the high/low latch
  LDA #$23
  STA $2006             ; write the high byte of $23C0 address
  LDA #$C0
  STA $2006             ; write the low byte of $23C0 address
  LDX #$00              ; start out at 0
LoadAttributeLoop:
  LDA attribute, x      ; load data from address (attribute + the value in x)
  STA $2007             ; write to PPU
  INX                   ; X = X + 1
  CPX #$08              ; Compare X to hex $08, decimal 8 - copying 8 bytes
  BNE LoadAttributeLoop  ; Branch to LoadAttributeLoop if compare was Not Equal to zero
                        ; if compare was equal to 128, keep going down
              
  LDA #%10010000   ; enable NMI, sprites from Pattern Table 0, background from Pattern Table 1
  STA $2000

  LDA #%00011110   ; enable sprites, enable background, no clipping on left side
  STA $2001

Forever:
  JMP Forever     ;jump back to Forever, infinite loop
  

NMI:
  LDA #$00
  STA $2003       ; set the low byte (00) of the RAM address
  LDA #$02
  STA $4014       ; set the high byte (02) of the RAM address, start the transfer


  ;;This is the PPU clean up section, so rendering the next frame starts properly.
  LDA #%10010000   ; enable NMI, sprites from Pattern Table 0, background from Pattern Table 1
  STA $2000
  LDA #%00011110   ; enable sprites, enable background, no clipping on left side
  STA $2001
  LDA #$00        ;;tell the ppu there is no background scrolling
  STA $2005
  STA $2005
  
  RTI             ; return from interrupt

;;;;;;;;;;;;;;  
  
  .pad $e000 

  .base $e000
  ;.org $E000
palette:
	;   actions						action selected		other elements												
  .db $0F,$26,$1A,$0F,  $0F,$28,$17,$0F,  $0F,$30,$21,$0F,  $0F,$09,$17,$0F   ;;background palette
  .db $0F,$1C,$15,$14,  $0F,$02,$38,$3C,  $0F,$1C,$15,$14,  $0F,$02,$38,$3C   ;;sprite palette

; first color is for transparency

sprites:
    ;vert tile attr horiz
;coracao 
	.db $80, $0, $00, $80   ;heart sprite	up 0
	.db $80, $1, $00, $88		;heart sprite up 1
	.db $88, $10, $00, $80  ;heart sprite down 0
	.db $88, $11, $00, $88  ;heart sprite down 1
	
;coracao partido
	.db $80, $20, $00, $90   ;heart sprite	up 0
	.db $80, $21, $00, $98		;heart sprite up 1
	.db $88, $30, $00, $90  ;heart sprite down 0
	.db $88, $31, $00, $98  ;heart sprite down 1
;fogo
	.db $80, $40, $00, $a0   ;heart sprite	up 0
	.db $88, $50, $00, $a0		;heart sprite up 1
;slash
	.db $22, $12, $00, $70  ;heart sprite down 0
	.db $22, $13, $00, $78  ;heart sprite down 1
	.db $22, $14, $00, $80  ;heart sprite down 0
	.db $22, $15, $00, $89  ;heart sprite down 0
	
	.db $2a, $23, $00, $78  ;heart sprite down 1
	.db $2a, $24, $00, $80  ;heart sprite down 0
	.db $2a, $25, $00, $88  ;heart sprite down 1

	.db $32, $34, $00, $80  ;heart sprite down 0
	.db $32, $35, $00, $88  ;heart sprite down 1

	.db $3a, $44, $00, $80  ;heart sprite down 0
	.db $3a, $45, $00, $88  ;heart sprite down 0

	.db $42, $53, $00, $78  ;heart sprite down 1
	.db $42, $54, $00, $80  ;heart sprite down 0
	.db $42, $55, $00, $88  ;heart sprite down 1

	.db $4a, $62, $00, $70  ;heart sprite down 0
	.db $4a, $63, $00, $78  ;heart sprite down 1
	.db $4a, $64, $00, $80  ;heart sprite down 0
	.db $4a, $65, $00, $88  ;heart sprite down 1

;hp
  .db $bf, $3, $00, $b6   ;sprite 1 hp first digit
  .db $bf, $2, $00, $be   ;sprite 0 hp second digit

;30 lines x 32 columns
background:
	.db $40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40		;line 0
	.db $40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40	  

	.db $40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$a,$b			;HEAD 0
	.db $c,$d,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40	     
  
	.db $40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$1a,$1b		;HEAD 1
	.db $1c,$1d,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40	   
  
	.db $40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$2a,$2b		;HEAD 3
	.db $2c,$2d,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40	  

	.db $40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$f,$3a,$3b		;TOR 0
	.db $3c,$3d,$47,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40	  
  
	.db $40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$1f,$49,$4a		;TOR 1
	.db $4b,$4c,$57,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40	  
  
	.db $40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$2e,$2f,$59,$5a		;TOR 2
	.db $5b,$5c,$67,$68,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40	  

	.db $40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$3e,$3f,$69,$6a		;TOR 3
	.db $6b,$6c,$77,$78,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40	  

	.db $40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$4e,$4f,$79,$7a		;TOR 4
	.db $7b,$7c,$87,$88,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40	  
  
	.db $40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$5e,$5f,$8a,$8b		;LEG 0
	.db $8c,$8d,$97,$98,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40	  
  
	.db $40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$99,$9a,$9b		;LEG 1
	.db $9c,$9d,$9e,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40	  
	
	.db $40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$a9,$aa,$ab		;FOOT 0
	.db $ac,$ad,$ae,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40

	.db $40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40		;line 12
	.db $40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40

	.db $40,$40,$40,$40,$40,$40,$40,$40,$40,$36,$38,$38,$38,$38,$38,$38		;SUP_BOR
	.db $38,$38,$38,$38,$38,$38,$39,$40,$40,$40,$40,$40,$40,$40,$40,$40	
  
	.db $40,$40,$40,$40,$40,$40,$40,$40,$40,$41,$40,$40,$40,$40,$40,$40		;ingame 0 
	.db $40,$40,$40,$40,$40,$40,$42,$40,$40,$40,$40,$40,$40,$40,$40,$40
  
	.db $40,$40,$40,$40,$40,$40,$40,$40,$40,$41,$40,$40,$40,$40,$40,$40		;ingame 1
	.db $40,$40,$40,$40,$40,$40,$42,$40,$40,$40,$40,$40,$40,$40,$40,$40
  
	.db $40,$40,$40,$40,$40,$40,$40,$40,$40,$41,$40,$40,$40,$40,$40,$40		;ingame 2
	.db $40,$40,$40,$40,$40,$40,$42,$40,$40,$40,$40,$40,$40,$40,$40,$40
  
	.db $40,$40,$40,$40,$40,$40,$40,$40,$40,$41,$40,$40,$40,$40,$40,$40		;ingame 3
	.db $40,$40,$40,$40,$40,$40,$42,$40,$40,$40,$40,$40,$40,$40,$40,$40

	.db $40,$40,$40,$40,$40,$40,$40,$40,$40,$41,$40,$40,$40,$40,$40,$40		;ingame 4
	.db $40,$40,$40,$40,$40,$40,$42,$40,$40,$40,$40,$40,$40,$40,$40,$40
  
	.db $40,$40,$40,$40,$40,$40,$40,$40,$40,$41,$40,$40,$40,$40,$40,$40		;ingame 5
	.db $40,$40,$40,$40,$40,$40,$42,$40,$40,$40,$40,$40,$40,$40,$40,$40
  
	.db $40,$40,$40,$40,$40,$40,$40,$40,$40,$41,$40,$40,$40,$40,$40,$40		;ingame 6
	.db $40,$40,$40,$40,$40,$40,$42,$40,$40,$40,$40,$40,$40,$40,$40,$40
  
	.db $40,$40,$40,$40,$40,$40,$40,$40,$40,$41,$40,$40,$40,$40,$40,$40		;ingame 7
	.db $40,$40,$40,$40,$40,$40,$42,$40,$40,$40,$40,$40,$40,$40,$40,$40

	.db $40,$40,$40,$40,$40,$40,$40,$40,$40,$44,$37,$37,$37,$37,$37,$37		;INF_BOR
	.db $37,$37,$37,$37,$37,$37,$43,$40,$40,$40,$40,$40,$40,$40,$40,$40	
	
	.db $40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40		;line 24
	.db $40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40	  
  
	.db $40,$40,$40,$40,$25,$21,$10,$34,$14,$27,$1,$40,$40,$40,$21,$31		;INFOS
	.db $1,$40,$40,$40,$17,$25,$40,$40,$40,$45,$1,$0,$40,$40,$40,$40	  

	.db $40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40		;line 26
	.db $40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40	

  .db $40,$50,$51,$52,$53,$54,$55,$56,$40,$70,$71,$72,$73,$74,$75,$76		;MENU_SUP
	.db $90,$91,$92,$93,$94,$95,$96,$40,$b0,$b1,$b2,$b3,$b4,$b5,$b6,$40	

  .db $40,$60,$61,$62,$63,$64,$65,$66,$40,$80,$81,$82,$83,$84,$85,$86		;MENU_INF
	.db $a0,$a1,$a2,$a3,$a4,$a5,$a6,$40,$c0,$c1,$c2,$c3,$c4,$c5,$c6,$40	
  
	.db $40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40		;line 29
	.db $40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40	


attribute:
  .db %00000000, %00010000, %01010000, %00010000, %00000000, %00000000, %00000000, %00110000


	.pad $fffa
  ;.org $FFFA     ;first of the three vectors starts here
  .dw NMI        ;when an NMI happens (once per frame if enabled) the 
                  ;processor will jump to the label NMI:
  .dw RESET      ;when the processor first turns on or is reset, it will jump
                  ;to the label RESET:
  .dw 0          ;external interrupt IRQ is not used in this tutorial
  
  
;;;;;;;;;;;;;;  
  
  
  .base $0000
  ;.org $0000
  .incbin "unrelated_chars.chr"   ;includes 8KB graphics file from SMB1
	.pad $7ff0