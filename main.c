#include <stdio.h>
#include <windows.h>

#define WINDOW_HEIGHT 400
#define WINDOW_WIDTH 400
#define BACKGROUND_COLOR 1 // 1 WHITE // 2 GREY // 3 BLACK // 9 BLUE

const char g_szClassName[] = "myWindowClass";

int getRandomNumber(int x, int y) {
    return rand() % (y - x + 1) + x;
}

void ColorPixel(HWND hwnd, int x, int y, COLORREF color)
{
    HDC hdc = GetDC(hwnd);
    SetPixel(hdc, x, y, color);
    ReleaseDC(hwnd, hdc);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    int mouseX = LOWORD(lParam);
    int mouseY = HIWORD(lParam);
    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_LBUTTONDOWN:
        {
            printf("Left Click! X:%d Y:%d\n", mouseX, mouseY);
            int a = getRandomNumber(0,255);
            int b = getRandomNumber(0,255);
            int c = 0;
            for(int i= 0; i < 100; i= i+1){
                for(int j= 0; j < 100; j= j+1){
                    c = getRandomNumber(0,255);
                    ColorPixel(hwnd, mouseX+i, mouseY+j, RGB(a, b, c));
                }
            }
        }
        break;
    case WM_RBUTTONDOWN:

            printf("Right Click! X:%d Y:%d\n", mouseX, mouseY);

            for(int i= 0; i < 100; i= i+1){
                for(int j= 0; j < 100; j= j+1){
                    ColorPixel(hwnd, mouseX+i, mouseY+j, RGB(255, 255, 255));
                }
            }
        break;
    case WM_CHAR:
        printf("%c", wParam);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+BACKGROUND_COLOR);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
        0, // WS_EX_CLIENTEDGE
        g_szClassName,
        "GAME TITLE",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Step 3: The Message Loop
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}
