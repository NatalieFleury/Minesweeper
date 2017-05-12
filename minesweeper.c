
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


//////////////////////////////////////
const char UNREVEALED = ' ' ;
const char REVEALED[9] = "012345678";
const char FLAG = 'F';
const char MINE = '*';
//////////////////////////////////////


bool flag(struct ms_board *b, int x, int y) {
  assert(b);
  assert(1 <= x && x <= b->width);
  assert(1 <= y && y <= b->height);
  if (b->board[(y-1) * b->width + x - 1] > 47 && b->board[(y-1) * b->width + x - 1] < 57) {
    return false;
  }
  else if (b->board[(y-1) * b->width + x - 1] == FLAG) {
    b->board[(y-1) * b->width + x - 1] = UNREVEALED;
  }
  else {
    b->board[(y-1) * b->width + x - 1] = FLAG;
  }  
  return true;
}


static bool in_mines(struct ms_board *b, int x, int y) {
  for(int i = 0; i < b->num_mines; ++i) {
    if (b->mines[i].x == x && b->mines[i].y == y) {
      return true;
    }
  }
  return false;
}


bool reveal(struct ms_board *b, int x, int y) {
  assert(b);
  assert(1 <= x && x <= b->width);
  assert(1 <= y && y <= b->height);
  if (b->board[(y-1) * b->width + x - 1] == FLAG || b->board[(y-1) * b->width + x - 1] == MINE || (b->board[(y-1) * b->width + x - 1] > 47 && b->board[(y-1) * b->width + x - 1] < 57)) {
    return false;
  }
  else if (in_mines(b, x, y)) {
    return false;
  }
  struct posn my_arr[8] = {{x-1, y-1}, {x, y-1}, {x+1, y-1}, {x-1, y}, {x+1, y}, {x-1, y+1}, {x, y+1}, {x+1, y+1}};
  int num_of_mines = 0;
  for(int i = 0; i < 8; ++i) {
    if (in_mines(b, my_arr[i].x, my_arr[i].y)) {
      ++num_of_mines;
    }
  }
  b->board[(y-1) * b->width + x - 1] = REVEALED[num_of_mines];
  if (num_of_mines == 0) {
    for(int i = 0; i < 8; ++i) {
      reveal(b, my_arr[i].x, my_arr[i].y);
    }
  }
  return true;
}


bool game_won(const struct ms_board *b) {
  assert(b);
  int blank_count = 0;
  for(int i = 0; i < b->width * b->height; ++i) {
    if (b->board[i] == MINE) {
      return false;
    }
    else if (b->board[i] == UNREVEALED || b->board[i] == FLAG) {
      ++blank_count;
    }
  }
  if (blank_count == b->num_mines) {
    return true;
  }
  return false;
}


bool game_lost(const struct ms_board *b) {
  assert(b);
  for(int i = 0; i < b->width * b->height; ++i) {
    if (b->board[i] == MINE) {
      return true;
    }
  }
  return false;
}



// a very simple assertion-based minesweeper test client


static bool char_array_equal(const char a[], const char b[], int len) {
  for (int i = 0; i < len; ++i) {
    if (a[i] != b[i]) return false;
  }
  return true;
}

int main(void) {
  struct posn mines[] = {{1,1}};
  char board[]  = "         ";     
  char eboard[] = " 1  F    ";
  struct ms_board b = {3, 3, board, 1, mines};
  assert(!game_won(&b));
  assert(!game_lost(&b));
  flag(&b, 2, 2);
  reveal(&b, 2, 1);
  assert(char_array_equal(board, eboard, 9));
}

