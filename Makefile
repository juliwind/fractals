CC = g++

main: $(CC) main.cpp -o main -lsfml-graphics -lsfml-window -lsfml-system

clean: rm -f main
