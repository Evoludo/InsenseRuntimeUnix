CC = gcc -Wall

ifdef DAL_DEBUG
DALDEBUG=-DDALDEBUG
endif

ifdef DAL_LOCAL
DALLOCAL=-DDALLOCAL
endif


CFLAGS = -g -c -DDALSMALL -DDALTEST $(DALDEBUG) $(DALLOCAL) 
RMCMD = rm -f

TestFindMinimumHopPath: TestFindMinimumHopPath.o Makefile
	$(CC) -o TestFindMinimumHopPath TestFindMinimumHopPath.o


TestBitMatrix: TestBitMatrix.o BitMatrix.o BitMatrix.h \
	Makefile BitMatrix.h
	$(CC) -o TestBitMatrix TestBitMatrix.o BitMatrix.o 

Test_DAL_mem: Person.o DAL_mem.o Test_DAL_mem.o DAL_error.o
	$(CC) -o Test_DAL_mem Person.o DAL_mem.o Test_DAL_mem.o DAL_error.o

TestList: DAL_mem.o DAL_error.o DAL_random.o Person.o \
	IteratedList.o TestList.o 
	$(CC) -o TestList DAL_mem.o DAL_error.o DAL_random.o Person.o \
	IteratedList.o TestList.o

TestMap: BSTMap.o DAL_mem.o FunctionPair.o TestMap.o
	$(CC) -o TestMap BSTMap.o DAL_mem.o FunctionPair.o TestMap.o

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	$(RMCMD) *.o

vclean: clean
	$(RMCMD) *~ *.exe*

