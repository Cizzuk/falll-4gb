LCCFLAGS = -Wm-yc -Wm-ynFALLLL
LCC = $${HOME}/gbdk/bin/lcc $(LCCFLAGS)

SRC = src/
BUILD = build/

.PHONY: all clean

all:
	@mkdir -p build
	$(LCC) -o $(BUILD)/falll.gb $(SRC)/*.c

clean:
	rm -r $(BUILD)
