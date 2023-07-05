CC = gcc
OUTBIN = testExecutable
INCLUDE_DIR = ../Inc
SOURCE_DIR = ../Src
UNITY_FOLDER = Unity/src

test : test.c $(SOURCE_DIR)/mpu_driver.c
	$(CC) -o $(OUTBIN) -iquote $(INCLUDE_DIR) -iquote $(UNITY_FOLDER) -Wall $(UNITY_FOLDER)/unity.c $^
	./$(OUTBIN)
	
clean:
	rm -f $(OUTBIN)