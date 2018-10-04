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
void ClearScreen ();
void DrawPixel (int x, int y, char c = ' ');
void AdjustScreenSize (int width, int height);

//#define TESTMODE
namespace Canvas {
    void SaveToFile (int height, int width, CanvasCell** canvas, char* FileName);
    bool LoadFromFile (int &height, int &width, CanvasCell** canvas, char* FileName);
}

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
    int height = 30, width = 30;
    AdjustScreenSize(width, height);
    SetConsoleTitle(TitleCaption);
    ClearScreen();
    bool isRunning = 1;
    INPUT_RECORD InputRecord;

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
void ClearScreen () {
    FillConsoleOutputCharacter(hout, ' ', 9999, {0, 0}, NULL);
}
void DrawPixel (int x, int y, char c) {
    SetConsoleCursorPosition(hout, {x,y});
    printf("%c",c);
}
void AdjustScreenSize (int width, int height) {
    char cmd[128];
    sprintf(cmd,"mode %d,%d",width,height);
    system(cmd);
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
