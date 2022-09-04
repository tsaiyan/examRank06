int usersID[64 * 1024];
int maxUserSocket = 0;
int nextUserID = 0;

char readMessageBuffer[42*4096], writeMessageBuffer[42*4096];

fd_set activeFd, readyReadFd, readyWriteFd; // https://ru.manpages.org/fd_set/2
