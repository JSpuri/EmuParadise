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
	ldy #10

  lda #3
  and #2  ; and
  ora #1  ; or
  eor #16 ; xor

  lda #3
  and #0  ; and zero flag
  lda #-1
  ldx #13
  and #-2 ; and negative flag

  lda #0
  ldx #16
  ora #0   ; or zero flag
  lda #-15
  ldx #18
  ora #2   ; or negative flag

  lda #13
  eor #13   ; eor zero flag
  lda #-1
  ldx #11
  eor #2  ; eor negative flag

	brk

;################################################################
; interrupt vectors
;################################################################

	.org $fffa

	.dw 0
	.dw reset
	.dw 0

	.dsb $2000
