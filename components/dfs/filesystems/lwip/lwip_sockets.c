/*
 * File      : lwip_sockets.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2015, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2015-02-17     Bernard      First version
 */

#include <dfs.h>
#include <dfs_def.h>
#include <dfs_posix.h>

#include <sys/socket.h>

#include "dfs_lwip.h"

int accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
    int sock = dfs_lwip_getsocket(s);

    return lwip_accept(sock, addr, addrlen);
}

int bind(int s, const struct sockaddr *name, socklen_t namelen)
{
    int sock = dfs_lwip_getsocket(s);
    
    return lwip_bind(sock, name, namelen);
}

int shutdown(int s, int how)
{
    int sock = dfs_lwip_getsocket(s);

    return lwip_shutdown(s, how);
}

int getpeername (int s, struct sockaddr *name, socklen_t *namelen)
{
    int sock = dfs_lwip_getsocket(s);

    return lwip_getpeername(sock, name, namelen);
}

int getsockname (int s, struct sockaddr *name, socklen_t *namelen)
{
    int sock = dfs_lwip_getsocket(s);
    
    return lwip_getsockname(sock, name, namelen);
}

int getsockopt (int s, int level, int optname, void *optval, socklen_t *optlen)
{
    int sock = dfs_lwip_getsocket(s);
    
    return lwip_getsockopt(sock, level, optname, optval, optlen);
}

int setsockopt (int s, int level, int optname, const void *optval, socklen_t optlen)
{
    int sock = dfs_lwip_getsocket(s);
    
    return lwip_setsockopt(sock, level, optname, optval, optlen);
}

int connect(int s, const struct sockaddr *name, socklen_t namelen)
{
    int sock = dfs_lwip_getsocket(s);
    
    return lwip_connect(sock, name, namelen);
}

int listen(int s, int backlog)
{
    int sock = dfs_lwip_getsocket(s);
    
    return lwip_listen(sock, backlog);
}

int recv(int s, void *mem, size_t len, int flags)
{
    int sock = dfs_lwip_getsocket(s);
    
    return lwip_recv(sock, mem, len, flags);
}

int recvfrom(int s, void *mem, size_t len, int flags,
      struct sockaddr *from, socklen_t *fromlen)
{
    int sock = dfs_lwip_getsocket(s);
    
    return lwip_recvfrom(sock, mem, len, flags, from, fromlen);
}

int send(int s, const void *dataptr, size_t size, int flags)
{
    int sock = dfs_lwip_getsocket(s);
    
    return lwip_send(sock, dataptr, size, flags);
}

int sendto(int s, const void *dataptr, size_t size, int flags,
    const struct sockaddr *to, socklen_t tolen)
{
    int sock = dfs_lwip_getsocket(s);
    
    return lwip_sendto(sock, dataptr, size, flags, to, tolen);
}

int socket(int domain, int type, int protocol)
{
    /* create a BSD socket */
    int fd;
    int sock;
    struct dfs_fd *d;

    /* allocate a fd */
    fd = fd_new();
    if (fd < 0)
    {
        rt_set_errno(-DFS_STATUS_ENOMEM);

        return -1;
    }
    d = fd_get(fd);

    /* create socket in lwip and then put it to the dfs_fd */
    sock = lwip_socket(domain, type, protocol);
    if (sock > 0)
    {
        /* this is a socket fd */
        d->type = FT_SOCKET;
        d->path = RT_NULL;

        d->fs = dfs_lwip_get_fs();
        
        d->flags = DFS_O_RDWR; /* set flags as read and write */
        d->size = 0;
        d->pos  = 0;
        
        /* set socket to the data of dfs_fd */
        d->data = (void*) sock;
    }

    /* release the ref-count of fd */
    fd_put(d);

    return fd;
}
