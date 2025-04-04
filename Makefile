CC = g++
CFLAGS = -I"C:\Program Files\PostgreSQL\17\include" -Wall
LDFLAGS = -L"C:\Program Files\PostgreSQL\17\lib" -lpq

all: Cod_university

Cod_university: Cod_university.cpp
	$(CC) $(CFLAGS) Cod_university.cpp -o Cod_university $(LDFLAGS)

clean:
	del Cod_university.exe