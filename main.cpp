#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <vector>

using namespace std;

// board properties
class Board {
private:
  vector<vector<string>> brd;
  vector<vector<bool>> mod;
  int sizeX = 9;
  int sizeY = 9;

public:
  // initiate board
  void create_board() {
    for (int i = 0; i < sizeY; i++) {
      vector<string> row;
      for (int j = 0; j < sizeX; j++) {
        row.emplace_back(" ");
      }
      brd.emplace_back(row);
    }
  }

  // output bounds
  // vertical
  void print_vert_bound(int counter) {
    if (counter % 3 == 0) {
      cout << " ][ ";
    } else
      cout << " | ";
  }

  // horizontal
  void print_hor_bound(int counter) {
    if (counter % 3 == 0) {
      for (int i = 0; i < 43; i++) {
        cout << "=";
      }
    } else {
      for (int i = 0; i < 43; i++) {
        cout << "-";
      }
    }
    cout << "\n";
  }

  // populate mod vector
  void create_mod() {
    for (int i = 0; i < sizeY; i++) {
      vector<bool> row;
      for (int j = 0; j < sizeX; j++) {
        row.emplace_back(true);
      }
      mod.emplace_back(row);
    }
  }

  // output board
  void print_board() {
    // counters for formatting
    int coll_counter = 0, row_counter = 0;

    for (auto &row : brd) {

      // print horizontal bounds
      print_hor_bound(row_counter);

      for (auto &val : row) {
        // print vertical bounds
        print_vert_bound(coll_counter);

        // print values
        cout << val;
        coll_counter++;
      }
      cout << " ][ "; // added for good visual formatting
      cout << "\n";

      row_counter++;
    }
    // bottom-most bound
    for (int i = 0; i < 43; i++) {
      cout << "=";
    }
    cout << "\n";
  }

  // enable input
  void write(int x, int y, string input) {
    brd[x][y] = input; // insert numbers
  }

  // check for incorrect input values
  bool input_checker(int x, int y, string move) {
    bool xok = true, yok = true, mok = true;

    // checking x
    if (x < 9 && x >= 0) {
      xok = true;
    } else
      xok = false;

    // chekcing y
    if (y < 9 && y >= 0) {
      yok = true;
    } else
      yok = false;

    // checking input character
    if (move == "1" || move == "2" || move == "3" || move == "4" ||
        move == "5" || move == "6" || move == "7" || move == "8" ||
        move == "9" || move == "e") {
      mok = true;
    } else
      mok = false;

    // final overall check
    if (xok == true && yok == true && mok == true) {
      return true;
    } else
      return false;
  }

  // check if a position is empty
  bool pos_empty_checker(int x, int y, string entry) {
    if (brd[x][y] == " ") {
      return true;
    } else {
      if (entry == "e") { // added exception for erasing numbers
        return true;
      } else
        return false;
    }
  }

  // FIX: prevent erasing level-loaded numbers by player

  // erase an entry
  void erase_pos(int x, int y) {
    if (mod[x][y] == true) {
      brd[x][y] = " "; // erase numbers
    } else {
      cout << "Level-loaded entry, cannot be modifed\n";
    }
  }

  // check for correctly filled cells
  // check if a section (row, column, or square) is valid
  bool check_section(const vector<string> &section) {
    unordered_set<string> seen;
    for (const auto &cell : section) {
      if (cell != "" && !seen.insert(cell).second) {
        return false;
      }
    }
    return true;
  }

  // check if the Sudoku board is solved
  bool endgame_checker() {
    // Check rows
    for (const auto &row : brd) {
      if (!check_section(row)) {
        return false;
      }
    }

    // Check columns
    for (int col = 0; col < 9; col++) {
      vector<string> column;
      for (int row = 0; row < 9; row++) {
        column.push_back(brd[row][col]);
      }
      if (!check_section(column)) {
        return false;
      }
    }

    // Check squares
    for (int row = 0; row < 9; row += 3) {
      for (int col = 0; col < 9; col += 3) {
        vector<string> square;
        for (int i = row; i < row + 3; ++i) {
          for (int j = col; j < col + 3; ++j) {
            square.push_back(brd[i][j]);
          }
        }
        if (!check_section(square)) {
          return false;
        }
      }
    }

    return true; // Sudoku is solved
  }

  // checks if board is full
  bool board_full() {
    bool full = true;
    for (auto &row : brd) {
      bool full_row = true;
      for (auto &val : row) {
        if (val == " ") {
          full_row = false;
          break;
        }
      }
      if (full_row == false) {
        full = false;
        break;
      }
    }
    return full;
  }

  // checks the outcome of a game
  bool check_win(bool correct_ent, bool full_board) {
    if (full_board == true && correct_ent == true) {
      return true;
    } else
      return false;
  }

  // game over message
  void game_over_text(int turns, float t, bool win) {
    if (win == true) {
      cout << "Game over - you win!\n"
           << "Time: " << t << "\nUsed turnes: " << turns << "\n";
    } else {
      cout << "Game over - you lose!\n"
           << "Time: " << t << "\nUsed turns: " << turns << "\n";
    }
  }

  // pick level
  string pick_level() {
    string level;
    cout << "Pick level [1-3]: ";
    cin >> level;
    return level;
  }

  // filling cord vector
  void append_int(vector<int> &target_vec, string cords) {
    for (char ch : cords) {
      target_vec.emplace_back(ch - '0');
    }
  }

  // filling nums vector
  void append_str(vector<string> &target_vec, string nums) {
    for (char ch : nums) {
      target_vec.emplace_back(string(1, ch));
    }
  }

  // loading chosen level
  void load_level(vector<string> num, vector<int> cor, int row) {
    // go over the numbers and their corresponding positions
    for (size_t i = 0; i < num.size(); i++) {
      // column from the 1D coordinate
      int col = cor[i] % 9;

      // place the number on the board
      brd[row][col] = num[i];
      mod[row][col] = false;
    }
  }

  // reading level from .csv file
  void read_level(string level) {
    // open file
    ifstream file(
        "/home/raven/Documents/GitHub/Sudoku-game/Sudoku-game/levels.csv");
    string line;

    // read lines
    while (getline(file, line)) {
      istringstream iss(line);
      string lev, row_str, nums_str, pos_str;

      // read level, row, numbers, and positions
      if (getline(iss, lev, ',') && getline(iss, row_str, ',') &&
          getline(iss, nums_str, ',') && getline(iss, pos_str, ',')) {
        // check if the level matches
        if (lev == level) {
          // convert row number from str to int
          int row = stoi(row_str);

          // parse numbers and positions
          for (size_t i = 0; i < nums_str.size(); i++) {
            string num = string(1, nums_str[i]);
            int pos = pos_str[i] - '0';
            int col = pos % 9;
            brd[row][col] = num;
            mod[row][col] = false;
          }
        }
      }
    }

    file.close();
  }
};
// main body
int main() {
  // create board
  Board board;
  board.create_board();

  // variables
  int turn = 0;
  int xcord = 0, ycord = 0;
  float time = 0;
  string num;
  bool game_over = false;
  board.create_mod();
  string lvl = board.pick_level();
  board.read_level(lvl);
  board.print_board();

  // start timer
  chrono::steady_clock::time_point begin = chrono::steady_clock::now();

  while (game_over == false) {

    // get input
    while (true) {
      cout << "Number: ";
      cin >> num;
      cout << "Coordinates: ";
      cin >> xcord >> ycord;

      // checking input
      if (board.input_checker(xcord, ycord, num) == true &&
          board.pos_empty_checker(xcord, ycord, num) ==
              true) { // correct values? position not empty?
        break;
      } else
        cout << "Invalid entries or position not empty, try again\n";
    }

    // update board
    if (num == "e") {
      board.erase_pos(xcord, ycord);
    } else
      board.write(xcord, ycord, num);

    // print board
    board.print_board();

    turn++;

    // endgame
    if (board.check_win(board.endgame_checker(), board.board_full()) == true) {
      chrono::steady_clock::time_point end = chrono::steady_clock::now();
      time = chrono::duration_cast<chrono::microseconds>(end - begin).count();
      board.game_over_text(
          turn, time,
          board.check_win(board.endgame_checker(), board.board_full()));
      game_over = true; // Exit the game loop
    } else if (board.board_full() == true &&
               board.check_win(board.endgame_checker(), board.board_full()) ==
                   false) {
      chrono::steady_clock::time_point end = chrono::steady_clock::now();
      time = chrono::duration_cast<chrono::microseconds>(end - begin).count();
      board.game_over_text(
          turn, time,
          board.check_win(board.endgame_checker(), board.board_full()));
      game_over = true; // Exit the game loop
    }
  }

  return 0;
}
