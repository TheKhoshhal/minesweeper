

// Position bomb_loc_check(int j, int height, int width, Position bombs[],
//                         Position temp_pos) {
//   if (bombs[j].x == temp_pos.x && bombs[j].y == temp_pos.y) {
//     temp_pos = createBomb(height, width);
//     temp_pos = bomb_loc_check(j, height, width, bombs, temp_pos);
//   }
//   return temp_pos;
// }


// void placeBombs(int bomb_count, Position bombs[], int height, int width) {
//    Position temp_pos;
//    for (int i = 0; i < bomb_num; ++i) {
//      if (i > 0) {
//        for (int j = 0; j <= i; ++j) {
//          temp_pos = createBomb(height, width);
//          bombs[i] = bomb_loc_check(j, height, width, bombs, temp_pos);
//        }
//      } else {
//        bombs[i] = createBomb(height, width);
//      }
//    }
// }

/* This is how to get a char from a specific point in a specific window */
// int x = mvwinch(game_win, 1, 1);
// mvprintw(1, 0, "%c", x);
