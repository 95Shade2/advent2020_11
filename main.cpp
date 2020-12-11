#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

vector<string> File_To_Str_Vec(string filename) {
    vector<string> lines_int;
    string line;
    ifstream file;

    file.open(filename.c_str());

    getline(file, line);
    while (file) {
        lines_int.push_back(line);
        getline(file, line);
    }

    file.close();

    return lines_int;
}

void Print(vector<string> ary, string col_sep, string row_sep) {
    for (int r = 0; r < ary.size(); r++) {
        for (int c = 0; c < ary[r].size(); c++) {
            cout << ary[r][c] << col_sep;
        }
        cout << row_sep;
    }
}

int Get_Visual(vector<string> seats, int row, int col) {
    int visual_seats = 0;
    int r;
    int c;
    bool hit;

    //starting from straight down and going clockwise
    vector<int> row_add = {1, 1, 0, -1, -1, -1, 0, 1};
    vector<int> col_add = {0, -1, -1, -1, 0, 1, 1, 1};

    for (int dir = 0; dir < row_add.size(); dir++) {
        r = row + row_add[dir];
        c = col + col_add[dir];
        hit = false;

        //keep going in the current direction until it hits a seat or reaches the end of the line
        while (!hit && r >= 0 && r < seats.size() && c >= 0 && c < seats[0].size()) {
            if (seats[r][c] == '#') {
                visual_seats++;
                hit = true;
            }
            else if (seats[r][c] == 'L') {
                hit = true;
            }

            r += row_add[dir];
            c += col_add[dir];
        }
    }

    return visual_seats;
}

int Get_Adjacent(vector<string> seats, int row, int col) {
    int adjacent_seats = 0;

    for (int r = row - 1; r <= row + 1; r++) {
        for (int c = col - 1; c <= col + 1; c++) {
            //don't include the center seat
            if (!(r == row && c == col)) {
                if (r >= 0 && r < seats.size() && c >= 0 && c < seats[0].size()) {
                    if (seats[r][c] == '#') {
                        adjacent_seats++;
                    }
                }
            }
        }
    }

    return adjacent_seats;
}

int State(vector<string> seats, int r, int c, int part = 1) {
    char seat = seats[r][c];
    int adjacent_seats = 0;
    int visual_seats = 0;

    if (part == 1) {
        if (seat == 'L') {
            adjacent_seats = Get_Adjacent(seats, r, c);

            //no seats adjacent are occupied, so fill it
            if (adjacent_seats == 0) {
                return 0;
            }
        }
        else if (seat == '#') {
            adjacent_seats = Get_Adjacent(seats, r, c);

            //4 or more people adjacent is too much, so make seat empty
            if (adjacent_seats >= 4) {
                return 1;
            }
        }
    }
    else {
        if (seat == 'L') {
            visual_seats = Get_Visual(seats, r, c);

            if (visual_seats == 0) {
                return 0;
            }
        }
        else if (seat == '#') {
            visual_seats = Get_Visual(seats, r, c);

            if (visual_seats >= 5) {
                return 1;
            }
        }
    }
    return -1;
}

int Update_Seats(vector<string> *seats, int part = 1) {
    string row;
    vector<string> new_seats = *seats;  //dont wanna change the seats while calculating people moving
    int state;
    int total_seats = 0;

    for (int r = 0; r < (*seats).size(); r++) {
        row = (*seats)[r];
        for (int c = 0; c < row.size(); c++) {
                state = State(*seats, r, c, part);

                //empty seat becomes filled
                if (state == 0) {
                    new_seats[r][c] = '#';
                    total_seats++;
                }
                //too many people sitting nearby, leave seat
                else if (state == 1) {
                    new_seats[r][c] = 'L';
                }
                //person stayed in seat
                else if (row[c] == '#') {
                    total_seats++;
                }
        }
    }

    //update the seats
    *seats = new_seats;

    return total_seats;
}

int Stabilize_Seats(vector<string> seats, int part = 1) {
    int seats_filled = 0;
    int prev_filled = -1;

    while (prev_filled != seats_filled) {
        prev_filled = seats_filled;

        seats_filled = Update_Seats(&seats, part);
    }

    return seats_filled;
}

int main()
{
    vector<string> seats;
    int seats_filled;

    seats = File_To_Str_Vec("seats.txt");

    seats_filled = Stabilize_Seats(seats);

    cout << seats_filled << endl;

    cout << "\n---Part 2---\n\n";

    //part 2
    seats_filled = Stabilize_Seats(seats, 2);

    cout << seats_filled << endl;

    return 0;
}
