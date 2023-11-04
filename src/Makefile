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
	$(MAKE) -C image_processing
	mv image_processing/detection .
	mkdir -p data
	cp -r image_processing/data/* data

clean:
	$(MAKE) -C ocr clean
	$(RM) solver xor detection
	rm -rf data
