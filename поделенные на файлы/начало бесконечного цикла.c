while (1) {
    readyReadFd = readyWriteFd = activeFd;
    if (select(maxUserSocket + 1, &readyReadFd, &readyWriteFd, NULL, NULL) < 0)
        continue;
