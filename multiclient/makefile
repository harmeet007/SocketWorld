echos: echos.o echo.o
	gcc echos.o -o echos
	gcc echo.o -o echo

echos.o: echos.c
	gcc -c echos.c

echo.o: echo.c
	gcc -c echo.c

clean:
	rm *.o core
