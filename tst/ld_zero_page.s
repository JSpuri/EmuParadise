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
	lda #$fa
	sta var1
	lda #$1b
	sta var2
	lda #$cd
	sta var3
	lda #$0

	lda var1
	lda var2
	lda var3
	ldx #1
	lda var1, X
	ldx #2
	lda var3, X
	lda #$0

	ldx var1
	ldx var2
	ldx var3
	ldy #255
	ldx var1, Y
	ldy #1
	ldx var3, Y
	ldx #$0

	ldy var1
	ldy var2
	ldy var3
	ldx #1
	ldy var1, X
	ldx #2
	ldy var3, X
	ldy #$0

	brk

;################################################################
; interrupt vectors
;################################################################

	.org $fffa

	.dw 0
	.dw reset
	.dw 0

	.dsb $2000