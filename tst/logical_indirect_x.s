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
	sta $3a
	lda #$3a
	sta $fe,X
	lda #1
	sta $3b
	lda #$3b
	sta $1b,X
	lda #16
	sta $3c
	lda #$3c
	sta $cd,X

	; and flags
	lda #0
	sta $3d
	lda #$3d
	sta $fb,X
	ldx #13
	lda #-2
	sta $3e
	lda #$3e
	sta $1c,X

	; ora flags
	lda #0
	ldx #16
	sta $3f
	lda #$3f
	sta $ce,X
	lda #2
	ldx #18
	sta $31
	lda #$31
	sta $fc,X

	; eor flags
	lda #13
	sta $32
	lda #$32
	sta $1d,X
	lda #2
	ldx #11
	sta $33
	lda #$33
	sta $cf,X

	ldx #0
  lda #3
  and ($fe,X)  ; and #2
  ora ($1b,X)  ; or #1
  eor ($cd,X)  ; xor #16

  lda #3
  and ($fb,X)  ; and zero flag #0
  lda #-1
  ldx #13
  and ($1c,X) ; and negative flag #-2

  lda #0
  ldx #16
  ora ($ce,X)   ; or zero flag #0
  lda #-15
  ldx #18
  ora ($fc,X)   ; or negative flag #2

  lda #13
  eor ($1d,X)   ; eor zero flag #13
  lda #-1
  ldx #11
  eor ($cf,X)   ; eor negative flag #2

	brk

;################################################################
; interrupt vectors
;################################################################

	.org $fffa

	.dw 0
	.dw reset
	.dw 0

	.dsb $2000
