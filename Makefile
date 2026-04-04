FLAGS = -lglfw -lGL

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

TARGET = $(BUILD_DIR)/image-viewer
SOURCE = $(SRC_DIR)/*.c
all:
	mkdir -p build 
	$(CC) -o $(TARGET) $(SOURCE)  $(FLAGS)
clean:
	rm -rf $(BUILD_DIR)
run:
	$(TARGET)