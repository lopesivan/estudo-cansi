A execução de comandos SSH de um programa C pode ser feita usando a biblioteca libssh. Primeiro, instale a biblioteca libssh em seu sistema, se ainda não o fez. Aqui está um exemplo de código que se conecta a um servidor SSH, autentica-se e executa os comandos desejados:

Substitua your_remote_server_ip, your_remote_username, your_remote_server_port e your_remote_password pelos valores apropriados. Por razões de segurança, é recomendável usar a autenticação por chave em vez de senha. Para isso, você pode substituir a chamada de função ssh_userauth_password por ssh_userauth_publickey_auto e remover a opção de senha.

Compile e execute o programa com as seguintes instruções:

gcc -o ssh_commands ssh_commands.c -lssh
./ssh_commands

sudo apt-get install libssh-dev
