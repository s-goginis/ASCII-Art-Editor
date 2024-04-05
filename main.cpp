// Shreya Gogini
// p2.cpp
// Generates a board that can be resized, painted,
// stored, loaded, painted, swapped based on user inputs

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

// returns resized function that overrides original grid based on user input
char **resize(char **, int, int, char, int *, int *);
// deallocates the array to free memory
void delete_array(char **, int, int);
// swaps all of one type of character for another based on user input
void swap(char, char, char **, int *, int *);
// prints the grid
void print_2d(char **, int *, int *);
// makes a 2d dynamic array with the designated dimensions and filler
char **make_grid(int, int, char);
// overrides the current grid with the one saved in a file
char **load(string, char **, int *, int *);
// stores the current grid into a file with the dimensions saved
void store(string, char **, int *, int *);
// Paints a certain element of the grid a certain character
void paint_2d(char **, int, int, char);

int  main() {
    // dimensions for the grid
    // pointers to update the dimensions when changed
    int  w      = 16;
    int *width  = &w;
    int  h      = 8;
    int *height = &h;
    // temporary fill character for the start of the grid
    char fill = '`';

    // creating the first grid
    char **grid = make_grid(*width, *height, fill);

    // print out the first grid for the user to start inputs
    print_2d(grid, width, height);

    string line;
    bool   done      = false;
    bool   bad_input = false;
    while (!done && getline(cin, line)) {
        stringstream stream(line);
        string       word;
        // getting the first word of the input to determine actions
        stream >> word;
        // keeping watch for bad input to re-prompt user
        bad_input = false;

        // Actions based on user input
        if (word == "quit") {
            done = true; // ends the loop
        } else if (word == "resize") {
            char new_fill;
            int  x = 0;
            int  y = 0;
            // gets input for new dimensions and extra fill
            if (stream >> x >> y >> new_fill) {
                // calls function with the data
                grid = resize(grid, x, y, new_fill, width, height);
                // updates the dimensions
                *width  = x;
                *height = y;
            } else {
                bad_input = true;
            }
        } else if (word == "paint") {
            int  x = 0;
            int  y = 0;
            char fill;
            // gets input for point on the grid and what to fill it with
            if (stream >> x >> y >> fill) {
                // makes sure the point is within bounds
                if (x >= 0 && x <= *width - 1 && y >= 0 && y <= *height - 1) {
                    paint_2d(grid, x, y, fill);
                    // grid[y][x] = fill;
                }
                // grid = paint(x, y, fill, grid, width, height);
            } else {
                bad_input = true;
            }
        } else if (word == "swap") {
            char target, repl;
            // gets input for target character and what to replace it with
            if (stream >> target >> repl) {
                swap(target, repl, grid, width, height);
            } else {
                bad_input = true;
            }
        } else if (word == "load") {
            string filepath;
            // gets input for the name of the file to open
            if (stream >> filepath) {
                // deallocates the current grid to override with new grid
                delete_array(grid, *width, *height);
                grid = load(filepath, grid, width, height);
            } else {
                bad_input = true;
            }
        } else if (word == "store") {
            string filepath;
            // gets input for the file to store the grid in
            if (stream >> filepath) {
                store(filepath, grid, width, height);
            } else {
                bad_input = true;
            }
        }

        if (bad_input == true) {
            // re-prompts user for new input
            stringstream stream(line);
            string       word;
            stream >> word;
        }
        if (word != "quit") {
            // prints the output of the input
            print_2d(grid, width, height);
        }
    }
    // deallocates the grid
    delete_array(grid, *width, *height);
    return 0;
}

// Makes the board based on function call
char **make_grid(int width, int height, char fill) {
    // dynamic allocation of a 2d array
    char **grid = new char *[height];
    for (int i = 0; i < height; i++) {
        grid[i] = new char[width];
    }
    // filling with a designated character
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            grid[i][j] = fill;
        }
    }
    // returns the new board at the end of the function
    return grid;
}

// Prints the board as it is with all the characters
void print_2d(char **grid, int *width, int *height) {
    for (int i = 0; i < *height; i++) {
        for (int j = 0; j < *width; j++) {
            cout << grid[i][j];
        }
        cout << endl;
    }
}

// Deallocates the memory stored by the dynamic 2d array
void delete_array(char **grid, int width, int height) {
    for (int i = 0; i < height; i++) {
        delete[] grid[i];
    }
    delete[] grid;
}

// Paints one element of user choice a certain fill
void paint_2d(char **grid, int x, int y, char fill) {
    grid[y][x] = fill;
}

// Swaps all of a target character with the new character
void swap(char target, char repl, char **grid, int *width, int *height) {
    for (int i = 0; i < *height; i++) {
        for (int j = 0; j < *width; j++) {
            // Finding all instances of the target character
            if (grid[i][j] == target) {
                // swapping the character
                grid[i][j] = repl;
            }
        }
    }
}

// Resizes the grid based on user input
char **resize(char **grid, int new_width, int new_height, char fill, int *width, int *height) {
    // Makes a new grid filled with the character user provides
    char **resized = make_grid(new_width, new_height, fill);
    // Find the minimum width and height to prevent out of bounds
    int min_height = (*height > new_height) ? new_height : *height;
    int min_width  = (*width > new_width) ? new_width : *width;

    // Overlaps the old grid with the new one
    for (int i = 0; i < min_height; i++) {
        for (int j = 0; j < min_width; j++) {
            resized[i][j] = grid[i][j];
        }
    }

    // Deletes the old grid since it is not needed anymore
    delete_array(grid, *width, *height);
    // Updates the values for the width and height with the new dimensions
    *height = new_height;
    *width  = new_width;

    // returns the new grid
    return resized;
}

// Finds the file in the user's directory and overrides the current grid
char **load(string filepath, char **grid, int *width, int *height) {
    ifstream file(filepath);
    // validates the filepath
    if (!file) {
        cout << "File error" << endl;
    }

    // Gets the width and height of the stored grid
    file >> *width >> *height;
    // makes a new grid using the dimensions and an empty fill character
    char **new_arr = make_grid(*width, *height, ' ');

    // Fills the new grid based on the stored grid
    for (int x = 0; x < *height; x++) {
        for (int y = 0; y < *width; y++) {
            file >> new_arr[x][y];
        }
    }
    file.close();
    // returns the new grid
    return new_arr;
}

// Stores the current grid to the designated file name
void store(string filepath, char **grid, int *width, int *height) {
    ofstream file(filepath);
    // input validation
    if (!file) {
        cout << "File error" << endl;
    }
    // Sends the width and height of the current grid to the file
    file << *width << " " << *height << endl;

    for (int x = 0; x < *height; x++) {
        for (int y = 0; y < *width; y++) {
            file << grid[x][y];
        }
        file << endl;
    }
    file.close();
}
