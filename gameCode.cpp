#include <iostream>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <algorithm>

using namespace std;

bool gameOver;
bool paused = false;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score, highScore = 0;
int tailX[100], tailY[100];
int nTail;
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direction dir;

void LoadHighScore() {
    ifstream infile("highscore.txt");
    if (infile.is_open()) {
        infile >> highScore;
        infile.close();
    }
}

void SaveHighScore() {
    if (score > highScore) {
        ofstream outfile("highscore.txt");
        outfile << score;
        outfile.close();
    }
}

void Setup() {
    gameOver = false;
    paused = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 0;
    LoadHighScore();
}

void Draw() {
    system("cls"); // clear screen

    for (int i = 0; i < width + 2; i++) cout << "#";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0) cout << "#";

            if (i == y && j == x)
                cout << "O"; // snake head
            else if (i == fruitY && j == fruitX)
                cout << "F"; // fruit
            else {
                bool print = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        cout << "o"; // snake body
                        print = true;
                    }
                }
                if (!print) cout << " ";
            }

            if (j == width - 1) cout << "#";
        }
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++) cout << "#";
    cout << endl;
    cout << "Score: " << score << "   High Score: " << max(score, highScore)
         << "   Level: " << (score / 50 + 1) << endl;
    cout << "Controls: W A S D to move, P to pause, X to exit" << endl;

    if (paused)
        cout << "*** PAUSED ***" << endl;
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a':
            if (dir != RIGHT) dir = LEFT;
            break;
        case 'd':
            if (dir != LEFT) dir = RIGHT;
            break;
        case 'w':
            if (dir != DOWN) dir = UP;
            break;
        case 's':
            if (dir != UP) dir = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
        case 'p':
            paused = !paused;
            break;
        }
    }
}

void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;

    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
    case LEFT: x--; break;
    case RIGHT: x++; break;
    case UP: y--; break;
    case DOWN: y++; break;
    default: break;
    }

    // Wall collision
    if (x >= width || x < 0 || y >= height || y < 0)
        gameOver = true;

    // Self collision
    for (int i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;

    // Eating fruit
    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
        Beep(750, 100); // optional sound effect
    }
}

int main() {
    Setup();

    while (!gameOver) {
        Draw();
        Input();
        if (!paused)
            Logic();

        // Increase speed as score increases (min delay = 30 ms)
        int delay = max(30, 100 - (score / 10));
        Sleep(delay);
    }

    SaveHighScore();
    cout << "\nGame Over! Final Score: " << score << endl;
    return 0;
}
