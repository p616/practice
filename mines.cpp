#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

// Prototypes
void Print_Grid (vector<vector<int> >&, vector<vector<string> >&, int, int);
void Play_Game(vector<vector<int> > &, vector<vector<string> > &, int, int);

int main()
{
    cout << "Enter # of rows and columns: ";
    int gridSize;
    cin >> gridSize;
    int ROWS = gridSize;
    int COLUMNS = gridSize;

    // int grid
    vector<vector<int> > grid(ROWS, vector<int>(COLUMNS, -2));

    // cover grid
    vector<vector<string> > cgrid(ROWS, vector<string>(COLUMNS, "*"));

    Print_Grid(grid, cgrid, ROWS, COLUMNS);
    cout << "\n";

    Play_Game(grid, cgrid, ROWS, COLUMNS);


    return 0;
}

void Print_Grid(vector<vector<int> > &grid, vector<vector<string> > &cgrid, int ROWS, int COLUMNS)
{
    srand(time(0));

    int mines;

    // Assigns -1's randomly to some squares
    for(int i=0; i<ROWS; i++)
    {
        for(int j=0; j<COLUMNS; j++)
        {
            int randN = (rand()%10)+1;
            if(randN==1 || randN==2 || randN==3)
            {
                grid[i][j] = -1; // mines are -1's
            }
        }
    }

    // this second nested loop checks surrounding squares for each elements.
    // However many mines are adjacent to the square,
    // that is the number assigned to that square.
    for(int i=0; i<ROWS; i++)
    {
        for(int j=0; j<COLUMNS; j++)
        {
            mines = 0;
            if(grid[i][j] != -2)
                continue;
            else if(grid[i][j] == -2)
            {
                // checks squares surrounding top left corner
                if(i == 0 && j == 0)
                {
                   if(grid[i][j+1] == -1)
                        mines++;
                   if(grid[i+1][j] == -1)
                        mines++;
                   if(grid[i+1][j+1] == -1)
                        mines++;
                }

                // top right corner
                if(i == 0 && j == COLUMNS-1)
                {
                    if(grid[i][j-1] == -1)
                        mines++;
                    if(grid[i+1][j] == -1)
                        mines++;
                    if(grid[i+1][j-1] == -1)
                        mines++;
                }

                // bottom left corner
                if(i == ROWS-1 && j == 0)
                {
                    if(grid[i][j+1] == -1)
                        mines++;
                    if(grid[i-1][j] == -1)
                        mines++;
                    if(grid[i-1][j+1] == -1)
                        mines++;
                }

                // bottom right corner
                if(i == ROWS-1 && j == COLUMNS-1)
                {
                    if(grid[i][j-1] == -1)
                        mines++;
                    if(grid[i-1][j-1] == -1)
                        mines++;
                    if(grid[i-1][j] == -1)
                        mines++;
                }

                // top edge
                if( i == 0 && j != 0 && j != COLUMNS-1)
                {
                    if(grid[i][j+1] == -1)
                        mines++;
                    if(grid[i][j-1] == -1)
                        mines++;
                    if(grid[i+1][j] == -1)
                        mines++;
                    if(grid[i+1][j+1] == -1)
                        mines++;
                    if(grid[i+1][j-1] == -1)
                        mines++;
                }

                // bottom edge
                if(i == ROWS-1 && j != 0 && j != COLUMNS-1)
                {
                    if(grid[i][j+1] == -1)
                        mines++;
                    if(grid[i][j-1] == -1)
                        mines++;
                    if(grid[i-1][j] == -1)
                        mines++;
                    if(grid[i-1][j+1] == -1)
                        mines++;
                    if(grid[i-1][j-1] == -1)
                        mines++;
                }

                // left edge
                if(j == 0 && i != 0 && i != ROWS-1)
                {
                    if(grid[i+1][j] == -1)
                        mines++;
                    if(grid[i-1][j] == -1)
                        mines++;
                    if(grid[i][j+1] == -1)
                        mines++;
                    if(grid[i+1][j+1] == -1)
                        mines++;
                    if(grid[i-1][j+1] == -1)
                        mines++;
                }

                // right edge
                if(j == COLUMNS-1 && i != 0 && i != ROWS-1)
                {
                    if(grid[i+1][j] == -1)
                        mines++;
                    if(grid[i-1][j] == -1)
                        mines++;
                    if(grid[i][j-1] == -1)
                        mines++;
                    if(grid[i+1][j-1] == -1)
                        mines++;
                    if(grid[i-1][j-1] == -1)
                        mines++;
                }


                // all other possible squares to be checked
                if(i !=0 && i!= ROWS-1 && j != 0 && j != COLUMNS-1)
                {
                    if(grid[i][j+1] == -1)
                        mines++;
                    if(grid[i][j-1] == -1)
                        mines++;
                    if(grid[i+1][j] == -1)
                        mines++;
                    if(grid[i-1][j] == -1)
                        mines++;
                    if(grid[i+1][j+1] == -1)
                        mines++;
                    if(grid[i+1][j-1] == -1)
                        mines++;
                    if(grid[i-1][j-1] == -1)
                        mines++;
                    if(grid[i-1][j+1] == -1)
                        mines++;
                }
                grid[i][j] = mines; // assign number of mines adjecent to that square
            }
        }

    }

    // print out int grid
    for(int i=0; i<ROWS; i++)
    {
        for(int j=0; j<COLUMNS; j++)
        {
            cout << setw(3) << grid[i][j] << setw(3);
        }

        cout << endl;
    }

    cout << "\n\nCOVER GRID\n\n";

    //Print cover grid
    for(int i=0; i<ROWS; i++)
    {
        for(int j=0; j<COLUMNS; j++)
        {
            cout << setw(3) << cgrid[i][j] << setw(3);
        }

        cout << endl;
    }

}

void Play_Game(vector<vector<int> > &grid, vector<vector<string> > &cgrid, int ROWS, int COLUMNS)
{
    // Bool variables used to break out of the nested loops
    bool flag1 = true;
    bool flag2 = true;
    bool flag3 = true;

    int row, column; // coordinares entered by user stored in these two variables

    while (flag1==true)
    {
        cout << "Enter row and column # (separated by space): ";
        cin >> row >> column;
        cout << "\n";

        for(int i=0; i<ROWS; i++)
        {
            for(int j=0; j<COLUMNS; j++)
            {
                if(grid[row][column] == grid[i][j] && grid[i][j])
                    cout << setw(3) << grid[i][j] << setw(3);
                else cout << setw(3) << cgrid[i][j] << setw(3);

                if(grid[row][column] == -1)
                    flag3 = false;

            }

            if(flag3 == false)
                flag2 = false;

            cout << endl;

        }

        if(flag2 == false)
            flag1 = false;

        cout << "\n";
    }

    cout << "\nYOU LOSE!" << endl;
}

