all:
	g++ assembler.cpp -o sicasm
	g++ interpreter.cpp -o sicsim

clean:
	rm sicasm sicsim *.out
