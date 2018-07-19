.PHONY: multithreads
multithreads: multithreads.c
	gcc -g -Wall -o multithreads multithreads.c -lpthread

.PHONY: multiprocess
multiprocess: multiprocess.c
	gcc -g -Wall -o multiprocess multiprocess.c


.PHONY: multiprocess1
multiprocess1: multiprocess1.c
	gcc -g -Wall -o multiprocess1 multiprocess1.c


.PHONY: multicore
multicore: multicore.c
	gcc -g -Wall -o multicore multicore.c -lpthread

.PHONY: multicpu
multicpu: multicpu.c
	gcc -g -Wall -o multicpu multicpu.c


.PHONY: clean
clean:
	rm -rf *.o multithreads multiprocess multiprocess1 multicore
