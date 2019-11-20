;################################################################
; Constants
;################################################################

PRG_COUNT = 1 ;1 = 16KB, 2 = 32KB
MIRRORING = %0001 ;%0000 = horizontal, %0001 = vertical, %1000 = four-screen

;################################################################
; Variables
;################################################################

	.enum $00ff
var1: .dsb 1
	.ende

	.enum $0000
var2: .dsb 1
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
	lda #$0

	ldy #0
	lda #$01
	sta (var1), Y
	ldy #1
	lda #$f2
	sta (var1), Y
	lda #0

	ldy #0
	lda (var1), Y
	ldy #1
	lda (var1), Y

	brk

;################################################################
; interrupt vectors
;################################################################

	.org $fffa

	.dw 0
	.dw reset
	.dw 0

	.dsb $2000