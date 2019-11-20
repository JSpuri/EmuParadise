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
	sta $0ba1
	lda #$fa
	sta $0ba3
	lda #$80
	sta $0ba5
	lda #$7a
	sta $0ba7
	lda #$8
	sta $0ba9
	lda #$3
	sta $0bab
	lda #$1
	sta $0bad
	lda #$f0
	sta $0baf

	; asl
	asl $0ba1

	; asl carry flag
	asl $0ba3

	; asl zero flag
	asl $0ba5

	; asl negative_flag
	asl $0ba7

	; lsr
	lsr $0ba9

	; lsr carry flag
	lsr $0bab

	; lsr zero flag
	lsr $0bad

	; lsr negative_flag
	lsr $0baf

	brk

;################################################################
; interrupt vectors
;################################################################

	.org $fffa

	.dw 0
	.dw reset
	.dw 0

	.dsb $2000
