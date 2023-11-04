.PHONY: all clean

all:
	$(MAKE) -C src
	mkdir -p build
	mkdir -p build/data
	mv src/xor build/
	mv src/solver build/
	cp -r src/data/* build/data/

clean:
	$(MAKE) -C src clean
	rm -rf build
