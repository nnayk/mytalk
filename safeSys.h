int safeSocket();
ssize_t safeSend(int, const void *, size_t, int);
ssize_t safeRecv(int, void *, size_t,int);
void safeListen(int, int);
int safeAccept(int, struct sockaddr *, socklen_t *);
void safeConnect(int,const struct sockaddr *, socklen_t);
ssize_t safeRead(int,void *,size_t);
ssize_t safeWrite(int, const void *, size_t);
