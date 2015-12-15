all: jwasm

OBJ = Reader.o BytecodeDecoder.o

jwasm: jwasm.cpp $(OBJ)
	g++ $< -o $@ $(OBJ)

BytecodeDecoder.o: BytecodeDecoder.cpp
	g++ $< -c -o $@

Reader.o: Reader.cpp
	g++ $< -c -o $@

clean:
	rm jwasm
	rm $(OBJ)
