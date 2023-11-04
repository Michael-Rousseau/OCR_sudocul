.PHONY: all clean backtracking ocr

all: backtracking ocr

ocr:
	$(MAKE) -C ocr
	mv ocr/xor .
	mkdir -p data
	cp -r ocr/data/* data/

backtracking:
	$(MAKE) -C backtracking
	mv backtracking/solver .
	mkdir -p data
	cp -r backtracking/data/* data

clean:
	$(MAKE) -C ocr clean
	$(RM) solver xor
	rm -rf data
