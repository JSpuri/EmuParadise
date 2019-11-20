;################################################################
; Constants
;################################################################

PRG_COUNT = 1 ;1 = 16KB, 2 = 32KB
MIRRORING = %0001 ;%0000 = horizontal, %0001 = vertical, %1000 = four-screen

;################################################################
; Variables
;################################################################

	.enum $02fe
var1: .dsb 1
var2: .dsb 1
	.ende

	.enum $0300
var3: .dsb 1
var4: .dsb 1
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
	lda #$01
	sta var1
	lda #$02
	sta var2
	lda #$f3
	sta var3
	lda #$f4
	sta var4
	lda #$0

	ldx #0
	lda var1, X
	ldx #1
	lda var1, X
	ldx #2
	lda var1, X
	ldx #3
	lda var1, X
	ldy #0
	lda var1, Y
	ldy #1
	lda var1, Y
	ldy #2
	lda var1, Y
	ldy #3
	lda var1, Y
	lda #$0

	ldy #0
	ldx var1, Y
	ldy #1
	ldx var1, Y
	ldy #2
	ldx var1, Y
	ldy #3
	ldx var1, Y
	ldx #$0

	ldx #0
	ldy var1, X
	ldx #1
	ldy var1, X
	ldx #2
	ldy var1, X
	ldx #3
	ldy var1, X
	ldy #$0

	lda #10
	sta $2001
	ldx $2001

	sta $2010
	ldx $2010

	sta $3FFF
	ldx $3FFF

	sta $4010
	ldx $4010

	sta $4020
	ldx $4020

	brk

;################################################################
; interrupt vectors
;################################################################

	.org $fffa

	.dw 0
	.dw reset
	.dw 0

	.dsb $2000