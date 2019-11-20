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
	sta $0141, Y
	lda #$41
	sta $fe
	lda #$01
	sta $ff

	lda #1
	sta $0153, Y
	lda #$53
	sta $1a
	lda #$01
	sta $1b

	lda #16
	sta $0165, Y
	lda #$65
	sta $cc
	lda #$01
	sta $cd

	; and flags
	lda #0
	sta $0177, Y
	lda #$77
	sta $fa
	lda #$01
	sta $fb

	ldx #13
	lda #-2
	sta $0189, Y
	lda #$89
	sta $1c
	lda #$01
	sta $1d

	; ora flags
	lda #0
	ldx #16
	sta $019b, Y
	lda #$9b
	sta $ce
	lda #$01
	sta $cf

	lda #2
	ldx #18
	sta $01ad, Y
	lda #$ad
	sta $fc
	lda #$01
	sta $fd

	; eor flags
	lda #13
	sta $01bf, Y
	lda #$bf
	sta $5e
	lda #$01
	sta $5f

	lda #2
	ldx #11
	sta $0280, Y
	lda #$80
	sta $6e
	lda #$02
	sta $6f


	ldx #0
  lda #3
  and ($fe), Y  ; and #2
  ora ($1a), Y  ; or #1
  eor ($cc), Y  ; xor #16

  lda #3
  and ($fa), Y  ; and zero flag #0
  lda #-1
  ldx #13
  and ($1c), Y ; and negative flag #-2

  lda #0
  ldx #16
  ora ($ce), Y   ; or zero flag #0
  lda #-15
  ldx #18
  ora ($fc), Y   ; or negative flag #2

  lda #13
  eor ($5e), Y   ; eor zero flag #13
  lda #-1
  ldx #11
  eor ($6e), Y   ; eor negative flag #2

	brk

;################################################################
; interrupt vectors
;################################################################

	.org $fffa

	.dw 0
	.dw reset
	.dw 0

	.dsb $2000
