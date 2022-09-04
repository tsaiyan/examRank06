int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
if (serverSocket < 0)
    fatal_error();

maxUserSocket = serverSocket;
