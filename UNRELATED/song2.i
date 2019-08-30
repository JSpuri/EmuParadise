song2_header:
    .byte $02           ;2 streams
    
    .byte MUSIC_SQ1     ;which stream
    .byte $01           ;status byte (stream enabled)
    .byte SQUARE_1      ;which channel
    .byte %00110111          ;initial volume (7) and duty (00)
    .word song2_square1 ;pointer to stream
    .byte $42		;tempo
    
    .byte MUSIC_SQ2     ;which stream
    .byte $01           ;status byte (stream enabled)
    .byte SQUARE_2      ;which channel
    .byte %11110111          ;initial volume (7) and duty (10)
    .word song2_square2 ;pointer to stream
    .byte $42		;tempo
    
    ;.byte MUSIC_TRI     ;which stream
    ;.byte $01           ;status byte (stream enabled)
    ;.byte TRIANGLE      ;which channel
    ;.byte $81           ;initial volume (on)
    ;.word song1_tri     ;pointer to stream
    ;.byte $40		;tempo
    
    ;.byte MUSIC_NOI     ;which stream
    ;.byte $00           ;disabled.  Our load routine will skip the
                        ;   rest of the reads if the status byte is 0.
                        ;   We are disabling Noise because we haven't covered it yet.
    
song2_square1:
    .byte sixteenth, Bb2, Db3, C3, Db3, Bb2, rest, Bb2, C3, Db3, Ab3, F3, rest, Bb2, Db3, C3, Db3, Bb2, rest, Bb2, C3, Db3, Ab3, F3, rest, Bb2, Db3, C3, Db3, Bb2, rest, Bb2, C3, Db3, Ab3, F3, rest
    .byte eighth, C3, Ab2, C3, Ab2, sixteenth, C3, Ab2, F3, Ab2
	.byte sixteenth, Bb2, Db3, C3, Db3, Bb2, rest, Bb2, C3, Db3, Ab3, F3, rest, Bb2, Db3, C3, Db3, Bb2, rest, Bb2, C3, Db3, Ab3, F3, rest, Bb2, Db3, C3, Db3, Bb2, rest, Bb2, C3, Db3, Ab3, F3, rest
	.byte eighth, Ab2, Eb2, F2, Ab2, C3, Eb3
	.byte sixteenth, Gb2, Bb3, C3, Db3, Bb2, F2, Gb2, Bb3, C3, Db3, Bb2, Eb3
	.byte Ab2, C4, Eb2, F2, Ab2, F2, eighth, Eb2, F2, Ab2
	.byte sixteenth, Bb2, Bb3, Eb3, F3, Db3, F2, Bb2, Bb3, Eb3, F3, Db3, F2
	.byte Bb2, Bb3, Eb3, F3, Db3, F2, Ab2, Ab3, eighth, Eb2, Ab2
	.byte Fs2, rest, rest, Fs2, rest, rest
	.byte Fs2, rest, rest, Fs2, rest, rest
	.byte Bb2, rest, rest, Bb2, rest, rest
	.byte C3, Ab3, C4, sixteenth, Ab3, Ab2, Bb2, C3, Eb3, C3
	.byte eighth, Fs2, rest, rest, Fs2, rest, rest
	.byte Fs2, rest, rest, Fs2, sixteenth, rest, Ab2, Bb2, C3
	.byte Bb2, C3, Db3, Eb3, F3, Bb3, Db4, C4, Ab3, F3, Db3, C3
	.byte eighth, Ab2, Eb2, F2, Ab2, C3, Eb3
	.byte sixteenth, Gb2, Bb3, C3, Db3, Bb2, F2, Gb2, Bb3, C3, Db3, Bb2, Eb3
	.byte Ab2, C4, Eb2, F2, Ab2, F2, eighth, Eb2, F2, Ab2
	.byte sixteenth, Bb2, Bb3, Eb3, F3, Db3, F2, Bb2, Bb3, Eb3, F3, Db3, F2
	.byte Bb2, Bb3, Eb3, F3, Db3, F2, eighth, Ab2, Eb2, F2
	.byte sixteenth, Eb2, Eb3, Ab2, Bb2, Fs2, Eb2, Eb2, Eb3, Ab2, Bb2, eighth, C3
	.byte sixteenth, F2, F3, Ab2, Ab3, F3, C3, F2, F3, Fs2, Fs3, Ab2, Ab3
	.byte sixteenth, Fs2, Fs3, Db3, Eb3, F3, F2, Fs2, Fs3, Db3, Eb3, F3, Fs2
    .byte F2, F3, Ab2, Ab3, F3, C3, F2, F3, Fs2, Fs3, Ab2, Ab3
	.byte Eb2, Eb3, Bb2, C3, Db3, F2, Eb2, Eb3, Bb2, C3, Db3, Eb2
	.byte Ab2, Eb2, C3, Ab2, F3, C3, Ab3, Ab2, Eb3, Ab3, C3, Ab3
	.byte Bb2, Eb2, C3, Eb2, Db3, Eb2, B2, Db3, Eb3, Fs2, Bb2, Db3
	.byte B2, Fs2, Db3, Fs2, Eb3, Fs2, Fs3, B2, Ab3, Db3, Db4, Ab3
	.byte B2, Fs2, Db3, Fs2, Eb3, Fs2, B2, Fs2, Db3, Fs2, Eb3, Fs2
	.byte Fs3, B2, Ab3, Db3, Db4, Eb3, eighth, B3, Fs4, B4
	.byte sixteenth, B2, Eb3, Db3, Eb3, B2, rest, B2, Db3, Eb3, B3, Fs3, rest
	.byte B2, Eb3, Db3, Eb3, B2, rest, B2, Db3, Eb3, B3, Fs3, rest
	.byte Bb2, Db3, C3, Db3, Bb2, rest, Bb2, C3, Db3, Ab3, F3, rest
	.byte eighth, Eb2, Ab2, C3, Eb3, sixteenth, C3, Ab2, F3, C3
	.byte sixteenth, B2, Eb3, Db3, Eb3, B2, rest, B2, Db3, Eb3, B3, Fs3, rest
	.byte B2, Eb3, Db3, Eb3, B2, rest, B2, Db3, Eb3, B3, Fs3, rest
	.byte Bb2, Db3, C3, Db3, Bb2, rest, Bb2, C3, Db3, Ab3, F3, rest
	.byte eighth, Bb2, F2, Ab2, F2, F3, F2
	.byte sixteenth, B2, Eb3, Db3, Eb3, B2, rest, B2, Db3, Eb3, B3, Fs3, rest
	.byte B2, Eb3, Db3, Eb3, B2, rest, B2, Db3, Eb3, B3, Fs3, rest
	.byte Bb2, Db3, C3, Db3, Bb2, rest, Bb2, C3, Db3, Ab3, F3, rest
	.byte eighth, Bb2, F2, Ab2, F2, F3, F2
	.byte sixteenth, B2, Eb3, Db3, Eb3, B2, rest, B2, Db3, Eb3, B3, Fs3, rest
	.byte B2, Eb3, Db3, Eb3, B2, rest, B2, Db3, Eb3, B3, Fs3, rest
	.byte Bb2, Db3, C3, Db3, Bb2, rest, Bb2, C3, Db3, Ab3, F3, rest
	.byte eighth, Bb2, F2, Ab2, F2, F3, F2
	.byte sixteenth, B2, Eb3, Db3, Eb3, B2, rest, B2, Db3, Eb3, B3, Fs3, rest
	.byte B2, Eb3, Db3, Eb3, B2, rest, B2, Db3, Eb3, B3, Fs3, rest
	.byte Bb2, Db3, C3, Db3, Bb2, rest, Bb2, C3, Db3, Ab3, F3, rest
	.byte eighth, Bb2, F2, Ab2, F2, F3, F2
	.byte sixteenth, F2, Db3, C3, Db3, Bb2, rest, F2, C3, Db3, Ab3, F3, rest
	.byte Ab2, Db3, C3, Db3, Bb2, rest, Ab2, C3, Db3, C4, Db3, rest
	.byte eighth, Bb2, Eb3, Bb3, Bb2, Eb3, Bb3
	.byte eighth, Ab2, Eb2, F2, Ab2, C3, Eb3
	.byte sixteenth, Gb2, Bb3, C3, Db3, Bb2, F2, Gb2, Bb3, C3, Db3, Bb2, Eb3
	.byte Ab2, C4, Eb2, F2, Ab2, F2, eighth, Eb2, F2, Ab2
	.byte sixteenth, Bb2, Bb3, Eb3, F3, Db3, F2, Bb2, Bb3, Eb3, F3, Db3, F2
	.byte Bb2, Bb3, Eb3, F3, Db3, F2, eighth, Ab2, Eb2, F2
	.byte sixteenth, Eb2, Eb3, Ab2, Bb2, Fs2, Eb2, Eb2, Eb3, Ab2, Bb2, eighth, C3
	.byte sixteenth, F2, F3, Ab2, Ab3, F3, C3, F2, F3, Fs2, Fs3, Ab2, Ab3
	.byte sixteenth, Fs2, Fs3, Db3, Eb3, F3, F2, Fs2, Fs3, Db3, Eb3, F3, Fs2
    .byte F2, F3, Ab2, Ab3, F3, C3, F2, F3, Fs2, Fs3, Ab2, Ab3
	.byte Eb2, Eb3, Bb2, C3, Db3, F2, Eb2, Eb3, Bb2, C3, Db3, Eb2
	.byte Ab2, Eb2, C3, Ab2, F3, C3, Ab3, Ab2, Eb3, Ab3, C3, Ab3
	.byte Fs2, Fs3, Db3, Eb3, F3, F2, Fs2, Fs3, Db3, Eb3, F3, Fs2
	.byte F2, F3, Ab2, Ab3, F3, C3, F2, F3, Fs2, Fs3, Ab2, Ab3
	.byte Bb2, Bb3, Eb3, F3, Db3, F2, Bb2, Bb3, Eb3, F3, Db3, F2
	.byte eighth, Bb2, sixteenth, rest, eighth, Bb2, sixteenth, rest, eighth, Bb2, Bb2, Bb2
    .byte $FF
    
song2_square2:
    .byte quarter, rest, rest, rest
	.byte quarter, rest, rest, rest
	.byte quarter, rest, rest, rest
	.byte eighth, Eb3, C3, Bb3, sixteenth, Ab3, Fs3, eighth, F3, Ab3
	.byte sixteenth, Bb3, rest, Bb3, rest, Bb3, rest, Bb3, rest, Bb3, rest, Bb3, rest
	.byte quarter, rest, rest, rest
	.byte sixteenth, Db3, rest, Db3, rest, Db3, rest, Db3, rest, Db3, rest, Db3, rest
	.byte eighth, Eb4, C5, Bb4, sixteenth, Ab4, Fs4, eighth, F4, Ab4
	.byte half, Bb4, eighth, F4, Ab4
	.byte quarter, Bb4, eighth, Db5, sixteenth, Eb5, Db5, eighth, C5, Db5
	.byte quarter, Bb4, Bb4, Bb4
	.byte quarter, rest, rest, rest
	.byte rest, rest, rest
	.byte rest, rest, rest
	.byte rest, rest, rest
	.byte eighth, Eb4, C5, Bb4, sixteenth, Ab4, Fs4, eighth, F4, Ab4
	.byte Bb4, Gb4, Gb3, rest, sixteenth, Bb3, Ab3, Bb3, rest
	.byte rest, rest, Gb4, F4, Bb4, F4, rest, Db4, Ab3, rest, rest, rest
	.byte Db4, Eb4, Ab4, Bb4, C5, Db5, Eb5, Db5, C5, Bb4, Ab4, Eb4
	.byte eighth, Eb4, C5, Bb4, sixteenth, Ab4, Fs4, eighth, F4, Ab4
	.byte half, Bb4, eighth, F4, Ab4
	.byte quarter, Bb4, eighth, Db5, sixteenth, Eb5, Db5, eighth, C5, Db5
	.byte quarter, Bb4, Bb4, Bb4
	.byte eighth, rest, rest, rest, F4, Bb4, Db5
	.byte half, F5, eighth, Db5, Eb5
	.byte quarter, F5, eighth, Eb5, sixteenth, Db5, Eb5, eighth, C5, Ab4
	.byte half, Bb4, eighth, Bb4, Db5
	.byte quarter, C5, eighth, Bb4, sixteenth, Ab4, Fs4, eighth, F4, Eb4
	.byte half, F4, eighth, Db5, Eb5
	.byte quarter, F5, eighth, Eb5, sixteenth, Db5, Eb5, eighth, C5, Ab4
	.byte eighth, Bb4, Bb4, Bb4, Eb4, Eb4, Eb4
	.byte Fs4, Fs4, Fs4, Db5, Db5, Db5
	.byte sixteenth, Eb5, Eb5, rest, Eb5, rest, Eb5, Eb5, rest, Eb5, rest, Eb5, rest
	.byte Eb5, Eb5, rest, Eb5, rest, Eb5, Eb5, rest, Fs5, rest, Ab5, rest
	.byte quarter, rest, rest, rest
	.byte rest, rest, rest
	.byte thirtysecond, E5, F5, Fs5, G5, sixteenth, Ab5, rest, Ab5, rest, G5, rest, G5, rest, eighth, G5
	.byte sixteenth, rest, thirtysecond, F5, Fs5, eighth, Ab5, sixteenth, Ab5, rest, eighth, Ab5, sixteenth, G5, F5, eighth, Eb5 
	.byte sixteenth, F5, rest, F5, rest, F5, rest, F5, rest, F5, rest, F5, rest
	.byte quarter, rest, rest, rest
	.byte thirtysecond, E5, F5, Fs5, G5, sixteenth, Ab5, rest, Ab5, rest, G5, rest, G5, rest, eighth, G5
	.byte Eb4, Db5, Bb4, sixteenth, Ab4, Fs4, eighth, F4, Ab4
	.byte quarter, Bb4, eighth, B4, Eb5, Db5, Eb5
	.byte quarter, F5, eighth, Eb5, Fs5, F5, Eb5
	.byte quarter, F5, eighth, Fs5, sixteenth, F5, Fs5, eighth, Eb5, F5 
	.byte quarter, Db5, eighth, Eb5, sixteenth, C5, Db5, eighth, Bb4, Ab4
	.byte quarter, F4, eighth, Fs4, quarter, Ab4, eighth, Bb4
	.byte quarter, F4, eighth, Fs4, quarter, Ab4, eighth, Bb4
	.byte sixteenth, F4, Fs4, quarter, F4, rest, eighth, rest
	.byte quarter, rest, rest, rest
	.byte quarter, Bb4, eighth, B4, Eb5, Db5, Eb5
	.byte quarter, F5, eighth, Eb5, Fs5, F5, Eb5
	.byte quarter, F5, eighth, Fs5, sixteenth, F5, Fs5, eighth, Eb5, F5 
	.byte quarter, Db5, eighth, Eb5, sixteenth, C5, Db5, eighth, Bb4, Ab4
	.byte quarter, F4, eighth, Fs4, quarter, Ab4, eighth, Bb4
	.byte quarter, F4, eighth, Fs4, quarter, Ab4, eighth, Bb4
	.byte sixteenth, F4, Fs4, quarter, F4, half, F4
	.byte eighth, Eb4, C5, Bb4, sixteenth, Ab4, Fs4, eighth, F4, Ab4
	.byte half, Bb4, eighth, F4, Ab4
	.byte quarter, Bb4, eighth, Db5, sixteenth, Eb5, Db5, eighth, C5, Db5
	.byte quarter, Bb4, Bb4, Bb4
	.byte eighth, rest, rest, rest, F4, Bb4, Db5
	.byte half, F5, eighth, Db5, Eb5
	.byte quarter, F5, eighth, Eb5, sixteenth, Db5, Eb5, eighth, C5, Ab4
	.byte half, Bb4, eighth, Bb4, Db5
	.byte quarter, C5, eighth, Bb4, sixteenth, Ab4, Fs4, eighth, F4, Eb4
	.byte half, F4, eighth, Db5, Eb5
	.byte quarter, F5, eighth, Eb5, sixteenth, Db5, Eb5, eighth, C5, Ab4
	.byte half, Bb4, eighth, Db5, Eb5
	.byte quarter, F5, eighth, Eb5, sixteenth, Db5, Eb5, eighth, C5, Ab4
	.byte quarter, Bb4, Bb4, Bb4
	.byte Bb4, Bb4, Bb4
    .byte $FF
    
;song1_tri:
    ;.byte thirtysecond, F6, D6, B5, D6, B5, Gs5, B5, Gs5, F5, Gs5, F5, D5, F5, D5, B4, Gs4
    ;.byte E6, Db6, Bb5, Db6, Bb5, G5, Bb5, G5, E5, G5, E5, Db5, E5, Db5, Bb4, G4
    ;.byte $FF
