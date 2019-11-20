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
	sta $01fe, X
	lda #1
	sta $011b, X
	lda #16
	sta $01cd, X

	; and flags
	lda #0
	sta $01fb, X
	ldx #13
	lda #-2
	sta $011c, X

	; ora flags
	lda #0
	ldx #16
	sta $01ce, X
	lda #2
	ldx #18
	sta $01fc, X

	; eor flags
	lda #13
	sta $011d, X
	lda #2
	ldx #11
	sta $01cf, X

	ldx #0
  lda #3
  and $01fe, X  ; and #2
  ora $011b, X  ; or #1
  eor $01cd, X  ; xor #16

  lda #3
  and $01fb, X  ; and zero flag #0
  lda #-1
  ldx #13
  and $011c, X ; and negative flag #-2

  lda #0
  ldx #16
  ora $01ce, X   ; or zero flag #0
  lda #-15
  ldx #18
  ora $01fc, X   ; or negative flag #2

  lda #13
  eor $011d, X   ; eor zero flag #13
  lda #-1
  ldx #11
  eor $01cf, X   ; eor negative flag #2

	brk

;################################################################
; interrupt vectors
;################################################################

	.org $fffa

	.dw 0
	.dw reset
	.dw 0

	.dsb $2000
