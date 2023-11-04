.PHONY: all clean backtracking ocr

all: backtracking ocr detection processing

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
	$(MAKE) -C edge_detection
	mv edge_detection/detection .
	mkdir -p data
	cp -r edge_detection/data/* data

processing:
	$(MAKE) -C image_processing
	mv image_processing/processing .
	mkdir -p data
	cp -r image_processing/data/* data

clean:
	$(MAKE) -C ocr clean
	$(MAKE) -C backtracking clean
	$(MAKE) -C image_processing clean
	$(MAKE) -C edge_detection clean
	$(RM) solver xor detection processing
	rm -rf data
