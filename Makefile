CC = gcc
# pkg-config automaticaly gets the include paths (-i) and libraries (-l)
# added cglm, freetype2, fontconfig and harfbuzz for Runara right compilance
CFLAGS = -I$(INCLUDE_DIR) `pkg-config --cflags glfw3 cglm freetype2 fontconfig harfbuzz`
LIBS = `pkg-config --libs glfw3 cglm freetype2 fontconfig harfbuzz` -lGL -lm -lclipboard

# Runara's path
RUNARA_LDFLAGS = -Linclude/runara/lib -lrunara

#directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

TARGET = $(BUILD_DIR)/image-viewer
SOURCE = $(SRC_DIR)/*.c

all:
	mkdir -p $(BUILD_DIR)
	$(CC) -o $(TARGET) $(SOURCE) $(CFLAGS) $(RUNARA_LDFLAGS) $(LIBS)

clean:
	rm -rf $(BUILD_DIR)

run:
	./$(TARGET)