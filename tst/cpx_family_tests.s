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
    ; Setup the values to be tested for cpx
    lda #1 
    ldx #1
    sta var1
    sta var2

    ; Immediate CPX
    cpx #1
    cpx #0
    cpx #2

    ; Absolute Zeropage CPX
    ldx #1
    cpx var1
    ldx #0
    cpx var1
    ldx #2
    cpx var1

    ; Absolute CPX
    ldx #1
    cpx var2
    ldx #0
    cpx var2
    ldx #2
    cpx var2

    brk
;################################################################
; interrupt vectors
;################################################################

	.org $fffa

	.dw 0
	.dw reset
	.dw 0

	.dsb $2000

    