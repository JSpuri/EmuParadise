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

	lda #2
	sta $fa
	lda #1
	sta $1b
	lda #16
	sta $cd

	; and flags
	lda #0
	sta $fb
	lda #-2
	sta $1c

	; ora flags
	lda #0
	sta $ce
	lda #2
	sta $fc

	; eor flags
	lda #13
	sta $1d
	lda #2
	sta $cf

  lda #3
  and $fa  ; and #2
  ora $1b  ; or #1
  eor $cd  ; xor #16

  lda #3
  and $fb  ; and zero flag #0
  lda #-1
  ldx #13
  and $1c ; and negative flag #-2

  lda #0
  ldx #16
  ora $ce   ; or zero flag #0
  lda #-15
  ldx #18
  ora $fc   ; or negative flag #2

  lda #13
  eor $1d   ; eor zero flag #13
  lda #-1
  ldx #11
  eor $cf  ; eor negative flag #2

	brk

;################################################################
; interrupt vectors
;################################################################

	.org $fffa

	.dw 0
	.dw reset
	.dw 0

	.dsb $2000
