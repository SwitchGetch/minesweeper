#include <iostream>
#include <vector>
#include <time.h>
#include <vector>
#include <windows.h>
using namespace std;

HANDLE hand = GetStdHandle(STD_OUTPUT_HANDLE);
COORD cord;

void FieldCreation(vector<vector<char>>& field, vector<vector<char>>& mask)
{
    for (int i = 0; i < 11; i++)
    {
        vector<char> FieldLine;
        vector<char> MaskLine;

        for (int j = 0; j < 11; j++)
        {
            if (i == 0)
            {
                if (j == 0 || j == 10)
                {
                    FieldLine.push_back(' ');

                    MaskLine.push_back(' ');
                }
                else
                {
                    FieldLine.push_back(j + '0');

                    MaskLine.push_back(' ');
                }
            }
            else if (j == 0)
            {
                if (i == 10)
                {
                    FieldLine.push_back(' ');

                    MaskLine.push_back(' ');
                }
                else
                {
                    FieldLine.push_back(i + '0');

                    MaskLine.push_back(' ');
                }
            }
            else
            {
                if (!(rand() % 8))
                {
                    if (i < 10 && j < 10)
                    {
                        FieldLine.push_back('*');
                    }
                    else
                    {
                        FieldLine.push_back(' ');
                    }
                }
                else
                {
                    FieldLine.push_back(' ');
                }

                if (i < 10 && j < 10)
                {
                    MaskLine.push_back('#');
                }
                else
                {
                    MaskLine.push_back(' ');
                }
            }
        }

        field.push_back(FieldLine);
        mask.push_back(MaskLine);
    }

    for (int i = 1; i < 10; i++)
    {
        for (int j = 1; j < 10; j++)
        {
            if (field[i][j] == ' ')
            {
                int count = 0;

                for (int i0 = i - 1; i0 < i + 2; i0++)
                {
                    for (int j0 = j - 1; j0 < j + 2; j0++)
                    {
                        if (field[i0][j0] == '*')
                        {
                            count++;
                        }
                    }
                }

                field[i][j] = count + '0';
            }
        }
    }

    return;
}

void FieldOutput(vector<vector<char>>& field, vector<vector<char>>& mask)
{
    /*cord.X = 0;
    cord.Y = 0;
    SetConsoleCursorPosition(hand, cord);*/

    system("cls");

    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            if (mask[i][j] == ' ')
            {
                if (i == 0 || j == 0)
                {
                    SetConsoleTextAttribute(hand, 15);
                }
                else if (field[i][j] != '*')
                {
                    SetConsoleTextAttribute(hand, field[i][j] - '0');
                }
                else
                {
                    SetConsoleTextAttribute(hand, 12);
                }

                cout << field[i][j] << " ";
            }
            else
            {
                SetConsoleTextAttribute(hand, 15);

                cout << mask[i][j] << " ";
            }
        }

        cout << endl;
    }
}

bool Check(vector<vector<char>>& field, vector<vector<char>>& mask)
{
    int MasksNumber = 0, BombsNumber = 0;

    for (int i = 1; i < 10; i++)
    {
        for (int j = 1; j < 10; j++)
        {
            if (field[i][j] == '*')
            {
                BombsNumber++;
            }

            if (mask[i][j] == '#')
            {
                MasksNumber++;
            }
        }
    }

    return MasksNumber == BombsNumber;
}

int main()
{
    srand(time(NULL));

    vector<vector<char>> field;
    vector<vector<char>> mask;

    FieldCreation(field, mask);

    FieldOutput(field, mask);

    while (true)
    {
        pair<int, int> pos;

        cout << "open a position: ";
        cin >> pos.first;
        cin >> pos.second;

        if (pos.first > 0 && pos.second > 0 && pos.first < 10 && pos.second < 10)
        {
            mask[pos.first][pos.second] = ' ';

            FieldOutput(field, mask);

            if (field[pos.first][pos.second] == '*')
            {
                for (int i = 0; i < 10; i++)
                {
                    for (int j = 0; j < 10; j++)
                    {
                        if (field[i][j] == '*')
                        {
                            mask[i][j] = ' ';
                        }
                    }
                }

                FieldOutput(field, mask);

                Sleep(1000);

                system("cls");

                SetConsoleTextAttribute(hand, 12);

                cout << "game over!" << endl;

                break;
            }

            if (Check(field, mask))
            {
                Sleep(1000);

                system("cls");

                SetConsoleTextAttribute(hand, 10);

                cout << "victory!" << endl;

                break;
            }
        }
    }
}
