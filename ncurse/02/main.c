#include <ncurses.h>

int main() {
  initscr();            // inicializa a tela
  cbreak();             // desativa o buffer de entrada
  keypad(stdscr, TRUE); // permite a leitura de teclas de função, setas, etc.
  noecho();             // desativa o eco de entrada

  char *choices[] = {
      "Conectar",
      "Configurar",
      "Sair",
  };

  int choice;
  int highlight = 0;
  int num_choices = sizeof(choices) / sizeof(char *);

  while (1) {
    for (int i = 0; i < num_choices; ++i) {
      if (i == highlight)
        attron(A_REVERSE); // inverte as cores para destacar a seleção
      mvprintw(2 * i + 1, 2, "%s", choices[i]);
      attroff(A_REVERSE); // desativa a inversão de cores
    }

    choice = getch(); // lê a entrada do teclado

    switch (choice) {
    case KEY_UP:
      if (highlight > 0)
        --highlight;
      break;
    case KEY_DOWN:
      if (highlight < num_choices - 1)
        ++highlight;
      break;
    default:
      break;
    }
    if (choice == '\n') // se o usuário pressionou ENTER
    {
      break;
    }
  }

  printw("\nVocê escolheu: %s", choices[highlight]);

  refresh();
  getch(); // aguarda o usuário pressionar qualquer tecla

  endwin(); // encerra a janela
  return 0;
}
