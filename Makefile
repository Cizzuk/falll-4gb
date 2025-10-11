LCC = $${HOME}/gbdk/bin/lcc -Wm-ys -Wm-yc
SRC = src/
BUILD = build/

.PHONY: all clean

all:
	@mkdir -p build
	$(LCC) -o $(BUILD)/build.gb $(SRC)/*.c

clean:
	rm -r $(BUILD)
