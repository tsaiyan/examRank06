if (FD_ISSET(socket, &readyReadFd) && socket != serverSocket) {
    // чтение данных из сокета
    int readBytes = recv(socket, readMessageBuffer, 42*4096, 0);
    // если ошибка, пользак отключился
    if (readBytes <= 0) {
        sprintf(writeMessageBuffer, "server: client %d just left\n", usersID[socket]);
        sendMessageToAllUsers(socket);
        FD_CLR(socket, &activeFd);
        close(socket);
        break;
    }
