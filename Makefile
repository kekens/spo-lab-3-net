all: main.c
	gcc file_controller/file_controller.c net/tcp.c net/udp.c -o spo_net -pthread -lssl -lcrypto main.c