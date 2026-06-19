CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -isystem external

SRC = main.cpp src/Image.cpp src/CLI.cpp src/Graph.cpp src/UnionFind.cpp src/FelzenszwalbSegmenter.cpp src/MST.cpp src/CoustySegmenter.cpp src/IFTSegmenter.cpp src/PriorityQueue.cpp
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

run-cousty: $(TARGET)
	$(RUN_PREFIX)$(TARGET) --input data/input/flor.jpg --output data/output/cousty.png --method cousty --threshold 40

run-ift: $(TARGET)
	$(RUN_PREFIX)$(TARGET) --input data/input/flor.jpg --output data/output/ift.png --method ift --seeds data/input/seeds.txt

# Compila e roda o teste do metodo IFT.
test-ift:
	$(CXX) $(CXXFLAGS) tests/test_ift.cpp src/Graph.cpp src/IFTSegmenter.cpp src/PriorityQueue.cpp -o test_ift$(EXE)
	$(RUN_PREFIX)test_ift$(EXE)

.PHONY: all clean run-example run-gray run-cousty run-ift test-ift
