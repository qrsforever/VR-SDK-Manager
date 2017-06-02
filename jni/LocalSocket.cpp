#include "LocalSocket.h"
#include "VrLog.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/un.h>

#define VR_MAX_BUFFLEN 256
#define VR_SOCKET_NONBLOCK 1

namespace VR {

LocalSocket::LocalSocket(const char *sockpath) : mSockPath(sockpath)
{
    mSockFD = _CreateSocket();
#if VR_SOCKET_NONBLOCK
    if (mSockFD > 0)
        fcntl(mSockFD, F_SETFL, O_NONBLOCK);
#endif
}

LocalSocket::~LocalSocket()
{
    if (mSockFD > 0)
        close(mSockFD);
    mSockFD = -1;
}

std::string
LocalSocket::acquire(std::string field)
{
    LOGI("LocalSocket::acquire (%s)\n", field.c_str());
    if (-1 == mSockFD)
        return "-E1";

    int retnum = -1;
    char tbuff[VR_MAX_BUFFLEN + 1] = { 0 };

    strncpy(tbuff, field.c_str(), VR_MAX_BUFFLEN);
    retnum = write(mSockFD, tbuff, strlen(tbuff));

    if (retnum > 0) {
        memset(tbuff, 0, VR_MAX_BUFFLEN);
#if VR_SOCKET_NONBLOCK
        int retval = -1;
        struct timeval tv;
        fd_set rset;
        FD_ZERO(&rset);
        FD_SET(mSockFD, &rset);
        tv.tv_sec = 0;
        tv.tv_usec = 100000;
AGAIN:
        retval = select(mSockFD + 1, &rset, 0, 0, &tv);
        if (-1 == retval) {
            if (EINTR == errno)
                goto AGAIN;
            return "-E2";
        }
        if (0 == retval)
            return "-E3";
        if (FD_ISSET(mSockFD, &rset))
            retnum = read(mSockFD, tbuff, VR_MAX_BUFFLEN);
#else
        retnum = read(mSockFD, tbuff, VR_MAX_BUFFLEN);
#endif
        return retnum > 0 ? std::string(tbuff) : "-E0";
    }
}

int
LocalSocket::_CreateSocket()
{
    LOGI("_CreateSocket\n");
    int connfd = -1;
    int ret = 0;
    static struct sockaddr_un sa;

    connfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(connfd<0) {
        LOGE("socket error: %s\n", strerror(errno));
        return -1;
    }
    sa.sun_family=AF_UNIX;
    strcpy(sa.sun_path, mSockPath.c_str());

    ret = connect(connfd, (struct sockaddr*)&sa, sizeof(sa));
    if(ret == -1) {
        close(connfd);
        LOGE("connect fail: %s\n", strerror(errno));
        return -1;
    }
    return connfd;
}

}
