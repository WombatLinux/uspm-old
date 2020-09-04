uspm: uspm/uspm.c uspm/fm.c uspm/iu.c uspm/dephandle.c
	@mkdir -p "build"
	@gcc -lcjson -lcurl -o build/uspm main.c -lm

clean:
	@rm -rf build

install:
	@mv ./build/uspm /usr/local/bin/

uninstall:
	@rm /usr/local/bin/uspm
