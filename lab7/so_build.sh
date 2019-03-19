gcc -fPIC -c mystr.c
ld -shared -o libmystr.so.1.1 mystr.o

