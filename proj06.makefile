# Project 6 #

proj06: proj06.student.o
	g++ proj06.student.o -o proj06

proj06.student.o: proj06.student.c
	g++ -std=c++11 -Wall -c proj06.student.c

clean:
	rmx -f proj06 proj06.student.o
