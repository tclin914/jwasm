all: jwasm

OBJ = JavaClassLoader.o UTF8.o Reader.o JavaClass.o JavaConstantPool.o

jwasm: jwasm.cpp $(OBJ) 
	g++ $< -o $@ $(OBJ)

clean:
	rm jwasm
	rm $(OBJ)
