#include <iostream>
#include <vector>
#include <time.h>
#include <vector>
#include <windows.h>
using namespace std;

HANDLE hand = GetStdHandle(STD_OUTPUT_HANDLE);

void FieldCreation(vector<vector<char>>& field, vector<vector<char>>& mask)
{
    for (int i = 0; i < 11; i++)
    {
        vector<char> FieldLine;
        vector<char> MaskLine;

        for (int j = 0; j < 11; j++)
        {
            if (i > 0 && j > 0 && i < 10 && j < 10)
            {
                MaskLine.push_back('#');
            }
            else
            {
                MaskLine.push_back(' ');
            }

            if ((i == 0 && j == 0) || i > 9 || j > 9)
            {
                FieldLine.push_back(' ');
            }
            else if (i == 0 && j < 10)
            {
                FieldLine.push_back(j + '0');
            }
            else if (j == 0 && i < 10)
            {
                FieldLine.push_back(i + '0');
            }
            else if (i > 0 && j > 0 && i < 10 && j < 10)
            {
                FieldLine.push_back('0');
            }
        }

        field.push_back(FieldLine);
        mask.push_back(MaskLine);
    }

    return;
}

void BombsNumber(vector<vector<char>>& field)
{
    for (int i = 1; i < 10; i++)
    {
        for (int j = 1; j < 10; j++)
        {
            if (field[i][j] == '0')
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
            else if (mask[i][j] == char(30))
            {
                SetConsoleTextAttribute(hand, 12);

                cout << mask[i][j] << " ";
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
    int BombsNumber = 0, FlagsNumber = 0;
    bool victory = true;

    for (int i = 1; i < 10; i++)
    {
        for (int j = 1; j < 10; j++)
        {
            if (field[i][j] == '*')
            {
                BombsNumber++;
            }

            if (mask[i][j] == char(30))
            {
                FlagsNumber++;
            }

            if (field[i][j] == '*' && mask[i][j] != char(30))
            {
                victory = false;
            }
        }
    }

    return victory && BombsNumber == FlagsNumber;
}

void OpenZeroPositions(vector<vector<char>>& field, vector<vector<char>>& mask, int x, int y)
{
    for (int i = x - 1; i < x + 2; i++)
    {
        for (int j = y - 1; j < y + 2; j++)
        {
            if (field[i][j] == '0' && mask[i][j] == '#')
            {
                mask[i][j] = ' ';

                OpenZeroPositions(field, mask, i, j);
            }
        }
    }

    for (int i = 1; i < 10; i++)
    {
        for (int j = 1; j < 10; j++)
        {
            if (field[i][j] != '0')
            {
                for (int i0 = i - 1; i0 < i + 2; i0++)
                {
                    for (int j0 = j - 1; j0 < j + 2; j0++)
                    {
                        if (field[i0][j0] == '0' && mask[i0][j0] == ' ')
                        {
                            mask[i][j] = ' ';
                        }
                    }
                }
            }
        }
    }

    return;
}

void GameOver(vector<vector<char>>& field, vector<vector<char>>& mask)
{
    if (Check(field, mask))
    {
        system("cls");

        FieldOutput(field, mask);

        SetConsoleTextAttribute(hand, 10);

        cout << "victory!" << endl;

        SetConsoleTextAttribute(hand, 15);
    }
    else
    {
        system("cls");

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

        SetConsoleTextAttribute(hand, 12);

        cout << "game over!" << endl;

        SetConsoleTextAttribute(hand, 15);
    }

    return;
}

int main()
{
    srand(time(NULL));

    vector<vector<char>> field;
    vector<vector<char>> mask;
    bool alive = true;
    int x = 0, y = 0, bombs = 9, action;

    FieldCreation(field, mask);

    BombsNumber(field);

    FieldOutput(field, mask);

    while (true)
    {
        cout << "open position: ";
        cin >> x;
        cin >> y;

        if (x > 0 && y > 0 && x < 10 && y < 10)
        {
            mask[x][y] = ' ';

            while (bombs > 0)
            {
                int tempX = 1 + rand() % 9, tempY = 1 + rand() % 9;

                if (field[tempX][tempY] == '0' && mask[tempX][tempY] != ' ')
                {
                    field[tempX][tempY] = '*';

                    bombs--;
                }
            }


            BombsNumber(field);

            if (field[x][y] == '0')
            {
                OpenZeroPositions(field, mask, x, y);
            }

            system("cls");

            FieldOutput(field, mask);

            break;
        }
        else
        {
            system("cls");

            FieldOutput(field, mask);
        }
    }

    while (alive)
    {
        cout << "actions: " << endl;
        cout << "1) open position" << endl;
        cout << "2) put flag" << endl;
        cout << "3) remove flag" << endl;
        cout << endl;

        cin >> action;
        cin >> x;
        cin >> y;

        if (action > 0 && action < 4 && x > 0 && y > 0 && x < 10 && y < 10)
        {
            if (action == 1 && mask[x][y] != char(30))
            {
                mask[x][y] = ' ';

                if (field[x][y] == '0')
                {
                    OpenZeroPositions(field, mask, x, y);
                }

                FieldOutput(field, mask);

                if (field[x][y] == '*' || Check(field, mask))
                {
                    alive = false;
                }
            }
            else if (action == 2)
            {
                if (mask[x][y] == '#')
                {
                    mask[x][y] = char(30);
                }

                FieldOutput(field, mask);

                if (Check(field, mask))
                {
                    alive = false;
                }
            }
            else if (action == 3)
            {
                if (mask[x][y] == char(30))
                {
                    mask[x][y] = '#';
                }

                FieldOutput(field, mask);

                if (Check(field, mask))
                {
                    alive = false;
                }
            }
            else
            {
                system("cls");

                FieldOutput(field, mask);
            }
        }
        else
        {
            system("cls");

            FieldOutput(field, mask);
        }
    }

    GameOver(field, mask);
}
