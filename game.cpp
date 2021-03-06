// ConsoleApplication2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include <iostream>     // cin/cout
#include <string>
#include <vector>
#include <array>
#include <set>
#include <unordered_set>
#include <map>
#include <stack>
#include <queue>
#include <limits>       // numeric_limits<T>::max()
#include <climits>      // INT_MAX
#include <cmath>        // pow
#include <algorithm>    // sort
#include <utility>      // pair

using namespace std;

string gameOfStones(int n)
{
     // DP[i] will store winner for n = i:
    // -1 = winner/result unknown
    //  0 = false = First
    //  1 = true = Second
    static vector<int> DP(101, -1);

    // set known values for the lowest values of i (no choices, just obvious)
    // DP[1] = true (n=1 ==> no moves available, so Second wins by default) 
    DP[1] = 1;
    // also know DP[i] for each of the 3 valid moves (2, 3, or 5) ==> First wins
    DP[2] = 0;
    DP[3] = 0;
    DP[5] = 0;
    // finally, of remaining values of n, only 4 and 6 require no random(-ish) choice:
    // when n = 4, First can remove 3 stones, leaving one (and win)
    // when n = 6, First can remove 5 stones, leaving one (and win)
    DP[4] = 0;
    DP[6] = 0;

    // valid moves are 2,3,5 (randomly? start with largest, then next largest, ...)
    unordered_set<int> moves = { 5, 3, 2 };
    auto nextmove = moves.begin();

    // three vectors of "random(-ish)" choices for each player (two previous, one current)
    // ex: choices[0][1] = vector of Second player's choices in first game 
    array<array<vector<int>,2>,3> choices;

    // trynewchoice set if move choice made in last game (even if that choice was none)
    // trynewchoice required for tryagain reset (+ loser=First + more choices left to try)
    bool tryagain = true;
    bool trynewchoice = false;

    // save initial start value for retries
    int startN = n;

    // First always starts
    bool turn = 0;
    int game = 0;
    while (tryagain)
    {
        // reset new try flags
        tryagain = false;
        trynewchoice = false;

        // game loop
        while (n > 1)
        {
            // when n = one of the 3 valid moves, player can remove all stones,
            // leaving no possible moves for next player (and win)
            if (moves.find(n) != moves.end())
            {
                break;
            }
            // when n = 4, player can remove 3 stones, leaving one (and win)
            // when n = 6, player can remove 5 stones, leaving one (and win)
            if ((n == 4) || (n == 6))
            {
                break;
            }

            // 4 and 6 are the last 2 values that can be decided without user choice
            // going forward (for n >=7), unless the lower entries in the DP array 
            // are holding results saved from previous runs, the game will require 
            // some kind of random (or AI) choice from one or both players...

            // first check the DP array

            // if this move leaves a number that would set winner = me, this indicates 
            // a winning choice for player Second, but for player First the opposite 
            // is actually true (as for the game "starting" with the next move,    
            // player Second effectively now becomes player First, and vice versa)
            // so if I'm Second, and any moves result in winner = me, I win 
            // but if I'm First, and any moves result in winner = !me, I win
            // i.e. if turn==0, check for winner==1, if turn==1, check for winner==1...
            if ((DP[n - 2] == 1) || (DP[n - 3] == 1) || (DP[n - 5] == 1))
            {
                break;
            }
            // or, if any and all possible moves would set winner as described above,
            // then current player can't win (turn also tracks winner, so swap and break)
            if ((DP[n - 2] == 0) && (DP[n - 3] == 0) && (DP[n - 5] == 0))
            {
                turn = !turn;
                break;
            }

            // last resort, random(-ish) choice (so can set choice flag for retries)
            trynewchoice = true;

            // first, continue to try 5 while n >= 10 (5+3+2=10 = sum of valid moves),
            // as long as n-5 result is not known (if known = turn = bad)
            // then, for n < 10, try each valid move in turn (randomly? starting with 
            // 5 -- again, as long as n-5 result is not known), stopping when the  
            // chosen move results in a win for First player (or all options exhausted)
            int move = 0;
            // (n >= 10) can prevents winner resets (mistake move is made above 10)
            //if ( ( (*nextmove == 5) || (n >= 10) ) && (DP[n - 5] == -1) )
            if ( (*nextmove == 5) && (DP[n - 5] == -1) )
            {
                // skip if choosing 5 will result in one last valid move for next player
                int rem = (n - 5);
                if ((rem != 5) && (rem != 3) && (rem != 2))
                {
                    move = 5;
                }
            }
            // next, if 3 is next move to try, or if 5 was scheduled to be the next
            // move to try but not chosen, try 3 as long as n-3 result is not
            // known (again, if known = turn = bad)
            if ( (*nextmove == 3) && (move == 0) && (DP[n - 3] == -1))
            {
                // skip if choosing 3 will result in one last valid move for next player
                int rem = (n - 3);
                if ((rem != 5) && (rem != 3) && (rem != 2))
                {
                    move = 3;
                }
            }
            // finally, if 2 is the next move to try, or if no move has been chosen  
            // from either 3 or 5, try 2 (as long as n-2 result is not known)
            if ( (*nextmove == 2) && (move == 0) && (DP[n - 2] == -1))
            {
                // skip if choosing 2 will result in one last valid move for next player
                int rem = (n - 2);
                if ((rem != 5) && (rem != 3) && (rem != 2))
                {
                    move = 2;
                }
            }

            // save this player's choice (even if choice was no move at all)
            choices[game][turn].push_back(move);


            // if no move was chosen, it's because the player can't win
            // too bad, gotta bite the bullet and do something (and then lose)
            // turn tracks winner, so swap and break
            if (move == 0)
            {
                turn = !turn;
                break;
            }

            // update n, and pass turn to next player
            n -= move;
            turn = !turn;
        }

        // if broke out of loop before end of game (n > 1), winner = turn
        // if loop completed due to no more moves (n <= 1), winner = !turn
        // if startN = 1 (n == 1 because loop never ran), winner = !turn
        if (n <= 1)
        {
            turn = !turn;
        }

        // if a random(-ish) choice was made in this game,
        // and the result is a new winner (either this a   
        // retry game or previous winner unknown / -1)
        // and that is due to the winner changing a previous choice
        // try a different default random(-ish) move (if any left...)
        // (only resetting for winner = Second was wrong logic)
        // (if new winner, but due to change made by prev winner,
        //  keep prev winner - this change was a mistake)
        // if was change by winner, prev was a mistake, keep new winner
        // if winner hasn;t changed, just try once more (give other guy
        // one last chance to fix things)
        // if winner chosen entirely from DP, dont retry (wasted of time)
        if ( trynewchoice == true ) 
        {    
            // who was the previous winner?
            int previousWinner = DP[startN];

            // if this was a retry and the winner changed, 
            // was it because of a choice change by the new winner?
            // if so, they win, done
            // if was choice change by prev winner, just a mistake
            // in that case keep previous winner and retry one more time
            bool quitGame = false;
            if ((game > 0) && (previousWinner != turn))
            {
                // compare new winner's current and previous choices
                vector<int> &newChoices = choices[game][turn];
                vector<int> &prevChoices = choices[game - 1][turn];

                // if vectors are equal, reset to previous winner and try again
                bool undoWinnerChange = true;
                if (newChoices.size() == prevChoices.size())
                {
                    for (int i = 0; i < newChoices.size(); ++i)
                    {
                        if (newChoices[i] != prevChoices[i])
                        {
                            // found a choice change, this guy's the winner
                            undoWinnerChange = false;
                            break;
                        }
                    }
                }
                else
                {
                    // number of choices are not the same, this guy's the winner
                    undoWinnerChange = false;
                }

                if (undoWinnerChange)
                {
                    turn = !turn;
                }
                else
                {
                    // skip the next retry, we're done
                    quitGame = true;
                }
            }

            // try again, unless previous retry found the right winner (then quitGame = true)
            if (!quitGame)
            {
                // if new game number indicates all possible valid moves tried, done 
                game++;
                if (game < moves.size())
                {
                    // reset game and try again:
                    // restore n to initial value
                    // First goes first
                    // set a new "preferred" choice
                    tryagain = true;
                    n = startN;
                    turn = 0;
                    advance(nextmove, 1);
                }
            }
        }

        // save the result for this value of n
        DP[startN] = turn;
    }

    // save the result for this value of n
    //DP[startN] = turn;

    return (turn == 0 ? "First" : "Second");
}


int main()
{
    //cout << gameOfStones(7) << endl;
    //cout << gameOfStones(8) << endl;
    //cout << gameOfStones(9) << endl;
    //cout << gameOfStones(10) << endl;
    cout << gameOfStones(55) << endl;
    //cout << gameOfStones(34) << endl;
    //cout << gameOfStones(92) << endl;
    return 0;
}