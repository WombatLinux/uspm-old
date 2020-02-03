uspm: main.c parser.c
	gcc -lcjson -lcurl -o uspm main.c -lm

clean:
	rm uspm