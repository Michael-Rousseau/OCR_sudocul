SRC_DIRS := $(wildcard */)

.PHONY: all clean $(SRC_DIRS)

all: $(SRC_DIRS)

clean:
	$(RM) -r $(SRC_DIRS)

$(SRC_DIRS):
	$(MAKE) -C $@
