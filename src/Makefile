.PHONY: all clean backtracking ocr

all: backtracking ocr detection

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

detection:
	$(MAKE) -C hough
	mv hough/detection .
	mkdir -p data
	cp -r hough/data/* data

clean:
	$(MAKE) -C ocr clean
	$(RM) solver xor detection
	rm -rf data
