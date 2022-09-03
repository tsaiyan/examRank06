#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>

// sprintf - запись текста в массив
// bind - присваивает адрес (127.0.0.1: port) сокету https://ru.manpages.org/bind/2

int usersID[64 * 1024];
int maxUserSocket = 0;
int nextUserID = 0;

char readMessageBuffer[42*4096], writeMessageBuffer[42*4096];

fd_set active, readyRead, readyWrite; // https://ru.manpages.org/fd_set/2

void fatal_error() {
    write(2, "Fatal error\n", 12);
    exit(1);
}

// берет сообщение из буффера и отправляет всем пользователям через сокет, кроме отправителя
void sendMessageToAllUsers(int currentUserSocket) {
    for (int i = 0; i <= maxUserSocket; i++)
        if (FD_ISSET(i, &readyWrite) && i != currentUserSocket)
            send(i, writeMessageBuffer, strlen(writeMessageBuffer), 0);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        write(2, "Wrong number of arguments\n", 26);
        exit(1);
    }
    
    // считанный порт и адрес
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(2130706433); // берем из сабджекта
    addr.sin_port = htons(atoi(argv[1]));
    
    // зануление
    bzero(&usersID, sizeof(usersID));
    FD_ZERO(&active);

    // открытие сокета, команды гуглить или можно обойтись без них хардкодом
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
        fatal_error();

    maxUserSocket = serverSocket;
    // установить пользака активным
    FD_SET(serverSocket, &active);

    if ((bind(serverSocket, (const struct sockaddr *)&addr, sizeof(addr))) < 0)
        fatal_error();
    
    // listen -
    if (listen(serverSocket, 128) < 0)
        fatal_error();

    while (1) {
        readyRead = readyWrite = active; // это фдшники или сокеты?
        // select - проверка всех фдшников на готовность
        if (select(maxUserSocket + 1, &readyRead, &readyWrite, NULL, NULL) < 0)
            continue; // при первом запуске он выходит тут

        for (int socket = 0; socket <= maxUserSocket; socket++) {
            if (FD_ISSET(socket, &readyRead) && socket == serverSocket) { // находит клиента
                int userSocket = accept(serverSocket, (struct sockaddr *)&addr, &addr_len);
                if (userSocket < 0)
                    continue; // не нашел других клиентов

                maxUserSocket = (userSocket > maxUserSocket) ? userSocket : maxUserSocket;
                usersID[userSocket] = nextUserID; // записывает номер пользака
                nextUserID += 1;
                FD_SET(userSocket, &active);

                sprintf(writeMessageBuffer, "server: client %d just arrived\n", usersID[userSocket]);
                sendMessageToAllUsers(userSocket);
                break;
            }

            // MARK: - чтение сообщения
            
            if (FD_ISSET(socket, &readyRead) && socket != serverSocket) {
                // чтение данных из сокета
                int readBytes = recv(socket, readMessageBuffer, 42*4096, 0);
                // если ошибка, пользак отключился
                if (readBytes <= 0) {
                    sprintf(writeMessageBuffer, "server: client %d just left\n", usersID[socket]);
                    sendMessageToAllUsers(socket);
                    FD_CLR(socket, &active);
                    close(socket);
                    break;
                }
                
                // MARK: - получение сообщения

                else {
                    readMessageBuffer[readBytes - 1] = 0;
                    sprintf(writeMessageBuffer, "client %d: %s\n", usersID[socket], readMessageBuffer);
                    sendMessageToAllUsers(socket);
                }
            }
        }
    }
}
