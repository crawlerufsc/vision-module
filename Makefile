all: bin

bin:
	rm -rf build
	mkdir  -p build/net
	cp neuralnet/trained/classes.txt build/net
	cp neuralnet/trained/colors.txt build/net
	cp neuralnet/trained/hrnet_w18.onnx build/net
	cd build && cmake ..
	cd build && make
	cp build/vision-bin .

clean:
	rm -rf build
	rm vision-bin

