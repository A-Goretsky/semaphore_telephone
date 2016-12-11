compile_control: control.c compile_client
	gcc -o control control.c

compile_client: client.c
	gcc -o client client.c
