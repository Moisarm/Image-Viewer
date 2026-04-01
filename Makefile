LIB = -lglfw -lGL

all:
	mkdir -p build 
	$(CC) -o build/image-viewer src/*.c  $(LIB)
clean:
	rm -rf build