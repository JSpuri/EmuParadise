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
	lda #$f3
	ldx #254
	sta var2, X
	lda #$f1
	sta var1
	lda #$f2
	ldx #2
	sta var3, X

	lda var1
	lda var2
	lda var3

	ldx #$e3
	ldy #254
	stx var2, Y
	ldx #$e1
	stx var1
	ldx #$e2
	ldy #2
	stx var3, Y

	lda var1
	lda var2
	lda var3

	ldy #$c3
	ldx #254
	sty var2, X
	ldy #$c1
	sty var1
	ldy #$c2
	ldx #2
	sty var3, X

	lda var1
	lda var2
	lda var3

	brk

;################################################################
; interrupt vectors
;################################################################

	.org $fffa

	.dw 0
	.dw reset
	.dw 0

	.dsb $2000
