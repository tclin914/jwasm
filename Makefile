all: jwasm

# OBJ = Reader.o BytecodeDecoder.o
OBJ = JavaClassLoader.o UTF8.o

jwasm: jwasm.cpp $(OBJ)
	g++ $< -o $@ $(OBJ)

JavaClassLoader.o: JavaClassLoader.cpp
	g++ $< -c -o $@

UTF8.O: UTF8.cpp
	g++ $< -c -o $@

# BytecodeDecoder.o: BytecodeDecoder.cpp
	# g++ $< -c -o $@

# Reader.o: Reader.cpp
	# g++ $< -c -o $@

clean:
	rm jwasm
	rm $(OBJ)
