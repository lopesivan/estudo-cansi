#include <libssh/libssh.h>
#include <libssh/sftp.h>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

    const char *local_file_path = "/home/ivan/wget.txt";
    const char *remote_file_path = "remote_file.txt";

    ssh_session session = ssh_new();
    if (session == NULL) {
        fprintf(stderr, "Error creating SSH session\n");
        exit(EXIT_FAILURE);
    }

    ssh_options_set(session, SSH_OPTIONS_HOST, "dev");
    ssh_options_set(session, SSH_OPTIONS_USER, "ivan");
    ssh_options_parse_config(session, "/home/ivan/.ssh/config.d/dev");

    int rc = ssh_connect(session);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error connecting to server: %s\n", ssh_get_error(session));
        ssh_free(session);
        exit(EXIT_FAILURE);
    }

    rc = ssh_userauth_publickey_auto(session, NULL, NULL);
    if (rc != SSH_AUTH_SUCCESS) {
        fprintf(stderr, "Error authenticating with password: %s\n", ssh_get_error(session));
        ssh_disconnect(session);
        ssh_free(session);
        exit(EXIT_FAILURE);
    }

    sftp_session sftp = sftp_new(session);
    if (sftp == NULL) {
        fprintf(stderr, "Error creating SFTP session: %s\n", ssh_get_error(session));
        ssh_disconnect(session);
        ssh_free(session);
        exit(EXIT_FAILURE);
    }

    rc = sftp_init(sftp);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error initializing SFTP session: %d\n", sftp_get_error(sftp));
        sftp_free(sftp);
        ssh_disconnect(session);
        ssh_free(session);
        exit(EXIT_FAILURE);
    }

    FILE *local_file = fopen(local_file_path, "rb");
    if (local_file == NULL) {
        perror("Error opening local file");
        sftp_free(sftp);
        ssh_disconnect(session);
        ssh_free(session);
        exit(EXIT_FAILURE);
    }

    sftp_file remote_file = sftp_open(sftp, remote_file_path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (remote_file == NULL) {
        fprintf(stderr, "Error opening remote file: %d\n", sftp_get_error(sftp));
        fclose(local_file);
        sftp_free(sftp);
        ssh_disconnect(session);
        ssh_free(session);
        exit(EXIT_FAILURE);
    }

    char buffer[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), local_file)) > 0) {
        if (sftp_write(remote_file, buffer, bytes_read) != bytes_read) {
            fprintf(stderr, "Error writing to remote file: %d\n", sftp_get_error(sftp));
            sftp_close(remote_file);
            fclose(local_file);
            sftp_free(sftp);
            ssh_disconnect(session);
            ssh_free(session);
            exit(EXIT_FAILURE);
        }
    }

    sftp_close(remote_file);
    fclose(local_file);
    sftp_free(sftp);
    ssh_disconnect(session);
    ssh_free(session);

    return 0;
}
