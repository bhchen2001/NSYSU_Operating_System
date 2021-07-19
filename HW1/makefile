CC = gcc
FILE1=B083040012_HW1
FILE2=command_line_processing

all:run

run:$(FILE1).c $(FILE1).o $(FILE1)
	$(CC) -c $(FILE1).c
	$(CC) -o $(FILE1) $(FILE1).o
	./$(FILE1)

# run:$(FILE1)
# 	./$(FILE1)

# $(FILE1):$(FILE1).o
# 	$(CC) -o $(FILE1) $(FILE1).o
# $(FILE1).o:$(FILE1).c
# 	$(CC) -c $(FILE1).c

clean:
	rm -f $(FILE1)
	rm -f $(FILE1).o