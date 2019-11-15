CC=g++
CCFLAGS=-std=gnu++11 -O3
SDLFLAG=$$(sdl2-config --cflags --libs)

SRC=./src
TST=./tst
RES=./res
BIN=./bin
LOG=./log
EXT=./ext
NES=./bin/nesemu
GAMEDIR=./UNRELATED/
GAMESRC=${GAMEDIR}main.asm
GAME=./bin/UNRELATED.nes

TESTS=$(addprefix ${BIN}/, $(notdir $(patsubst %.s,%,$(sort $(wildcard ${TST}/*.s)))))
CROSS_AS=${EXT}/asm6f/asm6f

all: ${BIN} ${LOG} ${NES}

${NES}:
	${CC} ${CCFLAGS} ${SRC}/*.cpp  -o ${NES} ${SDLFLAG}

${GAME}: ${BIN}
	$(MAKE) -C ${GAMEDIR} all

${BIN}:
	@mkdir -p ${BIN}
	$(MAKE) -C ./ext/asm6f/ all

${BIN}/%: ${TST}/%.s
	${CROSS_AS} $^ $@

${LOG}:
	@mkdir -p ${LOG}

test: ${BIN} ${LOG} ${NES} ${TESTS}
	@{  echo "************************* Tests ******************************"; \
		test_failed=0; \
		test_passed=0; \
		for test in ${TESTS}; do \
			result="${LOG}/$$(basename $$test).log"; \
			expected="${RES}/$$(basename $$test).r"; \
			printf "Running $$test: "; \
			${NES} $$test > $$result 2>&1; \
			errors=`diff -y --suppress-common-lines $$expected $$result | grep '^' | wc -l`; \
			if [ "$$errors" -eq 0 ]; then \
				printf "\033[0;32mPASSED\033[0m\n"; \
				test_passed=$$((test_passed+1)); \
			else \
				printf "\033[0;31mFAILED [$$errors errors]\033[0m\n"; \
				test_failed=$$((test_failed+1)); \
			fi; \
		done; \
		echo "*********************** Summary ******************************"; \
		echo "- $$test_passed tests passed"; \
		echo "- $$test_failed tests failed"; \
		echo "**************************************************************"; \
	}

test-game: ${BIN} ${NES} ${GAME}
	${CC} ${CCFLAGS} ${SRC}/*.cpp  -o ${NES} ${SDLFLAG}
	${NES} ${GAME}

clean:
	rm -rf ${BIN} ${LOG} ./ext/asm6f/asm6f ${GAME}

res:
	for f in log/*.log;do cp $f res/$(basename "$f" .log).r;done

