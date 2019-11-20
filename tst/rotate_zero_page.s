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
var2: .dsb 1
	.ende

	.enum $00ff
var3: .dsb 1
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
	ldx #$aa

	lda #$8
	sta $a1
	lda #$fa
	sta $a3
	lda #$80
	sta $a5
	lda #$7a
	sta $a7
	lda #$8
	sta $a9
	lda #$3
	sta $ab
	lda #$1
	sta $ad
	lda #$f0
	sta $af

	; rol
	rol $a1

	; rol carry flag
	rol $a3

	; rol zero flag
	rol $a5

	; rol negative_flag
	rol $a7

	; ror
	ror $a9

	; ror carry flag
	ror $ab

	; ror zero flag
	ror $ad

	; ror negative_flag
	ror $af

	brk

;################################################################
; interrupt vectors
;################################################################

	.org $fffa

	.dw 0
	.dw reset
	.dw 0

	.dsb $2000
