.PHONY: all clean

all:
	$(MAKE) -C src
	mkdir -p build
	cp -r data/* build/
	cp src/ui/*glade* build/
	cp src/ui/style.css build/
	cp src/run build/

clean:
	$(MAKE) -C src clean
	rm -rf build
