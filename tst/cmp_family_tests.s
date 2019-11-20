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

	.ende

	.enum $0100


	.ende

	.enum $07fe
var2: .dsb 1
	.ende

	.enum $fefe
debug: .dsb 1
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
    ; Setup the values to be tested for cmp
    lda #1 
    ldx #1
    ldy #1
    sta var1
    sta var2
    sta var1, x
    sta var2, x    
    sta (var1, x)
    sta (var1),y
   
    ; Immediate CMP
    ; Flags NZC should be 001, 011, 100
    lda #1
    cmp #1
    cmp #0
    cmp #2

    ; Absolute Zeropage CMP
    lda #1
    cmp var1
    lda #0
    cmp var1 
    lda #2
    cmp var1

    ; Absolute CMP
    lda #1
    cmp var2
    lda #0
    cmp var2 
    lda #2
    cmp var2

    ; Zeropage + X CMP
    lda #1
    cmp var1, x
    lda #0
    cmp var1, x
    lda #2
    cmp var1, x
    lda #179
    sta debug

    ; Address + X CMP
    lda #1
    cmp var2, x
    lda #0
    cmp var2, x 
    lda #2
    cmp var2, x

    ; Indirect X CMP
    lda #1
    cmp (var1, x)
    lda #0
    cmp (var1, x)
    lda #2
    cmp (var1, x)

    ; Indirect Y CMP
    lda #1
    cmp (var1), y
    lda #0
    cmp (var1), y
    lda #2
    cmp (var1), y

    brk

;################################################################
; interrupt vectors
;################################################################

	.org $fffa

	.dw 0
	.dw reset
	.dw 0

	.dsb $2000

    