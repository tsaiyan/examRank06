else {
    readMessageBuffer[readBytes - 1] = 0;
    sprintf(writeMessageBuffer, "client %d: %s\n", usersID[socket], readMessageBuffer);
    sendMessageToAllUsers(socket);
}
}
}
}
}

