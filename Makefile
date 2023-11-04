.PHONY: all clean

all:
	$(MAKE) -C src
	mkdir -p build
	mkdir -p build/data
	mv src/xor build/
	mv src/detection build/
	mv src/solver build/
	mv src/processing build/
	cp -r src/data/* build/data/
	rm -rf src/data

clean:
	$(MAKE) -C src clean
	rm -rf build
