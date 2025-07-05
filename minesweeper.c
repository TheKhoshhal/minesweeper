#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof((arr)[0]))

typedef struct {
  int x;
  int y;
} Position;

typedef struct {
  int height;
  int width;
  int bomb_num;
} WinArg;

int startx, starty;
bool gameOver = 0;

Position cursor;

void initCurses() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
}

bool position_exists(int bomb_count, Position *bombs, Position pos) {
  for (int i = 0; i < bomb_count; i++) {
    if (bombs[i].x == pos.x && bombs[i].y == pos.y) {
      return true;
    }
  }
  return false;
}

void placeBombs(int bomb_count, Position *bombs, int height, int width) {
  srand(time(NULL));

  for (int i = 0; i < bomb_count;) {
    Position temp_pos;
    temp_pos.y = (rand() % height) + 1;
    temp_pos.x = (rand() % width) + 1;

    if (!position_exists(i, bombs, temp_pos)) {
      bombs[i] = temp_pos;
      ++i;
    }
  }
}

int countAdjacentBombs(Position cursor, Position *bombs, int bomb_num) {
  int count = 0;

  for (int dx = -1; dx <= 1; dx++) {
    for (int dy = -1; dy <= 1; dy++) {
      if (dx == 0 && dy == 0) {
        continue;
      }

      Position adjacent = {cursor.x + dx, cursor.y + dy};

      for (int i = 0; i < bomb_num; i++) {
        if (bombs[i].x == adjacent.x && bombs[i].y == adjacent.y) {
          count++;
          break;
        }
      }
    }
  }

  return count;
}

void removeBlocks(WINDOW *game_win, Position *bombs, Position cursor,
                  int bomb_num, int height, int width) {

  if (cursor.x < 1 || cursor.x > width || cursor.y < 1 || cursor.y > height) {
    return;
  }

  int x = mvwinch(game_win, cursor.y, cursor.x);
  if (x != 'O') {
    return;
  }

  int adjacent_bomb_num = countAdjacentBombs(cursor, bombs, bomb_num);

  if (adjacent_bomb_num > 0) {
    mvwprintw(game_win, cursor.y, cursor.x, "%d", adjacent_bomb_num);
    return;
  }

  mvwprintw(game_win, cursor.y, cursor.x, " ");
  for (int dx = -1; dx <= 1; dx++) {
    for (int dy = -1; dy <= 1; dy++) {
      if (dx == 0 && dy == 0)
        continue;
      Position new_pos;
      new_pos.x = cursor.x + dx;
      new_pos.y = cursor.y + dy;
      removeBlocks(game_win, bombs, new_pos, bomb_num, height, width);
    }
  }
}

bool checkBlocksLeft(WINDOW *game_win, WinArg winarg) {
  for (int i = 1; i <= winarg.width; i++) {
    for (int j = 1; j <= winarg.height; j++) {
      int screen_char = mvwinch(game_win, j, i);
      if (screen_char == 'O') {
        return true;
      }
    }
  }
  return false;
}

WinArg getArgs(int argc, char *argv[]) {
  int l_width;
  int l_height;
  int l_bomb_num;
  WinArg winarg = {};
  if (argc > 1) {
    if (!strcmp(argv[1], "small")) {
      l_bomb_num = 20;
      l_height = 10;
      l_width = 18;
    } else if (!strcmp(argv[1], "medium")) {
      l_bomb_num = 50;
      l_height = 18;
      l_width = 30;
    } else if (!strcmp(argv[1], "huge")) {
      l_bomb_num = 99;
      l_height = 30;
      l_width = 50;
    }
  } else {
    l_bomb_num = 20;
    l_height = 10;
    l_width = 18;
  }
  winarg.height = l_height;
  winarg.width = l_width;
  winarg.bomb_num = l_bomb_num;
  return winarg;
}

WINDOW *initGame(int height, int width, int starty, int startx, int bomb_num,
                 Position *bombs) {
  WINDOW *game_win;
  char exitmsg[] = "Press q to exit";
  mvprintw(0, COLS - sizeof(exitmsg), "%s", exitmsg);

  mvprintw(0, 0, "bombs left: %02d", bomb_num);
  refresh();

  game_win = newwin(height + 2, width + 2, starty, startx);
  box(game_win, 0, 0);
  mvwprintw(game_win, 0, 1, "minesweeper");
  for (int i = 1; i <= width; ++i) {
    for (int j = 1; j <= height; ++j) {
      mvwaddch(game_win, j, i, 'O');
    }
  }
  wmove(game_win, 1, 1);
  wrefresh(game_win);

  placeBombs(bomb_num, bombs, height, width);

  // TODO: remove this
  // mvprintw(4, 0, "%02d %02d", bombs[0].x, bombs[0].y);

  return game_win;
}

//TODO: add colors to see easier
int main(int argc, char *argv[]) {
  initCurses();

  WinArg winarg = getArgs(argc, argv);

  // Calculating for a center placement
  starty = (LINES - winarg.height) / 2;
  startx = (COLS - winarg.width) / 2;

  // cursor location
  cursor.x = 1;
  cursor.y = 1;

  // bombs left
  int bombs_left = winarg.bomb_num;

  Position bombs[winarg.bomb_num];

  WINDOW *game_win = initGame(winarg.height, winarg.width, starty, startx,
                              winarg.bomb_num, bombs);
  wmove(game_win, cursor.y, cursor.x);
  wrefresh(game_win);

  int ch;
  int temp;
  while (!gameOver && (ch = getch()) != 'q') {
    switch (ch) {
    case KEY_UP:
    case 'k':
      if (cursor.y > 1) {
        temp = cursor.y;
        cursor.y = --temp;
      }
      break;
    case KEY_DOWN:
    case 'j':
      if (cursor.y < winarg.height) {
        temp = cursor.y;
        cursor.y = ++temp;
      }
      break;
    case KEY_LEFT:
    case 'h':
      if (cursor.x > 1) {
        temp = cursor.x;
        cursor.x = --temp;
      }
      break;
    case KEY_RIGHT:
    case 'l':
      if (cursor.x < winarg.width) {
        temp = cursor.x;
        cursor.x = ++temp;
      }
      break;
    case 'f':
      if (bombs_left > 0) {
        --bombs_left;
        mvprintw(0, 0, "bombs left: %02d", bombs_left);
        mvwprintw(game_win, cursor.y, cursor.x, "f");
        wrefresh(game_win);
        refresh();
      } else {
        bool blocks_left = checkBlocksLeft(game_win, winarg);

        if (!blocks_left) {
          char gameWinMsg[] = "!!You Win!!";
          mvprintw(4, (COLS - strlen(gameWinMsg)) / 2, "%s", gameWinMsg);
          refresh();
          sleep(2);
          gameOver = 1;
        }
      }
      break;
    case 'b':
      for (int i = 0; i < winarg.bomb_num; i++) {
        if (bombs[i].x == cursor.x && bombs[i].y == cursor.y) {
          mvwprintw(game_win, cursor.y, cursor.x, "B");
          char gameOverMsg[] = "GAME OVER!!!";
          mvprintw(4, (COLS - strlen(gameOverMsg)) / 2, "%s", gameOverMsg);
          wrefresh(game_win);
          refresh();
          sleep(2);
          gameOver = 1;
        }
      }

      if (!gameOver) {
        removeBlocks(game_win, bombs, cursor, winarg.bomb_num, winarg.height,
                     winarg.width);
        wrefresh(game_win);
      }

      if (bombs_left == 0) {
        bool blocks_left = checkBlocksLeft(game_win, winarg);
        if (!blocks_left) {
          char gameWinMsg[] = "!!You Win!!";
          mvprintw(4, (COLS - strlen(gameWinMsg)) / 2, "%s", gameWinMsg);
          refresh();
          sleep(2);
          gameOver = 1;
        }
      }

      break;
    }

    wmove(game_win, cursor.y, cursor.x);
    wrefresh(game_win);
  }

  endwin();
  return 0;
}
