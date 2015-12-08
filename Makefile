all: bytecode2wasm

OBJ = Reader.o BytecodeDecoder.o

bytecode2wasm: bytecode2wasm.cpp $(OBJ)
	g++ $< -o $@ $(OBJ)

BytecodeDecoder.o: BytecodeDecoder.cpp
	g++ $< -c -o $@

Reader.o: Reader.cpp
	g++ $< -c -o $@

clean:
	rm bytecode2wasm
	rm $(OBJ)
