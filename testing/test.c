#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define HUGE
#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof((arr)[0]))

int k = 0;

typedef struct {
  int x;
  int y;
} Position;

Position bombs[20] = {};

void calculateArgs(int argc, char *argv[]) {

  printf("%d\n", argc);
  if (argc > 1) {
    if (!strcmp(argv[1], "small")) {
      printf("you chose small");
    } else if (!strcmp(argv[1], "medium")) {
      printf("you chose medium");
    }
  }
}

// Position bomb_loc_check(int height, int width, Position bomb,
//                         Position pos) {
//   if (bomb.x == pos.x && bomb.y == pos.y) {
//     pos = createBomb(height, width);
//     pos = bomb_loc_check(height, width, bomb, pos);
//   }
//   return pos;
// }

// void createBombs(int bomb_num, Position bombs_arr[], int height, int width) {
//   Position temp_pos;
//   for (int i = 0; i < bomb_num; ++i) {
//     if (i > 0) {
//       for (int j = 0; j < i; ++j) {
//         temp_pos = createBomb(height, width);
//         bombs_arr[i] = bomb_loc_check(height, width, bombs_arr[j], temp_pos);
//       }
//     } else {
//       bombs_arr[i] = createBomb(height, width);
//     }
//   }
// }

bool position_exists(int count, Position *bombs, Position pos) {
  for (int i = 0; i < count; i++) {
    if (bombs[i].x == pos.x && bombs[i].y == pos.y) {
      return true;
    }
  }
  return false;
}

void place_bombs(Position *bombs, int bomb_count, int width, int height) {
  srand(time(NULL));

  for (int i = 0; i < bomb_count; ) {
    Position pos;
    // pos = createBomb(height, width);
    pos.x = (rand() % height) + 1;
    pos.y = (rand() % width) + 1;

    if (!position_exists(i, bombs, pos)) {
      bombs[i] = pos;
      ++i;
    }
  }
  
}


//TODO: complete function
// int check_cells(int bomb_num, Position *bombs, Position cursor) {
//   int neighbor_bombs = 0;
//   for (int i = 0; i < bomb_num; ++i) {
//     if (bombs[i].x >= cursor.x - 1 && bombs[i].x <= cursor.x + 1 &&
//         bombs[i].y >= cursor.y - 1 && bombs[i].y <= cursor.y + 1) {
//       ++neighbor_bombs;
//     }
//   }

//   return neighbor_bombs;
// }

int countAdjacentBombs(Position cursor, Position *bombs, int bomb_count) {
  int count = 0;

  for (int dx = -1; dx <= 1; dx++) {
    for (int dy = -1; dy <= 1; dy++) {
      if (dx == 0 && dy == 0) {
        continue;
      }

      Position adjacent = {cursor.x + dx, cursor.y + dy};

      for (int i = 0; i < bomb_count; i++) {
        if (bombs[i].x == adjacent.x && bombs[i].y == adjacent.y) {
          count++;
          break;
        }
      }
    }
  }

  return count;
}

int main(int argc, char *argv[]) {
  // createBombs(40, bombs, 18, 10);

  // for (int i = 0; i < 40; ++i) {
  //   printf("%d: x = %d, y = %d\n", i + 1, bombs[i].x, bombs[i].y);
  // }

  // calculateArgs(argc, argv);

  // createBombs(15, bombs, 5, 5);
  // place_bombs(bombs, 15, 5, 5);

  // for (int i = 0; i < 20; ++i) {
  //   printf("%d, %d\n", bombs[i].x, bombs[i].y);
  // }

  // for (int i = 50; i <= 100; ++i) {
  //   printf("%02d \n", i);
  // }

  // printf("%lu\n", ARRAY_LENGTH(bombs));

  // Position cursor;
  // cursor.x = 3;
  // cursor.y = 3;

  // int x = check_cells(15, bombs, cursor);
  // int x = count_adjacent_bombs(cursor, bombs, 15);
  // printf("%d", x);

  char x[] = "hello";
  printf("%lu", sizeof(x));

  return 0;
}
