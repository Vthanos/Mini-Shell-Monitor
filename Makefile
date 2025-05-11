all:
	gcc -o mini_shell main.c exec_utils.c monitor.c

clean:
	rm -f mini_shell
