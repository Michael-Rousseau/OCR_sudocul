.PHONY: all clean backtracking ocr

all: backtracking ocr

ocr:
	$(MAKE) -C ocr
	mv ocr/xor .
	mkdir data
	cp -r ocr/data/* data/

clean:
	$(MAKE) -C ocr clean
	$(RM) solver xor
	rm -rf data
