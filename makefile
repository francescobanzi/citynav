citynav: main.o map.o
    gcc -o citynav main.o map.o

main.o: main.c map.h
    gcc -c main.c

map.o: map.c map.h
    gcc -c map.c 

clean:
    rm -f main.o map.o 