all: jwasm

OBJ = JavaClassLoader.o UTF8.o Reader.o JavaClass.o JavaConstantPool.o

jwasm: jwasm.cpp $(OBJ) 
	g++ $< -o $@ $(OBJ)

JavaClassLoader.o: JavaClassLoader.cpp
	g++ $< -c -o $@

UTF8.o: UTF8.cpp
	g++ $< -c -o $@

Reader.o: Reader.cpp
	g++ $< -c -o $@

clean:
	rm jwasm
	rm $(OBJ)
