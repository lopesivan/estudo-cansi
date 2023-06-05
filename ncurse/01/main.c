#include <ncurses.h>

int main() {
  initscr();            // Inicializa a biblioteca ncurses
  cbreak();             // Desabilita a espera por Enter
  noecho();             // Desabilita a exibição do input do usuário
  keypad(stdscr, TRUE); // Habilita o uso das teclas de função
  start_color();        // Inicializa suporte a cores

  attron(A_BOLD); // Ativa o texto em negrito

  // Imprime o menu
  mvprintw(10, 10, "1. Opção 1");
  mvprintw(11, 10, "2. Opção 2");
  mvprintw(12, 10, "3. Opção 3");

  attroff(A_BOLD); // Desativa o texto em negrito

  refresh(); // Atualiza a tela
  getch();   // Espera por uma tecla pressionada

  endwin(); // Encerra a biblioteca ncurses

  return 0;
}
