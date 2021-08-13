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
    int bKey[4];
    bool bRotateHold = false;

    int nSpeed = 20;
    int nSpeedCounter = 0;
    bool bForceDown = false;

    int nPieceCount = 0;
    int nScore = 0;

    vector<int> vLines;

    while (!bGameOver)
    {
        this_thread::sleep_for(50ms);
        nSpeedCounter++;
        bForceDown = (nSpeedCounter == nSpeed);

        for (int k = 0; k < 4; k++)
        {
            bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;
        }

        nCurrentX += (bKey[0] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) ? 1 : 0;
		nCurrentX -= (bKey[1] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) ? 1 : 0;		
		nCurrentY += (bKey[2] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) ? 1 : 0;
        if (bKey[3])
		{
			nCurrentRotation += (bRotateHold && DoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) ? 1 : 0;
			bRotateHold = false;
		}
		else
			bRotateHold = true;
        
        if (bForceDown)
        {
            if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1) )
            {
                nCurrentY++;
            }
            else
            {
                // Lock piece
                for (int px = 0; px < 4; px++)
                    for (int py = 0; py < 4; py++)
                        if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] == L'X')
                        {
                            pField[(nCurrentY + py) * FIELD_WIDTH + (nCurrentX + px)] = nCurrentPiece + 1;
                        }

                nPieceCount++;      
                if (nPieceCount % 10 == 0)
                {
                    if (nSpeed >= 10)      
                    {
                        nSpeed--;
                    }
                    
                }
                  
                // Check got any lines
                for (int  py = 0; py < 4; py++)
                {
                    if (nCurrentY + py < FIELD_HEIGHT - 1)
                    {
                        bool bLine= true;
                        for (int px = 1; px < FIELD_WIDTH - 1; px++)
                        { 
                            bLine &= pField[(nCurrentY + py)*FIELD_WIDTH + px] != 0;
                        }
                        
                        if (bLine)
                        {
                            for (int px = 1; px < FIELD_WIDTH - 1; px++)
                            {
                                pField[(nCurrentY + py) * FIELD_WIDTH + px] = 8;
                            }

                            vLines.push_back(nCurrentY + py);
                        }
                    }
                }

                nScore += 25;
                if(!vLines.empty())	nScore += (1 << vLines.size()) * 100;


                
                // Next piece
                nCurrentPiece = rand() % 7;
                nCurrentRotation = 0;
                nCurrentX = FIELD_WIDTH / 2;
                nCurrentY = 0;

                // if piece does not fit
                bGameOver = !DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
            }

            nSpeedCounter = 0;
        }
        
        // Draw field
        for (int x = 0; x < FIELD_WIDTH; x++)
            for (int y = 0; y < FIELD_HEIGHT; y++)
                screen[(y + 2)*nScreenWidth + (x + 2)] = L" ABCDEFG=#"[pField[y * FIELD_WIDTH + x]];

        // Draw Current Piece 
        for (int px = 0; px < 4; px++)
            for (int py = 0; py < 4; py++)
                if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] == L'X')
                {
                    screen[(nCurrentY + py + 2)*nScreenWidth + (nCurrentX + px + 2)] = nCurrentPiece + 65;
                }

        swprintf_s(&screen[2 * nScreenWidth + FIELD_WIDTH + 6], 16, L"SCORE: %8d", nScore);

        if (!vLines.empty())
        {
            WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth*nScreenHeight, {0,0}, &dwByteWritten);
            this_thread::sleep_for(400ms);

            for (auto &v: vLines)
            {
                for (int px = 1; px < FIELD_WIDTH - 1; px++)
                {
                    for (int py = v; py > 0; py--)
                    {
                        pField[py*FIELD_WIDTH + px] = pField[(py - 1) * FIELD_WIDTH + px];
                    }
                    pField[px] = 0;
                }
            }
            vLines.clear();
            
        }
        
        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth*nScreenHeight, {0,0}, &dwByteWritten);
    }

    CloseHandle(hConsole);
    cout<< "Game Over! \n Score: " << nScore << endl;
    system("pause");

    return 0;
}

