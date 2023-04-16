output: main.o commands.o nav_command.o
	g++ main.o commands.o nav_command.o -lreadline -o shell379

main.o: main.cpp commands.h
	g++ -c main.cpp

commands.o: commands.cpp commands.h
	g++ -c commands.cpp

nav_command.o: nav_command.cpp nav_command.h
	g++ -c nav_command.cpp

clean:
	rm *.o shell379