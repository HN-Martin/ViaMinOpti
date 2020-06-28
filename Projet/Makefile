GCC_FLAGS=-Wall -g 
EXEC= TestInit TestIntersec TestBalayage TestGrapheInit

InitOBJ= F_Init.o SVGwriter.o
IntersecOBJ= F_Intersec.o
BalayageOBJ= AVL.o Tas.o F_Balayage.o
GrapheOBJ= GrapheInit.o GrapheCycle.o
OBJ= $(InitOBJ) $(IntersecOBJ) $(BalayageOBJ) $(GrapheOBJ)


all: $(EXEC)

clean:
	@rm -f *.o
	
cleanall: clean
	rm -f $(EXEC)

TestInit: F_Init.o SVGwriter.o Init/TestInit.c 
	gcc ${GCC_FLAGS} -o TestInit $^

TestIntersec: F_Init.o F_Intersec.o Intersec/TestIntersec.c
	gcc ${GCC_FLAGS} -o TestIntersec $^
	
TestBalayage: F_Init.o AVL.o Tas.o T.o F_Balayage.o Balayage/TestBalayage.c
	gcc ${GCC_FLAGS} -o TestBalayage $^

TestGrapheInit: F_Init.o SVGwriter.o GrapheInit.o GrapheCycle.o Graphe/TestGrapheInit.c
	gcc ${GCC_FLAGS} -o TestGrapheInit $^

F_Init.o: Init/F_Init.c
	gcc ${GCC_FLAGS} -c -o $@ $^
	
SVGwriter.o: Init/SVGwriter.c
	gcc ${GCC_FLAGS} -c -o $@ $^
	
F_Intersec.o: Intersec/F_Intersec.c
	gcc ${GCC_FLAGS} -c -o $@ $^
	
AVL.o:	Balayage/AVL.c
	gcc ${GCC_FLAGS} -c -o $@ $^

Tas.o: Balayage/Tas.c
	gcc ${GCC_FLAGS} -c -o $@ $^

T.o: Balayage/T.c
	gcc ${GCC_FLAGS} -c -o $@ $^

F_Balayage.o: Balayage/F_Balayage.c
	gcc ${GCC_FLAGS} -c -o $@ $^
	
GrapheInit.o: Graphe/GrapheInit.c
	gcc ${GCC_FLAGS} -c -o $@ $^
	
GrapheCycle.o: Graphe/GrapheCycle.c
	gcc ${GCC_FLAGS} -c -o $@ $^
	
.PHONY: clean all
