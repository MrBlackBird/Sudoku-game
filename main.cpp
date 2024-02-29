//sudoku game
#include <iostream>
#include <vector>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <chrono>

//board properties
struct Board{
    std::vector<std::vector<std::string>> brd;
    int sizeX = 9;
    int sizeY = 9;
};

//initiate board
Board create_board(){
    Board b;
    for(int i = 0; i < b.sizeY; i++){
        std::vector<std::string> row;
        for(int j = 0; j < b.sizeX; j++){
            row.emplace_back(" ");
        }
        b.brd.emplace_back(row);
    }
    return b;
}

//output bounds
//vertical
void print_vert_bound(int counter){
    if(counter % 3 == 0){
        std::cout<< " ][ ";
    }
    else
        std::cout<< " | ";
}

//horizontal
void print_hor_bound(int counter){
    if(counter % 3 == 0){
        for(int i = 0; i < 43; i++){
            std::cout<< "=";
        }
    }
    else{
        for(int i = 0; i < 43; i++){
            std::cout<< "-";
        }
    }
    std::cout<< "\n";
}

//output board
void print_board(Board &b){
    //counters for formatting
    int coll_counter = 0, row_counter = 0;

    for(auto &row : b.brd){

        //print horizontal bounds
        print_hor_bound(row_counter);

        for(auto &val : row){
            //print vertical bounds
            print_vert_bound(coll_counter);

            //print values
            std::cout<< val;
            coll_counter++;
        }
        std::cout<< " ][ ";         //added for good visual formatting
        std::cout<<"\n";

        row_counter++;

    }
    //bottom-most bound
    for(int i = 0; i < 43; i++){
        std::cout<< "=";
    }
    std::cout<< "\n";
}

//enable input
Board write(Board &b, int x, int y, std::string input){
    b.brd[x][y] = input;            //insert numbers
    return b;
}

//check for incorrect input values
bool input_checker(int x, int y, std::string move){
    bool xok = true, yok = true, mok = true;

    //checking x
    if(x < 9 && x >= 0){
        xok = true;
    }
    else
        xok = false;

    //chekcing y
    if(y < 9 && y >= 0){
        yok = true;
    }
    else yok = false;

    //checking input character
    if(move == "1" || move == "2" || move == "3" || move == "4" || move == "5" || move == "6" || move == "7" || move == "8" || move == "9" || move == "e"){
        mok = true;
    }
    else
        mok = false;

    //final overall check
    if(xok == true && yok == true && mok == true){
        return true;
    }
    else
        return false;
}

//check if a position is empty
bool pos_empty_checker(Board &b, int x, int y, std::string entry){
    if(b.brd[x][y] == " "){
        return true;
    }
    else{
        if(entry == "e"){           //added exeption for erasing numbers
            return true;
        }
        else
            return false;
    }
}

//erase an entry
Board erase_pos(Board &b, int x, int y){
    b.brd[x][y] = " ";              //erase numbers
    return b;
}

//check for correctly filled cells
//check if a section (row, column, or square) is valid
bool check_section(const std::vector<std::string>& section){
    std::unordered_set<std::string> seen;
    for(const auto& cell : section){
        if(cell != "" && !seen.insert(cell).second){
            return false;
        }
    }
    return true;
}

//check if the Sudoku board is solved
bool endgame_checker(Board &b){
    // Check rows
    for(const auto& row : b.brd){
        if(!check_section(row)){
            return false;
        }
    }

    // Check columns
    for(int col = 0; col < 9; ++col){
        std::vector<std::string> column;
        for(int row = 0; row < 9; ++row){
            column.push_back(b.brd[row][col]);
        }
        if(!check_section(column)){
            return false;
        }
    }

    // Check squares
    for(int row = 0; row < 9; row += 3){
        for(int col = 0; col < 9; col += 3){
            std::vector<std::string> square;
            for(int i = row; i < row + 3; ++i){
                for(int j = col; j < col + 3; ++j){
                    square.push_back(b.brd[i][j]);
                }
            }
            if(!check_section(square)){
                return false;
            }
        }
    }

    return true; // Sudoku is solved
}

//checks if board is full
bool board_full(Board &b){
    bool full = true;
    for(auto &row : b.brd){
        bool full_row = true;
        for(auto &val : row){
            if(val == " "){
                full_row = false;
                break;
            }
        }
        if(full_row == false){
            full = false;
            break;
        }
    }
    return full;
}

//checks the outcome of a game
bool check_win(bool correct_ent, bool full_board){
    if(full_board == true && correct_ent == true){
        return true;
    }
    else
        return false;
}

//game over message
void game_over_text(int turns, float t, bool win){
    if(win == true){
        std::cout<< "Game over - you win!\n" << "Time: " << t << "\nUsed turnes: " << turns;
    }
    else
        std::cout<< "Game over - you loose!\n" << "Time: " << t << "\nUsed turnes: " << turns;
}

//pick level
std::string pick_level(){
    std::string level;
    std::cout<< "Pick level [1-3]: ";
    std::cin>> level;
    return level;
}

//add conversion for coordinates
int convert_str_to_int(char ch){
    int conv_int = ch - '0';
    return conv_int;
}

void append_int(std::vector<int> &target_vec, std::string cords){
    for(char ch : cords){
        target_vec.emplace_back(convert_str_to_int(ch));
    }
}

void append_str(std::vector<std::string> &target_vec, std::string nums){
    for(int i = 0; i < nums.size(); i++){
        target_vec.emplace_back(nums[i]);
    }
}

void load_level(Board &b, std::vector<std::string> num, std::vector<int> cor, int row){
    // Iterate over the numbers and their corresponding positions
    for (int i = 0; i < num.size(); ++i) {
        // Calculate the column from the 1D coordinate
        int col = cor[i] % 9;

        // Place the number on the board at the specified row and column
        b.brd[row][col] = num[i];
    }
}

void read_level(Board &b, std::string level){
    //open file
    std::ifstream file("levels.csv");
    std::string line;

    //for coordinates
    std::vector<int> cords;
    std::vector<std::string> nums;

    //searched lvl -> level
    std::string r;
    while(std::getline(file, line)){
        std::istringstream iss(line);
        std::string lev, row, num, pos;

        //read the entries
        if(std::getline(iss, lev, ',') && std::getline(iss, row, ',') && std::getline(iss, num, ',') && std::getline(iss, pos, ',')){
            if(lev == level){
                //fill cords and nums
                append_str(nums,num);
                append_int(cords,pos);
                r = row;
            }
        }
    }

    file.close();

    //load board
    load_level(b, nums, cords, convert_str_to_int(r[0]));

}

//main body
int main()
{
    //create board
    Board board;
    board = create_board();

    //variables
    int turn = 0;
    int xcord = 0, ycord = 0;
    float time = 0;
    std::string num;
    bool game_over = false;
    std::string lvl = pick_level();
    read_level(board, lvl);
    
    //start timer
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    
    while(game_over == false){

        //get input
        while(true){
            std::cout<< "Number: ";
            std::cin>> num;
            std::cout<< "Coordinates: ";
            std::cin>> xcord >> ycord;

            //cheking input
            if(input_checker(xcord, ycord, num) == true && pos_empty_checker(board, xcord, ycord, num) == true){           //correct values? position not empty?
                break;
            }
            else
                std::cout<< "Invalid entries or position not empty, try again\n";
        }

        //update board
        if(num == "e"){
            board = erase_pos(board, xcord, ycord);
        }
        else
            board = write(board, xcord, ycord, num);

        //print board
        print_board(board);

        turn++;
        
        //endgame
        if(check_win(endgame_checker(board), board_full(board)) == true){
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
            game_over_text(turn, time, check_win(endgame_checker(board), board_full(board)));
        }
    }

    return 0;
}
