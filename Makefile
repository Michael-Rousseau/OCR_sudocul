SRC_DIRS := $(wildcard */)

.PHONY: all clean $(SRC_DIRS)

all: $(SRC_DIRS)

clean: $(addsuffix -clean, $(SRC_DIRS))

$(SRC_DIRS):
	$(MAKE) -C $@

%-clean:
	$(MAKE) -C $* clean
