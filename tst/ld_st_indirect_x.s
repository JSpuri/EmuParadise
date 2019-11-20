;################################################################
; Constants
;################################################################

PRG_COUNT = 1 ;1 = 16KB, 2 = 32KB
MIRRORING = %0001 ;%0000 = horizontal, %0001 = vertical, %1000 = four-screen

;################################################################
; Variables
;################################################################

	.enum $00fe
var1: .dsb 1
var2: .dsb 1
	.ende

	.enum $0000
var3: .dsb 1
var4: .dsb 1
	.ende

	.enum $07ab
label1:	.dsb 1
label2:	.dsb 1
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
	lda #<label1
	sta var1
	lda #>label1
	sta var2
	lda #<label2
	sta var3
	lda #>label2
	sta var4
	lda #$0

	ldx #0
	lda #$01
	sta (var1, X)
	ldx #2
	lda #$f2
	sta (var1, X)
	lda #0

	ldx #0
	lda (var1, X)
	ldx #2
	lda (var1, X)

	brk

;################################################################
; interrupt vectors
;################################################################

	.org $fffa

	.dw 0
	.dw reset
	.dw 0

	.dsb $2000