#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;

const int N = 9;

bool isValid(int grid[N][N], int row, int col, int num) {
    //rows and columns
    for (int x = 0; x < N; x++) {
        if (grid[row][x] == num || grid[x][col] == num)
            return false;
    }

    //3x3 subgrid
    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grid[i + startRow][j + startCol] == num)
                return false;

    return true;
}

bool solveSudoku(int grid[N][N]) {
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            if (grid[row][col] == 0) {
                for (int num = 1; num <= 9; num++) {
                    if (isValid(grid, row, col, num)) {
                        grid[row][col] = num;
                        if (solveSudoku(grid))
                            return true;
                        grid[row][col] = 0; //backtrack
                    }
                }
                return false;
            }
        }
    }
    return true;
}

void printGrid(int initgrid[N][N], int grid[N][N]) {
    for (int row = 0; row < N; row++) {
        if (row % 3 == 0) {
            cout << "-------------------------" << endl;
        }
        for (int col = 0; col < N; col++) {
            if(col % 3 == 0 ) {
                cout << "| ";
            }
            if(initgrid[row][col] == 0) {
                cout <<"\033[31m" << grid[row][col] << "\033[0m"<< " ";
            } 
            else {
                cout<< grid[row][col] << " ";
            }
        }
        cout << "| ";
        cout << endl;
    }
    cout << "-------------------------" << endl;
}

bool loadFromFile(const string& filename, int grid[N][N]) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Cannot open file " << filename << endl;
        return false;
    }

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            file >> grid[i][j];

    file.close();
    return true;
}

int main() {
    int initgrid[N][N];

    if (!loadFromFile("sudoku.txt", initgrid)) {
        return 1; // Exit if file loading fails
    }
    int grid [N][N];

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grid[i][j] = initgrid[i][j];
        }
    }
    auto start_time = std::chrono::high_resolution_clock::now();
    
    cout << "Solved Sudoku:" << endl;
    if (solveSudoku(grid)) {
        printGrid(initgrid,grid);
        
    }
    
    else {
        cout << "No solution exists" << endl;
    }
        
    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;
    cout << "Time taken: " 
         << std::chrono::duration_cast<std::chrono::microseconds>(time).count() 
         << " microseconds" << endl;

    return 0;
}