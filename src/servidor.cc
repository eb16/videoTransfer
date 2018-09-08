#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8888
#define MY_IP "127.0.0.1"
#define MAX_QUEUE 10
#define MAX_MSG_SIZE 1024
#define TRUE 1

void *tcp_handler(void *);
void *udp_handler();

int main(){
    //primitiva SOCKET
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    printf("\nprimitiva socket");

    //primitiva BIND
    struct sockaddr_in server_addr;
    socklen_t server_addr_size = sizeof server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(MY_IP);
    bind(
        server_socket,
        (struct sockaddr *)&server_addr, server_addr_size);
    printf("\n socketbindeado");

    //primitiva LISTEN
    listen(server_socket, MAX_QUEUE);
    printf("\n comienza a escuchar");

    pthread_t thread_id;

    while (1){
        printf("\n espero una conexion - accept...");
        //primitiva ACCEPT
        struct sockaddr_in client_addr;
        socklen_t client_addr_size = sizeof client_addr;
        int socket_to_client = accept(
            server_socket,
            (struct sockaddr *)&client_addr, &client_addr_size);
        //Thread control (TCP)
        pthread_create(&thread_id, NULL, tcp_handler, (void *)&socket_to_client);

        //primitiva CLOSE
    }
    close(server_socket);

    //CLOSE del socket que espera conexiones

    return 0;
}
void *udp_handler(){

    while (TRUE){
        /*
		if(estados.status[i] == 1)
		{
			if(cap no abierto)
				abrir cap

			cap.read();
			sock.sendto(estados.port[i], currentFrame)
			waitKey()

			else if(estados.status[i] == 0)
				sleep 1
			else if(estados.status[i] == 2)
				cerrar cap
		}
		*/
        break;
    }
    return 0;
}

void *tcp_handler(void *socket_desc){
    printf("\nRecibio conexion\n");

    int sock = *(int *)socket_desc;

    //primitiva RECEIVE
    char data[MAX_MSG_SIZE];
    int data_size = MAX_MSG_SIZE;
    int is_connected = 1;

    while (is_connected){
        int received_data_size = recv(sock, data, data_size, 0);

        std::string message = data;
        // puts(message.substr(0, 1).c_str());
        std::string play = "play";
        std::string pause = "pause";
        std::string stop = "stop";
        std::string init = "init";

        if (message.compare(0, play.length(), play) == 0){
            printf("Envio play\n");
        }else if (message.compare(0, pause.length(), pause) == 0){
            printf("Envio pause\n");
        }else if (message.compare(0, stop.length(), stop) == 0){
            printf("Envio stop\n");
        }else if (message.compare(0, init.length(), init) == 0){
            std::string puerto_str = message.substr(init.length() + 1, received_data_size - 2).c_str();
            int puerto = std::stoi(puerto_str);
            printf("%d\n", puerto);
        }else
            printf("Escribio cualquier cosa\n");

        //primitiva SEND
        int sent_data_size = send(sock, data, received_data_size, 0);
        printf("Enviado al cliente (%d bytes): %s\n", sent_data_size, data);
        is_connected = 0;
    } // Funciona con muchas terminales. Ahora que el servidor sepa el port.

    close(sock);
    return 0;
}