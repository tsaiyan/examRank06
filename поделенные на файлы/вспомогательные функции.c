void fatal_error() {
    write(2, "Fatal error\n", 12);
    exit(1);
}

// берет сообщение из буффера и отправляет всем пользователям через сокет, кроме того что в параметре
void sendMessageToAllUsers(int currentUserSocket) {
    for (int i = 0; i <= maxUserSocket; i++)
        if (FD_ISSET(i, &readyWriteFd) && i != currentUserSocket)
            send(i, writeMessageBuffer, strlen(writeMessageBuffer), 0);
}
