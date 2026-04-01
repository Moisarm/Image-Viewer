LIB = -lglfw -lGL

all:
	mkdir -p build 
	$(CC) -o build/image-viewer *.c $(LIB)
clean:
	rm -rf build