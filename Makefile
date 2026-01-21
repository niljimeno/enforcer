PREFIX?=/usr/X11R6
CFLAGS?=-Os -pedantic -Wall

all:
	rm -rf build/
	mkdir -p build/
	cc $(CFLAGS) -I$(PREFIX)/include src/main.c -L$(PREFIX)/lib -lX11 -o build/enforcer

init:
	# Xephyr -br -ac -noreset :1 &
	Xephyr -br -ac -noreset -screen 900x900 :1 &

run:
	# remember to run make init first!
	make
	DISPLAY=:1 ./build/enforcer dev

test:
	make
	./build/enforcer test

clean:
	rm -f ./build/enforcer
