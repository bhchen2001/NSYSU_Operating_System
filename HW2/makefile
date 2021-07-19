CC=gcc
FILE1=B083040012_HW2_part2
FLAG=-pthread
OUT = B083040012_HW2_part2

all: dep
	$(CC) -o $(OUT) *.o $(FLAG)

dep: 
	$(CC) $(FLAG) -c $(FILE1).c

clean:
	rm -f $(OUT).o
	rm -f $(OUT)

help:
	@printf "make clean dep all:			to make clean and all\n"