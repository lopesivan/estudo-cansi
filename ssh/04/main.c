#include <stdio.h>
#include <stdlib.h>
#include <libssh/libssh.h>

void execute_ssh_command(ssh_session session, const char *command) {
    ssh_channel channel;
    int rc;

    channel = ssh_channel_new(session);
    if (channel == NULL) {
        exit(-1);
    }

    rc = ssh_channel_open_session(channel);
    if (rc != SSH_OK) {
        ssh_channel_free(channel);
        exit(-1);
    }

    rc = ssh_channel_request_exec(channel, command);
    if (rc != SSH_OK) {
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        exit(-1);
    }

    char buffer[256];
    int nbytes;
    nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
    while (nbytes > 0) {
        fwrite(buffer, 1, nbytes, stdout);
        nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
    }

    ssh_channel_send_eof(channel);
    ssh_channel_close(channel);
    ssh_channel_free(channel);
}

int main() {
    char* your_remote_server_ip ="192.168.2.43";
    char* your_remote_username = "ivan";
    int your_remote_server_port = 22;
    char* your_remote_password = "XXXXXX";

    ssh_session session;
    int rc;

    session = ssh_new();
    if (session == NULL) {
        exit(-1);
    }

    ssh_options_set(session, SSH_OPTIONS_HOST, your_remote_server_ip);
    ssh_options_set(session, SSH_OPTIONS_USER, your_remote_username);
    ssh_options_set(session, SSH_OPTIONS_PORT, &your_remote_server_port);

    rc = ssh_connect(session);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error connecting to server: %s\n", ssh_get_error(session));
        ssh_free(session);
        exit(-1);
    }

    rc = ssh_userauth_password(session, NULL, your_remote_password);
    if (rc != SSH_AUTH_SUCCESS) {
        fprintf(stderr, "Error authenticating with password: %s\n", ssh_get_error(session));
        ssh_disconnect(session);
        ssh_free(session);
        exit(-1);
    }

    const char *commands[] = {
        "pwd"   ,
        "ls"    ,
        "gcc -v",
    };
    int num_commands = sizeof(commands) / sizeof(commands[0]);

    for (int i = 0; i < num_commands; i++) {
        printf("\nExecuting '%s' command:\n", commands[i]);
        execute_ssh_command(session, commands[i]);
    }

    ssh_disconnect(session);
    ssh_free(session);

    return 0;
}
