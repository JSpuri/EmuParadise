BIN=../bin
EXT=../ext
NES=../bin/nesemu
GAMESRC=main.asm
GAME=../bin/UNRELATED.nes
CROSS_AS=${EXT}/asm6f/asm6f

all: ${BIN} ${GAME}

${BIN}:
	@mkdir -p ${BIN}
	$(MAKE) -C ./ext/asm6f/ all

${GAME}: ${BIN}
	${CROSS_AS} ${GAMESRC} ${GAME}

clean:
	rm ${GAME}
