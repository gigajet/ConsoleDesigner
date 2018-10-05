#include <cstdio>
#include <windows.h>
#include <cstdlib>
#include <commdlg.h>
const HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
const HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
const char * TitleCaption = "Console Designer 0.1";
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

struct CanvasCell {
    char ASCII;
    int ForegoundColor;
    int BackgroundColor;
} canvas[30][30];
void GotoXY (int x, int y);
void SetColor (int BackgroundColor, int ForegroundColor);
void ClearScreen (int BackgroundColor = Color::Black, int ForegroundColor = Color::White);
void DrawPixel (int x, int y, char c = ' ');
void DrawFilledRectangle (int x1, int y1, int x2, int y2, char c = ' ');
void AdjustScreenSize (int width, int height);
void WriteTextXY (int x, int y, char * text);
bool InRect (int x1, int y1, int x2, int y2, int x, int y);

//#define TESTMODE
namespace Canvas {
    int width, height;

    void SaveToFile (int height, int width, CanvasCell** canvas, char* FileName);
    bool LoadFromFile (int &height, int &width, CanvasCell** canvas, char* FileName);
}

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
    }
};

int main (int argc, char ** argv) {
    #ifdef TESTMODE
        system("mode 90, 36");
        ClearScreen();
        GotoXY (5, 10); printf("d");
        SetColor(Color::Green, Color::Red);
        DrawPixel(5, 15);
        for (int x=0; x<90; x++) for (int y=0; y<42; y++)
            DrawPixel(x,y);

    #else

    char CurrentFileName[256] = "UNTITLED\0";
    int height = 36, width = 80;
    AdjustScreenSize(width, height);
    SetColor(Color::Black, Color::White);
    SetConsoleTitle(TitleCaption);
    ClearScreen();
    printf("%d",15);
    bool isRunning = 1;
    INPUT_RECORD InputRecord;

    BackgroundSection background; background.InitSection(width-18, 0);
    ForegroundSection foreground; foreground.InitSection(width-18, 10);
    InstructSection instruct; instruct.InitSection(width-18, 21);

    while (isRunning) {

    }
    return 0;


    #endif
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
    char cmd[128];
    sprintf(cmd,"mode %d,%d",width,height);
    system(cmd);
}
bool InRect (int x1, int y1, int x2, int y2, int x, int y) {
    return (x>=x1 && x<=x2 && y>=y1 && y<=y2);
}
void Canvas::SaveToFile (int height, int width, CanvasCell** canvas, char* FileName) {
    FILE * hFile = fopen(FileName, "wb");
    fprintf(hFile, "%d %d\n",height,width);
    fwrite(canvas, sizeof(CanvasCell), height*width, hFile);
    fclose(hFile);
}
bool Canvas::LoadFromFile (int &height, int &width, CanvasCell** canvas, char* FileName) {
    FILE * hFile = fopen(FileName, "wb");
    fscanf(hFile, "%d %d\n",&height,&width);
    fread(canvas,sizeof(CanvasCell),height*width,hFile);
    fclose(hFile);
    return 1;
}
void BackgroundSection::InitSection (int topLeftX, int topLeftY) {
    x = topLeftX; y = topLeftY; currentColor = Color::Black;
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
    x = topLeftX; y = topLeftY; currentColor = Color::White;
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
