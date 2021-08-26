run:	my-look.c my-rev.c	
	gcc my-look.c -o my-look -Wall -Werror
	gcc my-rev.c -o my-rev -Wall -Werror

clean:	
	rm -f my-look my-rev
