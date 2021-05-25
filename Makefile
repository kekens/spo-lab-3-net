all: main.c
	gcc file_ctrl/file_controller.c -o spo_net -lssl -lcrypto main.c