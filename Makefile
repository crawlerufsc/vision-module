all: bin

bin:
	rm -rf build
	mkdir -p build/net
	cp neuralnet/trained/classes.txt build/net
	cp neuralnet/trained/colors.txt build/net
	cp neuralnet/trained/rtkmodel_test.onnx build/net
	cp neuralnet/trained/hrnet_w18.onnx build/net
	cd build && cmake ..
	cd build && make -j4
	cp build/vision-bin .
	cp -R build/net .
	rm -rf build

clean:
	rm -rf build
	rm vision-bin
	rm net

