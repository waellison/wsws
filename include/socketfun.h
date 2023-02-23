#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef _SOCKETFUN_H
#define _SOCKETFUN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Represent a socket's ip address in a human-readable format. */
char *inadport_decimal(struct sockaddr_in *sad);

/* Serve a socket at the given hostname and port number, returning
   the file descriptor of the associated socket. */
int serve_socket(char *hostname, int port);

/* Accept a connection on the passed socket, returning the file
   descriptor of the associated connection.  The returned FD is
   read/write, as you would likely expect.
   
   @warning: To be called only by the server */
int accept_connection(int fildes);

/* Connect to the passed port on the passed server, returning the
   file descriptor of the connection.  As with `accept_connection`,
   the resulting file descriptor is read/write.

   @warning: To be called only by the client */
int request_connection(char *hostname, int port);

#ifdef __cplusplus
}
#endif

#endif /* defined _SOCKETFUN_H */

