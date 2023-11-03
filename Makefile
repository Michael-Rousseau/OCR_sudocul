.PHONY: all clean

all:
	$(MAKE) -C src
	mkdir build
	mkdir build/data
	mv src/xor build/
	cp -r src/data build/data/

clean:
	$(MAKE) -C src clean
	rm -rf build
