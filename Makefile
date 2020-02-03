uspm: main.c parser.c iu.c
	@gcc -lcjson -lcurl -o uspm main.c -lm

clean:
	@rm uspm

install:
	@mv ./uspm /usr/local/bin/

uninstall:
	@rm /usr/local/bin/uspm