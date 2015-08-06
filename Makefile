TARGET = json2module
CC = gcc

json2module:
	$(CC) -o $(TARGET) src/*.c 

test:
	./json2module test_json/test1.json -n test1 -o out_file -p com.test
	./json2module test_json/test2.json -n test2 -o out_file --objc
	./json2module test_json/test3.json -n test3 -o out_file --objc --java

.PHONY : clean
clean :
	rm -f *.o json2module out_file/*