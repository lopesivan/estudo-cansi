
KITS = bezier.so cpp.so square.so template.so tm.so tsoding.so
SOS  = $(addprefix ./build/lib,$(KITS))

init:
	cc -o nob nob.c
	./nob

libbezier.so:
	./build/panim ./build/$@

libcpp.so:
	./build/panim ./build/$@

libsquare.so:
	./build/panim ./build/$@

libtemplate.so:
	./build/panim ./build/$@

libtm.so:
	./build/panim ./build/$@

libtsoding.so:
	./build/panim ./build/$@

clean:
	rm $(SOS) ./build/panim nob
	rm -rf build/
