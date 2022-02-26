make:
	g++ prog1.cpp -o prog1
	g++ prog2.cpp -o prog2

clean:
	rm prog1 prog2

test:
	./prog1 '{ "progname": "./prog2", "url": "https://www.mail.ru" }'
