randmain: randmain.c randcpuid.c
	$(CC) $(CFLAGS) -o randmain.o -c randmain.c
	$(CC) $(CFLAGS) -o randcpuid.o -c randcpuid.c
	ar -cvq randcpuid.a randcpuid.o
	$(CC) $(CFLAGS) -o randmain randmain.o randcpuid.a -ldl -Wl,-rpath=$(PWD)

randlibhw.so: randlibhw.c
	$(CC) $(CFLAGS) -c -fPIC randlibhw.c -o randlibhw.o
	$(CC) $(CFLAGS) -shared -o randlibhw.so randlibhw.o

randlibsw.so: randlibsw.c
	$(CC) $(CFLAGS) -c -fPIC randlibsw.c -o randlibsw.o
	$(CC) $(CFLAGS) -shared -o randlibsw.so randlibsw.o
