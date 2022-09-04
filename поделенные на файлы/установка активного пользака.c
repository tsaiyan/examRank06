FD_SET(serverSocket, &activeFd);

if ((bind(serverSocket, (const struct sockaddr *)&addr, sizeof(addr))) < 0)
    fatal_error();
