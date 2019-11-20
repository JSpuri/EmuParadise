;################################################################
; Constants
;################################################################

PRG_COUNT = 1 ;1 = 16KB, 2 = 32KB
MIRRORING = %0001 ;%0000 = horizontal, %0001 = vertical, %1000 = four-screen


;################################################################
; Variables
;################################################################

	.enum $0000

var1: .dsb 1

	.ende

	.enum $0100


	.ende

	.enum $07fe
var2: .dsb 1
	.ende

	.enum $fefe
debug: .dsb 1
	.ende

;################################################################
; iNES header
;################################################################

	.db "NES", $1a		; Identification of the iNES header
	.db PRG_COUNT		; Number of 16KB PRG-ROM pages
	.db $01				; Number of 8KB CHR-ROM pages
	.db $00|MIRRORING	; Mapper 0 and mirroring
	.dsb 9, $00			; Clear the remaining bytes

;################################################################
; program bank(s)
;################################################################

	.base $10000-(PRG_COUNT*$4000)

;################################################################
; RESET
;################################################################
reset:
    ; ADC immediate without carry
    lda #0
    pha 
    plp
    lda #1 
    adc #1
    lda #0
    pha 
    plp
    lda #127
    adc #1
    lda #0
    pha 
    plp 
    lda #-128
    adc #-1
    lda #0
    pha 
    plp
    lda #255
    adc #1
    lda #0
    pha 
    plp
    ; ADC immediate with carry
    lda #0
    pha 
    plp
    sec
    lda #1 
    adc #1
    lda #0
    pha 
    plp
    sec
    lda #127
    adc #1
    lda #0
    pha 
    plp 
    sec
    lda #-128
    adc #-1
    lda #0
    pha 
    plp
    sec
    lda #255
    adc #1
    lda #0
    pha 
    plp
    brk

;################################################################
; interrupt vectors
;################################################################

	.org $fffa

	.dw 0
	.dw reset
	.dw 0

	.dsb $2000

    