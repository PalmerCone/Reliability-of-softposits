POSIT = SoftPosit/build/Linux-x86_64-GCC/softposit.a \
	-ISoftPosit/source/include -O2
READ = -lm
READP = -lm 

GITLAB = https://gitlab.com/cerlane/SoftPosit
build:
	git clone $(GITLAB)
	make -j6 all -C SoftPosit/build/Linux-x86_64-GCC
read:
	gcc $(READ) Read.c Read.h $(POSIT)
readp:
	gcc $(READP) ReadP.c Read.h $(POSIT)
movetests:
	mv *.txt tests
run:
	python runner.py 10 35
clean:
	rmdir test_*
	rm a.out Read readP test_* -f -d \
