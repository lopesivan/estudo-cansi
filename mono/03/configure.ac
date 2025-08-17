# configure.ac

# Define o nome e a versão do pacote e o endereço de e-mail do mantenedor.
AC_INIT([libsum], [0.02], [ivan@ivanlopes.eng.br],
        [libsum], [http://ivanlopes.eng.br/])

# Define a versão mínima do Autoconf necessária para gerar o script configure.
AC_PREREQ([2.68])
LT_PREREQ([2.2])

# Put autotools auxiliary files in a subdir, so they don't clutter top dir:
AC_CONFIG_AUX_DIR([build-aux])

Inicializa o Automake e define opções adicionais de compilação.
AM_INIT_AUTOMAKE([1.11 -Wall -Werror])

# Checks for programs.
AC_PROG_CC
AC_PROG_CC_C_O

# We use libraries
AC_PROG_RANLIB
AM_PROG_AR

LT_INIT([dlopen])

# Verifico se o programa existe no PATH, se existe carrego a variável de
# mesmo nome mas em maiúscula com o nome do programa

AC_PATH_PROG(MCS, mcs, "")
AC_PATH_PROG(MONO, mono, "")

# se as variáveia abaixo não forem vazias eu crio a variável
# `CONFIG_MONO' do Makefile.am.
AM_CONDITIONAL(CONFIG_MONO, [
  test ! \( -z "$MCS" -o \
            -z "$MONO"   \)])

# Criando comando
AC_CONFIG_COMMANDS([Test],
                   [echo "========= $mypkgname =========";
				   echo make -Csrc run ],
                   [mypkgname=$PACKAGE_NAME])

AC_CONFIG_FILES([
    Makefile
    src/Makefile
])

AC_OUTPUT

