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
	; initial tests
	ldy #10

	ldx #0
	lda #2
	sta $01fe, Y
	lda #1
	sta $011b, Y
	lda #16
	sta $01cd, Y

	; and flags
	lda #0
	sta $01fb, Y
	ldx #13
	lda #-2
	sta $011c, Y

	; ora flags
	lda #0
	ldx #16
	sta $01ce, Y
	lda #2
	ldx #18
	sta $01fc, Y

	; eor flags
	lda #13
	sta $011d, Y
	lda #2
	ldx #11
	sta $01cf, Y

	ldx #0
  lda #3
  and $01fe, Y  ; and #2
  ora $011b, Y  ; or #1
  eor $01cd, Y  ; xor #16

  lda #3
  and $01fb, Y  ; and zero flag #0
  lda #-1
  ldx #13
  and $011c, Y ; and negative flag #-2

  lda #0
  ldx #16
  ora $01ce, Y   ; or zero flag #0
  lda #-15
  ldx #18
  ora $01fc, Y   ; or negative flag #2

  lda #13
  eor $011d, Y   ; eor zero flag #13
  lda #-1
  ldx #11
  eor $01cf, Y   ; eor negative flag #2

	brk

;################################################################
; interrupt vectors
;################################################################

	.org $fffa

	.dw 0
	.dw reset
	.dw 0

	.dsb $2000
