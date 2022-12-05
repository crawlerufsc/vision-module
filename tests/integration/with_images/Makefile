all:
	rm -rf build
	mkdir  -p build/net
	cp ../../../neuralnet/trained/classes.txt build/net
	cp ../../../neuralnet/trained/colors.txt build/net
	cp ../../../neuralnet/trained/hrnet_w18.onnx build/net
	cp ../../../neuralnet/trained/rtkmodel_test.onnx build/net
	cd build && cmake ..
	cd build && make

clean:
	rm -rf build

