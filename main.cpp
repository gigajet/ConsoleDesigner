#include <cstdio>
#include <windows.h>
#include <cstdlib>
#include "commdlg.h"
#include <algorithm>
const HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
const HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
const char * TitleCaption = "Console Designer 0.1";
const int MAX_CANVAS_WIDTH = 120;
const int MAX_CANVAS_HEIGHT = 36;
namespace Color {
    const int Black = 0,
              Blue  = 1,
              Green = 2,
              Cyan  = 3,
              Red   = 4,
              Magenta = 5,
              Yellow = 6,
              LightGray = 7,
              DarkGray = 8,
              LightBlue = 9,
              LightGreen = 10,
              LightCyan = 11,
              LightRed = 12,
              LightMagenta = 13,
              LightYellow = 14,
              White = 15;
}
void GotoXY (int x, int y);
void SetColor (int BackgroundColor, int ForegroundColor);
void ClearScreen (int BackgroundColor = Color::Black, int ForegroundColor = Color::White);
void ClearEOL (int BackgroundColor = Color::Black, int ForegroundColor = Color::White);
void DrawPixel (int x, int y, char c = ' ');
void DrawFilledRectangle (int x1, int y1, int x2, int y2, char c = ' ');
void AdjustScreenSize (int width, int height);
void WriteTextXY (int x, int y, char * text);
void ReadInt (int &Result);
bool InRect (int x1, int y1, int x2, int y2, int x, int y);


struct CanvasCell {
    char ASCII;
    int ForegoundColor;
    int BackgroundColor;
} Cell[MAX_CANVAS_WIDTH][MAX_CANVAS_HEIGHT];
namespace Canvas {
    void SaveToFile (int height, int width, CanvasCell canvas[][MAX_CANVAS_HEIGHT], char* FileName);
    bool LoadFromFile (int &height, int &width, CanvasCell canvas[][MAX_CANVAS_HEIGHT], char* FileName);
    void Loader ();
    void Saver ();
}
struct CanvasSection {
    int height, width;
    void NewCanvas(int Width, int Height) ;
    bool InCanvas (int curX, int curY);
    void UpdateCell (int x, int y, char ASCII, int ForegroundColor, int BackgroundColor);
    void UpdateWall ();
    void Reload();
};
struct CurrentASCIISection {
    char ASCII; int x,y;
    void InitSection (int topLeftX, int topLeftY);
    void UpdateASCII (char newASCII);
};
///Each section require: 16 X-coord and 9 Y-coord to init.
///InitSection: Draw the graphics.
///ChangeColor: Change to new color, update the graphics.
struct BackgroundSection {
    int x,y,currentColor;

    ///Coordinates given relatively.
    const int ButtonX1 [0x10] = {0, 4, 8, 0xC, 0, 4, 8, 0xC, 0, 4, 8, 0xC, 0, 4, 8, 0xC};
    const int ButtonX2 [0x10] = {3, 7, 11, 15, 3, 7, 11, 15, 3, 7, 11, 15, 3, 7, 11, 15};
    const int ButtonY1 [0x10] = {1, 1, 1, 1, 3, 3, 3, 3, 5, 5, 5, 5, 7, 7, 7, 7};
    const int ButtonY2 [0x10] = {2, 2, 2, 2, 4, 4, 4, 4, 6, 6, 6, 6, 8, 8, 8, 8};

    const int CursorX [0x10] = {1, 5, 9, 13, 1, 5, 9, 13, 1, 5, 9, 13, 1, 5, 9, 13};
    const int CursorY [0x10] = {1, 1, 1, 1, 3, 3, 3, 3, 5, 5, 5, 5, 7, 7, 7, 7};

    void InitSection (int topLeftX, int topLeftY);
    void ChangeColor (int newColor);

    ///"X" cursor, colored White if currentColor=Black, otherwise X cursor colored Black.
    void UpdateCursor ();

    ///If any button color pressed, return that color button,
    ///Otherwise, return -1.
    int ButtonPressed (int curX, int curY);
};

struct ForegroundSection {
    int x,y,currentColor;

    ///Coordinates given relatively.
    const int ButtonX1 [0x10] = {0, 4, 8, 0xC, 0, 4, 8, 0xC, 0, 4, 8, 0xC, 0, 4, 8, 0xC};
    const int ButtonX2 [0x10] = {3, 7, 11, 15, 3, 7, 11, 15, 3, 7, 11, 15, 3, 7, 11, 15};
    const int ButtonY1 [0x10] = {1, 1, 1, 1, 3, 3, 3, 3, 5, 5, 5, 5, 7, 7, 7, 7};
    const int ButtonY2 [0x10] = {2, 2, 2, 2, 4, 4, 4, 4, 6, 6, 6, 6, 8, 8, 8, 8};

    const int CursorX [0x10] = {1, 5, 9, 13, 1, 5, 9, 13, 1, 5, 9, 13, 1, 5, 9, 13};
    const int CursorY [0x10] = {1, 1, 1, 1, 3, 3, 3, 3, 5, 5, 5, 5, 7, 7, 7, 7};

    void InitSection (int topLeftX, int topLeftY);
    void ChangeColor (int newColor);

    ///"X" cursor, colored White if currentColor=Black, otherwise X cursor colored Black.
    void UpdateCursor ();

    ///If any button color pressed, return that color button,
    ///Otherwise, return -1.
    int ButtonPressed (int curX, int curY);
};
struct InstructSection {
    int x, y;
    void InitSection (int topLeftX, int topLeftY) {
        x=topLeftX; y=topLeftY;
        WriteInstruction();
    }
    void WriteInstruction () {
        SetColor(Color::Black, Color::White);
        WriteTextXY(x,y,"F1: New file");
        WriteTextXY(x,y+1,"F2: Open file");
        WriteTextXY(x,y+2,"F3: Save file");
        WriteTextXY(x,y+3,"F4: Exit");
        WriteTextXY(x,y+4,"F5: Resize canvas");
        WriteTextXY(x,y+5,"F6: Change ASCII");
    }
};

void ProcessKeyboardEvent (KEY_EVENT_RECORD event);
void ProcessMouseEvent (MOUSE_EVENT_RECORD event);
void InitScreen ();

void NewFile ();
void LoadFile ();
void SaveFile ();
void ResizeCanvas ();
void PromptASCII();

CanvasSection canvas;
BackgroundSection background;
ForegroundSection foreground;
InstructSection instruct;
CurrentASCIISection ascii;
bool ExitSignal = 0;

int main (int argc, char ** argv) {
    ///Turn off the console cursor.
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.bVisible = false; cursorInfo.dwSize = 100;
    SetConsoleCursorInfo(hout, &cursorInfo);

    SetConsoleMode(hin, ENABLE_EXTENDED_FLAGS);
    SetConsoleMode(hin, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);
    ///Default color and caption
    SetColor(Color::Black, Color::White);
    SetConsoleTitle(TitleCaption);

    canvas.NewCanvas(MAX_CANVAS_WIDTH, MAX_CANVAS_HEIGHT);
    InitScreen();

    INPUT_RECORD InputRecord; DWORD numEventsRead;

    while (!ExitSignal) {
        ReadConsoleInput(hin, &InputRecord, 1, &numEventsRead);
        switch (InputRecord.EventType) {
        case KEY_EVENT:
            ProcessKeyboardEvent (InputRecord.Event.KeyEvent); break;
        case MOUSE_EVENT:
            ProcessMouseEvent (InputRecord.Event.MouseEvent); break;
        default : break;
        }
    }
    return 0;
}

void GotoXY (int x, int y) {
    SetConsoleCursorPosition(hout, {x,y});
}
void SetColor (int BackgroundColor, int ForegroundColor) {
    SetConsoleTextAttribute(hout, BackgroundColor*16+ForegroundColor);
}
void ClearScreen (int BackgroundColor, int ForegroundColor) {
    int color = BackgroundColor * 16 + ForegroundColor;
    DWORD written;
    FillConsoleOutputCharacter(hout, ' ', 9999, {0, 0}, &written);
    FillConsoleOutputAttribute(hout, color, 9999, {0, 0}, &written);
}
void ClearEOL (int BackgroundColor, int ForegroundColor) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hout, &csbi);
    SetColor(BackgroundColor, ForegroundColor);
    for (int w=csbi.dwCursorPosition.X; w<csbi.dwSize.X; ++w)
        printf(" ");
}
void DrawPixel (int x, int y, char c) {
    SetConsoleCursorPosition(hout, {x,y});
    printf("%c",c);
}
void WriteTextXY (int x, int y, char * text) {
    GotoXY(x,y);
    printf("%s",text);
}
void DrawFilledRectangle (int x1, int y1, int x2, int y2, char c) {
    for (int x=x1; x<=x2; x++)
        for (int y=y1; y<=y2; y++)
            DrawPixel(x, y, c);
}
void AdjustScreenSize (int width, int height) {
//    COORD dwSize = {width, height};
//    SetConsoleScreenBufferSize(hout, dwSize);

    char cmd[128];
    sprintf(cmd,"mode %d,%d\0",width,height);
    system(cmd);
    SetConsoleMode(hin, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);
}
bool InRect (int x1, int y1, int x2, int y2, int x, int y) {
    return (x>=x1 && x<=x2 && y>=y1 && y<=y2);
}
void ReadInt (int &Result) {
    ///Read input until "\n" is recorded.
    INPUT_RECORD InputRecord; DWORD NumEventRead;
    char buffer[256]; int bufferSize=0;
    bool NewLineEncountered = 0;
    while (!NewLineEncountered) {
        ReadConsoleInput(hin, &InputRecord, 1, &NumEventRead);

        if (InputRecord.EventType == KEY_EVENT && InputRecord.Event.KeyEvent.bKeyDown) {
            KEY_EVENT_RECORD event = InputRecord.Event.KeyEvent;

            if (event.wVirtualKeyCode == VK_BACK) {
                if (bufferSize==0) continue;
                printf("%c",8);
                SetColor(Color::Black, Color::White); printf(" ");
                printf("%c",8);
                if (bufferSize>0) buffer[--bufferSize]='\0';
                continue;
            }

            if (event.wVirtualKeyCode == VK_RETURN) {
                printf("\n");
                NewLineEncountered=1; continue;
            }

            bool isNumber = 0; char WhatNumber;
            for (char ch='0'; ch<='9'; ch++)
                if (event.wVirtualKeyCode == VkKeyScan(ch)) {
                    isNumber=1; WhatNumber=ch; break;
                }
            if (isNumber) {
                buffer[bufferSize++] = WhatNumber;
                printf("%c",WhatNumber);
            }
        }
    }
    buffer[bufferSize++]='\0';
    sscanf(buffer,"%d",&Result);
}
void Canvas::SaveToFile (int width, int height, CanvasCell canvas[][MAX_CANVAS_HEIGHT], char* FileName) {
    FILE * hFile = fopen(FileName, "wb");
    fprintf(hFile, "%d %d\n",width,height);

    for (int w=0; w<width; w++)
    for (int h=0; h<height; h++) {
        fwrite(&canvas[w][h], sizeof(CanvasCell), 1, hFile);
    }
    fclose(hFile);
}
bool Canvas::LoadFromFile (int &width, int &height, CanvasCell canvas[][MAX_CANVAS_HEIGHT] ,char* FileName) {
    FILE * hFile = fopen(FileName, "rb");
    fscanf(hFile, "%d %d\n",&width,&height);

    for (int w=0; w<width; w++)
    for (int h=0; h<height; h++) {
        fread(&canvas[w][h], sizeof(CanvasCell), 1, hFile);
    }
    fclose(hFile);
    return 1;
}
void BackgroundSection::InitSection (int topLeftX, int topLeftY) {
    x = topLeftX; y = topLeftY; currentColor = Color::White;
    SetColor(Color::Black, Color::White);
    WriteTextXY (x, y, "BACKGROUND COLOR");
    for (int color=0; color<16; color++) {
        SetColor(color, Color::Black);
        DrawFilledRectangle(x+ButtonX1[color],
                            y+ButtonY1[color],
                            x+ButtonX2[color],
                            y+ButtonY2[color]);
    }
    UpdateCursor();
}
void BackgroundSection::ChangeColor (int newColor) {
    ///Unchoose old color
    SetColor(currentColor, Color::Black);
    DrawFilledRectangle(x+ButtonX1[currentColor],
                        y+ButtonY1[currentColor],
                        x+ButtonX2[currentColor],
                        y+ButtonY2[currentColor]);
    currentColor = newColor;
    UpdateCursor();
}
void BackgroundSection::UpdateCursor () {
    int cursorColor = (currentColor == Color::Black) ? Color::White : Color::Black;
    SetColor(currentColor, cursorColor);
    DrawPixel(x+CursorX[currentColor], y+CursorY[currentColor], 'X');
}
///If any button color pressed, return that color button,
///Otherwise, return -1.
int BackgroundSection::ButtonPressed (int curX, int curY) {
    for (int id=0; id<16; ++id)
        if (InRect (x+ButtonX1[id],
                    y+ButtonY1[id],
                    x+ButtonX2[id],
                    y+ButtonY2[id],
                    curX,
                    curY))
            return id;
    return -1;
}
void ForegroundSection::InitSection (int topLeftX, int topLeftY) {
    x = topLeftX; y = topLeftY; currentColor = Color::Black;
    SetColor(Color::Black, Color::White);
    WriteTextXY (x, y, "FOREGROUND COLOR");
    for (int color=0; color<16; color++) {
        SetColor(color, Color::Black);
        DrawFilledRectangle(x+ButtonX1[color],
                            y+ButtonY1[color],
                            x+ButtonX2[color],
                            y+ButtonY2[color]);
    }
    UpdateCursor();
}
void ForegroundSection::ChangeColor (int newColor) {
    ///Unchoose old color
    SetColor(currentColor, Color::Black);
    DrawFilledRectangle(x+ButtonX1[currentColor],
                        y+ButtonY1[currentColor],
                        x+ButtonX2[currentColor],
                        y+ButtonY2[currentColor]);
    currentColor = newColor;
    UpdateCursor();
}
///"X" cursor, colored White if currentColor=Black, otherwise X cursor colored Black.
void ForegroundSection::UpdateCursor () {
    int cursorColor = (currentColor == Color::Black) ? Color::White : Color::Black;
    SetColor(currentColor, cursorColor);
    DrawPixel(x+CursorX[currentColor], y+CursorY[currentColor], 'X');
}
///If any button color pressed, return that color button,
///Otherwise, return -1.
int ForegroundSection::ButtonPressed (int curX, int curY) {
    for (int id=0; id<16; ++id)
        if (InRect (x+ButtonX1[id],
                    y+ButtonY1[id],
                    x+ButtonX2[id],
                    y+ButtonY2[id],
                    curX,
                    curY))
            return id;
    return -1;
}

void CanvasSection::NewCanvas(int Width, int Height) {
    width = Width; height = Height;
    ///New
    for (int h=0; h<height; h++)
        for (int w=0; w<width; w++)
            UpdateCell(w, h, ' ', Color::White, Color::Black);
    UpdateWall();
}
bool CanvasSection::InCanvas (int curX, int curY) {
    return InRect(0, 0, width-1, height-1, curX, curY);
}
void CanvasSection::UpdateCell (int x, int y, char ASCII, int ForegroundColor, int BackgroundColor) {
    ///Update the data.
    Cell[x][y] = {ASCII, ForegroundColor, BackgroundColor};
    ///Update the graphics.
    SetColor(BackgroundColor, ForegroundColor);
    DrawPixel(x,y,ASCII);
}
void CanvasSection::UpdateWall () {
    SetColor(Color::Magenta, Color::Magenta);
    for (int h=0; h<=height; h++)
        DrawPixel(width, h, ' '), DrawPixel(width+1, h, ' ');
    for (int w=0; w<=width; w++)
        DrawPixel(w, height, ' ');
}
void CanvasSection::Reload() {
    for (int x=0; x<width; ++x)
        for (int y=0; y<height; ++y)
            UpdateCell(x,y,Cell[x][y].ASCII,Cell[x][y].ForegoundColor,Cell[x][y].BackgroundColor);
}
void ProcessKeyboardEvent (KEY_EVENT_RECORD event) {
    if (FALSE == event.bKeyDown) {
        switch (event.wVirtualKeyCode) {
        case VK_F1: NewFile(); break;
        case VK_F2: LoadFile(); break;
        case VK_F3: SaveFile(); break;
        case VK_F4: ExitSignal=1; break;
        case VK_F5: ResizeCanvas(); break;
        case VK_F6: PromptASCII(); break;
        default: break;
        }
    }
}
void ProcessMouseEvent (MOUSE_EVENT_RECORD event) {
    int x = event.dwMousePosition.X, y = event.dwMousePosition.Y;
    if (event.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
            if (canvas.InCanvas(x,y))
                canvas.UpdateCell(x,y,ascii.ASCII,foreground.currentColor,background.currentColor);
            int colorButton = background.ButtonPressed(x,y);
            if (colorButton != -1) {
                background.ChangeColor(colorButton);
            }
            colorButton = foreground.ButtonPressed(x,y);
            if (colorButton != -1) {
                foreground.ChangeColor(colorButton);
            }
        }
    if (event.dwButtonState == RIGHTMOST_BUTTON_PRESSED) {
            if (canvas.InCanvas(x, y))
                canvas.UpdateCell(x, y, ' ', foreground.currentColor, background.currentColor);
        }
//    if (event.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
//        if (canvas.InCanvas(x,y))
//            canvas.UpdateCell(x,y,ascii.ASCII,foreground.currentColor,background.currentColor);
//        int colorButton = foreground.ButtonPressed(x,y);
//        if (colorButton != -1) foreground.ChangeColor(colorButton);
//        colorButton = background.ButtonPressed(x,y);
//        if (colorButton != -1) background.ChangeColor(colorButton);
//    }
//    if (event.dwButtonState & RIGHTMOST_BUTTON_PRESSED) {
//        if (canvas.InCanvas(x, y))
//            canvas.UpdateCell(x, y, ' ', foreground.currentColor, background.currentColor);
//    }
}
void InitScreen () {
    ///Adjust the screen;
    canvas.width = std::min(canvas.width, MAX_CANVAS_WIDTH);
    canvas.height = std::min(canvas.height, MAX_CANVAS_HEIGHT);
    int screenWidth = canvas.width + 22;
    int screenHeight = std::max(canvas.height+1, 32);
    AdjustScreenSize(screenWidth, screenHeight);

    ClearScreen();

//    for (int w=0; w<canvas.width; w++)
//        for (int h=0; h<canvas.height; h++)
//            canvas.UpdateCell(w, h, Cell[w][h].ASCII, Cell[w][h].ForegoundColor, Cell[w][h].BackgroundColor);
    canvas.Reload();
    canvas.UpdateWall();
    background.InitSection(canvas.width + 3, 0);
    foreground.InitSection(canvas.width + 3, 12);
    instruct.InitSection(canvas.width + 3, 24);
    ascii.InitSection(canvas.width + 3, 22);
}
void NewFile () {
    for (int w=0; w<canvas.width; ++w)
    for (int h=0; h<canvas.height; ++h)
    Cell[w][h] = {' ', Color::White, Color::Black};
    canvas.Reload();
}
void LoadFile () {
    const int BUFFER_SIZE = 512;
    char filename [BUFFER_SIZE];
    for (int i=0; i<BUFFER_SIZE; ++i) filename[i]='\0';
    OPENFILENAME ofn;
    ofn.lStructSize = sizeof(tagOFNA);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = NULL;
    ofn.lpstrCustomFilter = NULL;
    ofn.nFilterIndex = 0;
    ofn.lpstrFileTitle = NULL;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = NULL;
    ofn.lpstrFile = filename;
    ofn.nMaxFile = BUFFER_SIZE;
    ofn.Flags = (OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST);

    if (GetOpenFileName(&ofn)) {
        Canvas::LoadFromFile(canvas.width,
                             canvas.height,
                             Cell,
                             filename);
        InitScreen();
    }
}
void SaveFile () {
    const int BUFFER_SIZE = 512;
    char filename [BUFFER_SIZE];
    for (int i=0; i<BUFFER_SIZE; ++i) filename[i]='\0';
    OPENFILENAME ofn;
    ofn.lStructSize = sizeof(tagOFNA);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = NULL;
    ofn.lpstrCustomFilter = NULL;
    ofn.nFilterIndex = 0;
    ofn.lpstrFileTitle = NULL;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = NULL;
    ofn.lpstrFile = filename;
    ofn.lpstrDefExt = NULL;
    ofn.nMaxFile = BUFFER_SIZE;
    ofn.Flags = (OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT);

    if (GetSaveFileName(&ofn)) {
        Canvas::SaveToFile(canvas.width,
                           canvas.height,
                           Cell,
                           filename);
    }
}
void ResizeCanvas () {
    ClearScreen();
    GotoXY(0,0); SetColor(Color::Black, Color::White);
    printf("New width (positive, not exceed %d): ",MAX_CANVAS_WIDTH); ReadInt(canvas.width);
    printf("New height (positive, not exceed %d): ",MAX_CANVAS_HEIGHT); ReadInt(canvas.height);
    InitScreen();
}
void CurrentASCIISection::InitSection (int topLeftX, int topLeftY) {
    x=topLeftX; y=topLeftY;
    UpdateASCII(' ');
}
void CurrentASCIISection::UpdateASCII (char newASCII) {
    ASCII = newASCII;
    SetColor(Color::Black, Color::White);
    GotoXY(x,y);
    printf("CURRENT ASCII: ");
    SetColor(Color::White, Color::Black);
    printf("%c",ASCII);
}
void PromptASCII () {
    SetColor(Color::Black, Color::White);
    GotoXY(ascii.x, ascii.y);
    ClearEOL();
    GotoXY(ascii.x, ascii.y);

    const int sMaxSize=5;
    char s[sMaxSize];
    for (int i=0; i<sMaxSize; i++) s[i]='\0';
    char c;
    int sSize=0;
    printf("ASCII: ");

    ///User-defined FUCKING INPUT BUFFER.
    INPUT_RECORD InputRecord; DWORD NumEventsRead;
    bool NewLineEncountered=0;
    while (!NewLineEncountered) {
        ReadConsoleInput(hin, &InputRecord, 1, &NumEventsRead);
        if (InputRecord.EventType == KEY_EVENT && InputRecord.Event.KeyEvent.bKeyDown) {
            KEY_EVENT_RECORD event = InputRecord.Event.KeyEvent;
            if (event.wVirtualKeyCode == VK_BACK) {
                if (sSize==0) continue;
                printf("%c",8); SetColor(Color::Black, Color::White); printf(" "); printf("%c",8);
                if (sSize>0) s[--sSize]='\0';
                continue;
            }
            if (event.wVirtualKeyCode == VK_RETURN) {
                NewLineEncountered = 1; continue;
            }
            if (sSize < sMaxSize) {
                s[sSize++] = event.uChar.AsciiChar;
                printf("%c",s[sSize-1]);
            }
        }
    }

    if (s[0]=='\\') {
        int d;
        if (s[1]>='0' && s[1]<='9') {
            sscanf(s+1,"%d",&d); c=(char)d;
        }
        else c=s[1];
    }
    else c=s[0];

    GotoXY(ascii.x, ascii.y); ClearEOL();
    ascii.UpdateASCII(c);
}
