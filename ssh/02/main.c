#include <libssh2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void execute_ssh_command(LIBSSH2_SESSION *session, const char *command) {
    LIBSSH2_CHANNEL *channel;
    int rc;
    char buffer[256];
    ssize_t nbytes;

    channel = libssh2_channel_open_session(session);
    if (!channel) {
        exit(-1);
    }

    rc = libssh2_channel_exec(channel, command);
    if (rc != 0) {
        libssh2_channel_free(channel);
        exit(-1);
    }

    nbytes = libssh2_channel_read(channel, buffer, sizeof(buffer));
    while (nbytes > 0) {
        fwrite(buffer, 1, nbytes, stdout);
        nbytes = libssh2_channel_read(channel, buffer, sizeof(buffer));
    }

    libssh2_channel_send_eof(channel);
    libssh2_channel_wait_eof(channel);
    libssh2_channel_wait_closed(channel);
    libssh2_channel_free(channel);
}

int main() {
    const char *hostname = "192.168.2.43";
    const char *username = "ivan";
    const char *password = "XXXXXXXXX";
    int port = 22;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        exit(-1);
    }

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = inet_addr(hostname);
    if (connect(sock, (struct sockaddr *)(&sin), sizeof(struct sockaddr_in)) != 0) {
        perror("connect");
        exit(-1);
    }

    LIBSSH2_SESSION *session = libssh2_session_init();
    if (!session) {
        exit(-1);
    }

    int rc = libssh2_session_handshake(session, sock);
    if (rc != 0) {
        fprintf(stderr, "Error during SSH handshake: %d\n", rc);
        exit(-1);
    }

    rc = libssh2_userauth_password(session, username, password);
    if (rc != 0) {
        fprintf(stderr, "Error during SSH authentication: %d\n", rc);
        exit(-1);
    }

    printf("Executing 'pwd' command:\n");
    execute_ssh_command(session, "pwd");

    printf("\nExecuting 'ls' command:\n");
    execute_ssh_command(session, "ls");

    printf("\nExecuting 'gcc -v' command:\n");
    execute_ssh_command(session, "gcc -v");

    libssh2_session_disconnect(session, "Normal Shutdown");
    libssh2_session_free(session);

    close(sock);

    return 0;
}
