// ConsoleApplication2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>


// THIS PASSES THE TEST CASES, BUT NOT THE TIME LIMIT


using namespace std;

struct rect
{
    // points on "A" diagonal
    pair<int, int> d1;
    pair<int, int> d2;

    rect(pair<int, int> p1, pair<int, int> p2) : d1(p1), d2(p2) {}
};

/*
void printgrid(vector<vector<int>>& grid)
{
    for (int i = 0; i < grid.size(); ++i)
    {
        for (int j = 0; j < grid[i].size(); ++j)
        {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }
}
*/

// marks available requested space as occupied
// request rect diagonals are grid points
// booking x,y range should be inclusive of largst point
// returns number of spaces booked
int BookRect(vector<vector<int>>& grid, rect r)
{
    int booked = 0;
    for (int x = min(r.d1.first, r.d2.first);
        x <= max(r.d1.first, r.d2.first);
        ++x)
    {
        for (int y = min(r.d1.second, r.d2.second);
            y <= max(r.d1.second, r.d2.second);
            ++y)
        {
            if (grid[x][y] == 0)
            {
                grid[x][y] = 1;
                booked++;
            }
        }
    }
    return booked;
}

int main()
{
    int N;
    cin >> N;

    int M;
    cin >> M;

    int P;
    cin >> P;

    vector<rect> requests;
    for (int i = 0; i < P; ++i)
    {
        pair<int, int> p1;
        pair<int, int> p2;

        // rectangle diagonal points are input 1-based

        int n;
        cin >> n;
        p1.first = n-1;

        cin >> n;
        p1.second = n-1;

        cin >> n;
        p2.first = n-1;

        cin >> n;
        p2.second = n-1;

        rect r(p1, p2);
        requests.push_back(r);
    }

    // create grid representing total space
    // for each request, mark grid positions as occupied
    // don't care who gets what, so just count up unoccupied space at second

    // init grid to all to zeroes
    vector<vector<int>> grid;
    for (int i = 0; i < N; ++i)
    {
        vector<int> v(M, 0);
        grid.push_back(v);
    }

    // first request gets whole rect area
    // remaining requests get all, portion, or none of rect area
    int totalSpace = (N * M);
    for (int i = 0; i < requests.size(); ++i)
    {
        totalSpace -= BookRect(grid, requests[i]);
    }

    cout << totalSpace;

    return 0;
}
