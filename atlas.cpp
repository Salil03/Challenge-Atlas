#include <bits/stdc++.h>
#include "prettyprint.hpp"
#include <random>
using namespace std;

const int ALPHABET = 26;
std::mt19937 rng(std::random_device{}());

char user_input(string curr)
{
    cout << "Enter letter: " << curr;
    char in;
    cin >> in;
    return in;
}

bool randomBool()
{
    return std::uniform_int_distribution<>{0, 1}(rng);
}

pair<char, long double> computer_move(string curr, vector<string> unordered_places, int length)
{
    int curr_length = curr.length();
    vector<vector<string>> allowed(ALPHABET);
    bool valid = false;
    for (int i = 0; i < length; i++)
    {
        if (unordered_places[i] == curr)
        {
            return make_pair('0', 0);
        }
        if (unordered_places[i].compare(0, curr_length, curr) == 0)
        {
            allowed[unordered_places[i][curr_length] - 'a'].emplace_back(unordered_places[i]);
            valid = true;
        }
    }
    if (!valid)
    {
        return make_pair('1', 0);
    }
    vector<long double> strikes(ALPHABET, 10000);
    for (int i = 0; i < ALPHABET; i++)
    {
        if (!allowed[i].empty())
        {
            strikes[i] = 0;
        }
        for (string s : allowed[i])
        {
            if ((s.length() - curr_length) % 2 == 1)
            {
                strikes[i]++;
            }
        }
        if (strikes[i] == 0)
        {
            return make_pair('a' + i, 0);
        }
        strikes[i] = 10000;
    }
    for (int i = 0; i < ALPHABET; i++)
    {
        char one = 'a' + i;
        if (!allowed[i].empty() && count(unordered_places.begin(), unordered_places.end(), curr + one) == 0)
        {
            strikes[i] = 0;
            for (int j = 0; j < ALPHABET; j++)
            {
                char two = 'a' + j;
                pair<char, long double> recur = computer_move(curr + one + two, allowed[i], allowed[i].size());
                strikes[i] += recur.second;
            }
            strikes[i] /= allowed.size();
        }
        if (strikes[i] == 0)
        {
            return make_pair('a' + i, 0);
        }
    }
    cout << strikes << "\n";
    int minima1 = min_element(strikes.begin(), strikes.end()) - strikes.begin();
    int minima2 = -1;
    long double minimum2 = 10000;
    for (int i = 0; i < ALPHABET; i++)
    {
        if (i != minima1)
        {
            if (strikes[i] <= minimum2)
            {
                minima2 = i;
                minimum2 = strikes[i];
            }
        }
    }
    if ((strikes[minima1] == strikes[minima2] || strikes[minima2] != 10000) && randomBool())
    {
        return make_pair('a' + minima2, strikes[minima2]);
    }
    else
    {
        return make_pair('a' + minima1, strikes[minima1]);
    }
}

// main game loop
void game_loop(vector<string> &unordered_places)
{

    string game_string = "";
    int length = unordered_places.size();
    char in = user_input("");
    game_string += in;
    char out = computer_move(game_string, unordered_places, length).first;
    while (out != '0' && out != '1')
    {
        game_string += out;
        in = user_input(game_string);
        game_string += in;
        out = computer_move(game_string, unordered_places, length).first;
    }
    if (out == '0')
    {
        cout << "\nWIN\n";
    }
    if (out == '1')
    {
        cout << "\nINVALID\n";
    }
}

int main()
{
    ifstream places("countries.txt");
    vector<string> unordered_places;
    string place;
    if (places.is_open())
    {
        while (getline(places, place))
        {
            unordered_places.emplace_back(place);
        }
        places.close();
    }
    else
    {
        cout << "Countries not loaded properly\n";
        return 0;
    }
    game_loop(unordered_places);
}