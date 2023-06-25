vmobj: main.cpp vm/instructions.cpp vm/virtual_machine.cpp
	c++ -c -o build/o/main.o main.cpp
	c++ -c -o build/o/instructions.o vm/instructions.cpp
	c++ -c -o build/o/virtual_machine.o vm/virtual_machine.cpp
vmbuild: build/o/main.o build/o/instructions.o build/o/virtual_machine.o
	c++ -o build/_vmt build/o/main.o build/o/instructions.o build/o/virtual_machine.o
vm: build/o/main.o build/o/instructions.o build/o/virtual_machine.o
	c++ -o build/_vm build/o/main.o build/o/instructions.o build/o/virtual_machine.o