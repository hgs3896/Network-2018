CC = gcc
.SUFFIXES : .c .o

INC = -I include/
SRC = src/

SERVERDIR = server/
CLIENTDIR = client/
TESTDIR = test/
TESTCASEDIR = $(TESTDIR)testcases/

CFLAGS += -g $(INC)

all: 
	make server
	make client

server: $(SRC)server.c | $(SERVERDIR)
	$(CC) $(CFLAGS) $< -o $(SERVERDIR)$@

$(SERVERDIR):
	mkdir -p $@

client: $(SRC)client.c | $(CLIENTDIR)
	$(CC) $(CFLAGS) $< -o $(CLIENTDIR)$@

$(CLIENTDIR):
	mkdir -p $@

test: | $(TESTCASEDIR)
	
$(TESTCASEDIR):
	mkdir -p $(TESTCASEDIR)
	python3 $(TESTDIR)tc_generator.py
	
clean:
	rm $(SERVERDIR)server
	rm $(CLIENTDIR)client
	rm ./testfile*
	rm $(TESTDIR)testfile*
	rm $(TESTCASEDIR)testfile*
