#include <iostream>
#include <windows.h>
#include <thread>
#include <vector>
using namespace std;

wstring tetromino[7];

// Field Definition
const int FIELD_WIDTH = 12;
const int FIELD_HEIGHT = 18;

int nScreenWidth = 120;
int nScreenHeight = 30;

unsigned char *pField = nullptr;

/*
    The rotation formula:
        0:      i = y*w + x
        90:     i = 12 + y - (x*4)
        180:    i = 15 - (y*4) - x
        270:    i = 3 + y*(x*4)
*/
int Rotate(int px, int py, int r){
    switch (r % 4)
    {
    case 0:
        return py * 4 + px;
    case 1:
        return 12 + py - (px*4);
    case 2:
        return 15 - (py*4) - px;
    case 3:
        return 3 - py + (px*4);
    default:
        return 0;
    }
}

bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY){
    for (int px = 0; px < 4; px++)
    {
        for (int py = 0; py < 4; py++)
        {
            int pi = Rotate(px, py, nRotation);

            int fi = (nPosY + py) * FIELD_WIDTH + (nPosX + px);

            if (nPosX + px >= 0 && nPosX + px < FIELD_WIDTH)
            {
                if (nPosY + py >= 0 && nPosY + py < FIELD_HEIGHT)
                {
                    if (tetromino[nTetromino][pi] == L'X' && pField[fi] != 0)
                    {
                        return false;
                    }
                    
                }
                
            }
            
        }
        
    }
    return true;
}

int main()
{
    // Initial
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");

    tetromino[1].append(L"..X.");
    tetromino[1].append(L".XX.");
    tetromino[1].append(L".X..");
    tetromino[1].append(L"....");

    tetromino[2].append(L".X..");
    tetromino[2].append(L".XX.");
    tetromino[2].append(L"..X.");
    tetromino[2].append(L"....");
    
    tetromino[3].append(L"....");
    tetromino[3].append(L".XX.");
    tetromino[3].append(L".XX.");
    tetromino[3].append(L"....");

    tetromino[4].append(L"..X.");
    tetromino[4].append(L".XX.");
    tetromino[4].append(L"..X.");
    tetromino[4].append(L"....");
    
    tetromino[5].append(L"....");
    tetromino[5].append(L".XX.");
    tetromino[5].append(L"..X.");
    tetromino[5].append(L"..X.");
    
    tetromino[6].append(L"....");
    tetromino[6].append(L".XX.");
    tetromino[6].append(L".X..");
    tetromino[6].append(L".X..");

    pField = new unsigned char[FIELD_WIDTH*FIELD_HEIGHT];
    for (int x = 0; x < FIELD_WIDTH; x++)
    {
        for (int y = 0; y < FIELD_HEIGHT; y++)
        {
            pField[y*FIELD_WIDTH + x] = ( (x == 0) || (x == FIELD_WIDTH - 1) || (y == FIELD_HEIGHT - 1)) ? 9 : 0;
        }
    }

    wchar_t *screen = new wchar_t[nScreenWidth*nScreenHeight];
    for (int i = 0; i < nScreenHeight*nScreenWidth; i++)
    {
        screen[i] = L' ';
    }
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwByteWritten = 0;
    

    // Running
    bool bGameOver = false;

    int nCurrentPiece = 1;
    int nCurrentRotation = 0;
    int nCurrentX = FIELD_WIDTH/2;
    int nCurrentY = 0;

    int nScore = 0;

    while (!bGameOver)
    {       
        // Draw field
        for (int x = 0; x < FIELD_WIDTH; x++)
            for (int y = 0; y < FIELD_HEIGHT; y++)
                screen[(y + 2)*nScreenWidth + (x + 2)] = L" ABCDEFG=#"[pField[y * FIELD_WIDTH + x]];

        
        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth*nScreenHeight, {0,0}, &dwByteWritten);
    }

    CloseHandle(hConsole);
    cout<< "Game Over! \n Score: " << nScore << endl;
    system("pause");

    return 0;
}

