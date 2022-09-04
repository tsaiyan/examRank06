for (int socket = 0; socket <= maxUserSocket; socket++) {
    if (FD_ISSET(socket, &readyReadFd) && socket == serverSocket) { // находит клиента
        int userSocket = accept(serverSocket, (struct sockaddr *)&addr, &addr_len);
        if (userSocket < 0)
            continue; // не нашел других клиентов

        maxUserSocket = (userSocket > maxUserSocket) ? userSocket : maxUserSocket;
        usersID[userSocket] = nextUserID; // записывает номер пользака
        nextUserID += 1;
        FD_SET(userSocket, &activeFd);

        sprintf(writeMessageBuffer, "server: client %d just arrived\n", usersID[userSocket]);
        sendMessageToAllUsers(userSocket);
        break;
    }
