CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -isystem external

SRC = main.cpp src/Image.cpp src/CLI.cpp src/Graph.cpp src/UnionFind.cpp src/FelzenszwalbSegmenter.cpp
OUT = segmentador

ifeq ($(OS),Windows_NT)
EXE = .exe
RM = del /Q /F
RUN_PREFIX = .\\
else
EXE =
RM = rm -f
RUN_PREFIX = ./
endif

TARGET = $(OUT)$(EXE)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	$(RM) $(TARGET)

run-example: $(TARGET)
	$(RUN_PREFIX)$(TARGET) --input data/input/flor.jpg --output data/output/saida.png --method copy --color

run-gray: $(TARGET)
	$(RUN_PREFIX)$(TARGET) --input data/input/flor.jpg --output data/output/cinza.png --method copy --gray

.PHONY: all clean run-example run-gray