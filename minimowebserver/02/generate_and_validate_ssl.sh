#!/bin/bash

# Definindo variáveis
KEY_FILE="theKey.pem"
CERT_FILE="fullChain.pem"
SUBJECT="/C=BR/ST=Rio de Janeiro/L=Rio de Janeiro/O=UERJ/CN=Ivan Lopes/emailAddress=lopesivan.ufrj@gmail.com"

# Gerando a chave privada e o certificado autoassinado
openssl req -newkey rsa:2048 -nodes -keyout $KEY_FILE -x509 -days 365 -out $CERT_FILE -subj "$SUBJECT"

# Validando a chave privada
echo "Validando a chave privada..."
openssl rsa -in $KEY_FILE -check

# Validando o certificado
echo "Validando o certificado..."
openssl x509 -in $CERT_FILE -text -noout

echo "Geração e validação concluídas."
