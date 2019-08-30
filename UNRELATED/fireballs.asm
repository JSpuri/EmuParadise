vblankwaitLoop:      ; Wait for vblank, PPU is ready after this

    BIT $2002
    BPL vblankwaitLoop

    INC onWait          ; when ppu is ready adds one to onWait
    LDX firstPass     
    CPX #$00            ; if its in first pass (just after reset)
    BEQ loadFirstPass   ; goes to loadFirstPass
                        ; else keeps going
    LDX patternResetCount
    CPX #$08
    BEQ patternReset
    LDX #$60
p1Loop:
    JSR pattern1        ; calls pattern1
    INX                 ; X+4 to reach next byte
    INX
    INX
    INX
    CPX max_fireballs ; if X == 56 (7 sprites) it leaves the loop
    BNE p1Loop
    LDX spriteCounter       ; this is the counter that dictates the direction
    INX
    CPX #$0C            ; after 12 cicles to one side it changes direction
    BMI ignoreChangedir ; jumps to ignoreChangedir if X < 0C (if N flag is set)
    JSR changep1dir
    LDX patternResetCount
    INX
    STX patternResetCount
    LDX #$00

ignoreChangedir:
    STX spriteCounter       ; if X < 12 it just increases by one, from INX, else it returns as 0 from changep1dir
    JMP vblankwaitLoop

patternReset:
    LDX #$60
    LDA #$ef
patternResetLoop:
    STA $0200, x
    INX
    INX
    INX
    INX
    CPX #$98
    BNE patternResetLoop
    LDX #$00
    STX spriteCounter
    STX num_oam
    STX onWait
    STX p1direction
    STX p1firstpassdir
    STX firstPass
    STX patternResetCount
    JMP vblankwaitLoop

loadFirstPass:        
    LDX onWait          
    CPX #$08            ;if 8 cicles of "wainting" has passed another sprite is loaded onto the screen
    BNE runOtherSprites
    LDX #$00
    STX onWait          ; resets the onWait counter

    LDX spriteCounter       ; this counter will keep track of how many bytes were written
    TXA
    CLC
    ADC #$60
    TAY

    LDA #$60            ; load first byte of top of fire (vertical position)
    STA $0200, y
    INX
    INY
    LDA sprite_fogo + 1      ; load second byte of top of fire (sprite number)
    STA $0200, y
    INX
    INY
    LDA sprite_fogo + 2      ; load third byte of top of fire (attrs)
    STA $0200, y
    INX
    TXA                 ; accumulator will receive X's value to more easily add to the horizontal position
    TAY                 ; y register will receive x's value to add distance relative to first sprite
                        ; so if the second sprite added +4 to its horizontal position, the third will add +8
    LDA #$3F            ; load fourth byte of top of fire (horizontal position)

loadHorizontalLoop1:  ; this loop sets the horizontal distance between the sprites
        CLC
        ADC #$02
        DEY               
        CPY #$00
        BPL loadHorizontalLoop1
    TAY
    TXA
    INX
    STX spriteCounter
    CLC
    ADC #$60
    TAX
    TYA
    STA $0200, x
    TXA               ; this is done here so that the current value of x is used in the bottom portion of the sprite
    TAY
    INY               ; the y register will keep the old value of x until the next horizontal position loop

    LDX spriteCounter

    ;does the same for bottom of part of the fire
    LDA #$66            
    STA $0200, y
    INX
    INY
    LDA sprite_fogo + 5      
    STA $0200, y
    INX
    INY
    LDA sprite_fogo + 6      
    STA $0200, y
    INX
    TXA
    SEC
    SBC #$04
    TAY
    LDA #$3F            

loadHorizontalLoop2:
        CLC
        ADC #$02
        DEY
        CPY #$00
        BNE loadHorizontalLoop2
    TAY
    TXA
    INX
    STX spriteCounter
    CLC
    ADC #$60
    TAX
    TYA
    STA $0200, x

    LDX num_oam
    INX
    STX num_oam

runOtherSprites:      ; this portion of code also acts as the "wainting" routine
    LDY spriteCounter       ; it takes care of moving the already loaded sprites in order to distance them vertically
                      ; from the ones to be loaded               
    CPY #$00            ; the sprite movement starts from 8 due to hp sprite
    BEQ skipEndFirstPass
    LDX #$60
loadPassP1Loop:       ; this loop has almost the same ideia of p1Loop, but in reverse
                      ; instead of going from the first sprite to the last, it goes from the last sprite loaded
                      ; using the current value of spriteCounter, up to the first one added
    JSR pattern1
    INX   
    INX
    INX
    INX
    DEY
    DEY
    DEY
    DEY
    CPY #$00            ; stops when reaches hp sprite
    BNE loadPassP1Loop
    LDX p1firstpassdir  ; checks if its time to change directions
    INX
    CPX #$0C
    BMI ignoreChange
    JSR changep1dirfp

ignoreChange:
    STX p1firstpassdir  ; stores p1firstpassdir+1 if going to same direction, if not x returns as 0 from change1dirfp
    LDX spriteCounter
    CPX #$38            ; when p1 counter reaches 56 it means all fire sprites were loaded
    BNE skipEndFirstPass
    LDX #$00            ; so it resets the counter
    STX spriteCounter
    LDX #$01            ; and sets the firstPass flag to 1
    STX firstPass       

skipEndFirstPass
    JMP vblankwaitLoop

pattern1:                   ; receives one paramenter, wich is the position of the sprite to be moved
    LDA $0200, x            ; first it loads its vertical position
    CMP #$AF                ; if it has reached the bottom of the box
    BEQ skipVerticalIncrease   
    CLC               ; resets to the top
    ADC #$01
    STA $0200, x
skipVerticalIncrease:          ; if not just goes down by one
	LDA p1direction         ; checks direction to move sprite
	CMP #$00                ; 0 == right; 1 == left
	BNE p1left

p1right:
    LDA $0203, x
    CMP #$af                ; if going to the right checks if it has reached the right wall
    BEQ endp1               ; if it has it just goes down
    CLC
    ADC #01                 ; if not goes one to the right
	JMP endp1

p1left:                   ; same as p1right but to the left (duh)
    LDA $0203, x
    CMP #$49
    BEQ endp1
	SEC
    SBC #$01

endp1:
	STA $0203, x
	RTS

changep1dir:            ; changes direction of sprites after first pass
    LDA #$00
    STA spriteCounter         ; resets the spriteCounter
    LDA p1direction       ; if it was going to the right (a == 0) sets p1direction to 1 (goes to changeLeft)
    CMP #$00              ; else sets p1direction to 0 (goes to changeRight)
    BEQ changeLeft
changeRight:
    LDA #$00
    JMP setDir
changeLeft:
    LDA #$01
setDir:
    STA p1direction
    LDX #$00
    RTS

changep1dirfp:          ; does exactly the same as changep1dir but to p1firstpassdir instead of p1direction
    LDA #$00
    STA p1firstpassdir
    LDA p1direction
    CMP #$00
    BEQ changeLeft2
changeRight2:
    LDA #$00
    JMP setDir2
changeLeft2:
    LDA #$01
setDir2:
    STA p1direction
    LDX #$00
    RTS

