struct sockaddr_in addr;
socklen_t addr_len = sizeof(addr);
addr.sin_family = AF_INET;
addr.sin_addr.s_addr = htonl(2130706433); // берем из сабджекта
addr.sin_port = htons(atoi(argv[1]));
