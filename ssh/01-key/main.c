#include <libssh/libssh.h>
#include <stdio.h>
#include <stdlib.h>

void execute_command(ssh_session session, const char *command) {
    ssh_channel channel = ssh_channel_new(session);
    if (channel == NULL) {
        fprintf(stderr, "Error creating SSH channel\n");
        exit(EXIT_FAILURE);
    }

    int rc = ssh_channel_open_session(channel);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error opening SSH session: %s\n", ssh_get_error(session));
        ssh_channel_free(channel);
        exit(EXIT_FAILURE);
    }

    rc = ssh_channel_request_exec(channel, command);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error executing command: %s\n", ssh_get_error(session));
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    int nbytes;
    while ((nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0)) > 0) {
        fwrite(buffer, 1, nbytes, stdout);
    }

    ssh_channel_send_eof(channel);
    ssh_channel_close(channel);
    ssh_channel_free(channel);
}

int main() {
    ssh_session session = ssh_new();
    if (session == NULL) {
        fprintf(stderr, "Error creating SSH session\n");
        exit(EXIT_FAILURE);
    }

    ssh_options_set(session, SSH_OPTIONS_HOST, "dev");
    ssh_options_set(session, SSH_OPTIONS_USER, "ivan");
    ssh_options_parse_config(session, "config.d/dev");

    int rc = ssh_connect(session);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error connecting to server: %s\n", ssh_get_error(session));
        ssh_free(session);
        exit(EXIT_FAILURE);
    }

    ssh_key key;
    if (ssh_pki_import_privkey_file("/home/ivan/.ssh/id_rsa_dev", NULL, NULL, NULL, &key) != SSH_OK) {
        fprintf(stderr, "Error loading private key: %s\n", ssh_get_error(session));
        ssh_free(session);
        exit(EXIT_FAILURE);
    }

    rc = ssh_userauth_publickey(session, NULL, key);
    ssh_key_free(key);

    if (rc != SSH_AUTH_SUCCESS) {
        fprintf(stderr, "Error authenticating with public key: %s\n", ssh_get_error(session));
        ssh_disconnect(session);
        ssh_free(session);
        exit(EXIT_FAILURE);
    }

    printf("Output for 'pwd':\n");
    execute_command(session, "pwd");
    printf("\nOutput for 'ls':\n");
    execute_command(session, "ls");
    printf("\nOutput for 'gcc -v':\n");
    execute_command(session, "gcc -v");

    ssh_disconnect(session);
    ssh_free(session);

    return 0;
}
