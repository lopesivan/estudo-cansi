# configure.ac

dnl === Projeto / metadata ===
AC_INIT([libsum], [0.02], [ivan@ivanlopes.eng.br], [libsum], [http://ivanlopes.eng.br/])

dnl Requisito mínimo do Autoconf (ajuste se precisar)
AC_PREREQ([2.68])

dnl Coloca arquivos auxiliares do autotools em build-aux/
AC_CONFIG_AUX_DIR([build-aux])

dnl Diretório de macros locais (.m4)
AC_CONFIG_MACRO_DIRS([m4])

dnl === Automake ===
dnl Opções comuns: 'foreign' evita exigir arquivos GNU extras;
dnl -Wall pra avisos. Evitei -Werror para não travar builds por warnings.
AM_INIT_AUTOMAKE([foreign -Wall])

dnl === Ferramentas de compilação ===
AC_PROG_CC
AC_PROG_CC_C_O

dnl Para criação de libs estáticas com Automake moderno
AM_PROG_AR

dnl Libtool (substitui AC_PROG_LIBTOOL/AC_PROG_RANLIB)
LT_INIT([dlopen])

dnl === Descoberta de programas ===
dnl Se existirem no PATH, definem MCS e MONO
AC_PATH_PROG([MCS],  [mcs],  [])
AC_PATH_PROG([MONO], [mono], [])

dnl Se ambas as variáveis não forem vazias, ativa CONFIG_MONO no Makefile.am
AM_CONDITIONAL([CONFIG_MONO], [test -n "$MCS" -a -n "$MONO"])

dnl === Comandos extras na configuração (opcional) ===
AC_CONFIG_COMMANDS([Test],
[
  echo "========= $mypkgname ========="
  echo "make -C src run"
],
[mypkgname=$PACKAGE_NAME])

dnl === Arquivos gerados ===
AC_CONFIG_FILES([
  Makefile
  src/Makefile
])

AC_OUTPUT
