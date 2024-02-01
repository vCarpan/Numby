#include <stdio.h>
#include <windows.h>

#define WINDOW_HEIGHT 400
#define WINDOW_WIDTH 400
#define BACKGROUND_COLOR 3 // 1 WHITE // 2 GREY // 3 BLACK // 9 BLUE

const char g_szClassName[] = "myWindowClass";

const int p1Color = RGB(0, 0, 255);
const int p2Color = RGB(255, 0, 0);

int p1Size = 10;
int p2Size = 10;

int playerStack = 0;
int p1posX = 100;
int p1posY = 200;

int p2posX = 300;
int p2posY = 200;

int getRandomNumber(int x, int y)
{
    return rand() % (y - x + 1) + x;
}

void ColorPixel(HWND hwnd, int x, int y, COLORREF color)
{
    HDC hdc = GetDC(hwnd);
    SetPixel(hdc, x, y, color);
    ReleaseDC(hwnd, hdc);
}

void Pattern1(HWND hwnd, int x, int y)
{
    int a = 0;
    int b = 255;
    int c = 255;
    for (int i = 0; i < 100; i = i + 1)
    {
        ColorPixel(hwnd, (x + i), y + ((i * i / 10)), RGB(a, b, c));
        ColorPixel(hwnd, (x + i), y + ((i * i / 10)) + 1, RGB(a, b, c));
        ColorPixel(hwnd, (x + i), y + ((i * i / 10)) + 2, RGB(a, b, c));
    }

    for (int i = 100; i >= 0; i = i - 1)
    {
        ColorPixel(hwnd, (x - i), y + ((i * i / 10)), RGB(a, b, c));
        ColorPixel(hwnd, (x - i), y + ((i * i / 10)) + 1, RGB(a, b, c));
        ColorPixel(hwnd, (x - i), y + ((i * i / 10)) + 2, RGB(a, b, c));
    }
}

void Pattern2(HWND hwnd, int x, int y)
{
    int a = getRandomNumber(0, 255);
    int b = getRandomNumber(0, 255);
    int c = 0;
    for (int i = 0; i < 100; i = i + 1)
    {
        for (int j = 0; j < 100; j = j + 1)
        {
            c = getRandomNumber(0, 255);
            ColorPixel(hwnd, x + i, y + j, RGB(a, b, c));
        }
    }
}

void DrawLine(HWND hwnd, int x1, int y1, int x2, int y2, COLORREF color) {
    // Bresenham's line algorithm
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (x1 != x2 || y1 != y2) {
        ColorPixel(hwnd, x1, y1, color);

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }

    // Ensure the endpoint is colored
    ColorPixel(hwnd, x2, y2, color);
}


void DrawCube(HWND hwnd, int posX, int posY, int size, COLORREF color) {
    int halfSize = size / 2;

    // Front face
    DrawLine(hwnd, posX - halfSize, posY - halfSize, posX + halfSize, posY - halfSize, color);
    DrawLine(hwnd, posX + halfSize, posY - halfSize, posX + halfSize, posY + halfSize, color);
    DrawLine(hwnd, posX + halfSize, posY + halfSize, posX - halfSize, posY + halfSize, color);
    DrawLine(hwnd, posX - halfSize, posY + halfSize, posX - halfSize, posY - halfSize, color);

    // Back face
    DrawLine(hwnd, posX - halfSize, posY - halfSize - size, posX + halfSize, posY - halfSize - size, color);
    DrawLine(hwnd, posX + halfSize, posY - halfSize - size, posX + halfSize, posY + halfSize - size, color);
    DrawLine(hwnd, posX + halfSize, posY + halfSize - size, posX - halfSize, posY + halfSize - size, color);
    DrawLine(hwnd, posX - halfSize, posY + halfSize - size, posX - halfSize, posY - halfSize - size, color);

    // Connect front and back faces
    DrawLine(hwnd, posX - halfSize, posY - halfSize, posX - halfSize, posY - halfSize - size, color);
    DrawLine(hwnd, posX + halfSize, posY - halfSize, posX + halfSize, posY - halfSize - size, color);
    DrawLine(hwnd, posX + halfSize, posY + halfSize, posX + halfSize, posY + halfSize - size, color);
    DrawLine(hwnd, posX - halfSize, posY + halfSize, posX - halfSize, posY + halfSize - size, color);
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
        Pattern2(hwnd, mouseX, mouseY);
    }
    break;
    case WM_RBUTTONDOWN:

        printf("Right Click! X:%d Y:%d\n", mouseX, mouseY);

        for (int i = 0; i < 100; i = i + 1)
        {
            for (int j = 0; j < 100; j = j + 1)
            {
                ColorPixel(hwnd, mouseX + i, mouseY + j, RGB(255, 255, 255));
            }
        }
        break;
    case WM_CHAR:
        // printf("%c", wParam);

        // Player1
        if (wParam == 'a')
        {
            for (int i = 0; i < p1Size; i++)
            {
                for (int j = 0; j < p1Size; j++)
                {
                    ColorPixel(hwnd, p1posX + i, p1posY + j, RGB(0, 0, 0));
                }
            }
            p1posX = p1posX - p1Size;
            for (int i = 0; i < p1Size; i++)
            {
                for (int j = 0; j < p1Size; j++)
                {
                    ColorPixel(hwnd, p1posX + i, p1posY + j, p1Color);
                }
            }
        }
        else if (wParam == 'd')
        {
            for (int i = 0; i < p1Size; i++)
            {
                for (int j = 0; j < p1Size; j++)
                {
                    ColorPixel(hwnd, p1posX + i, p1posY + j, RGB(0, 0, 0));
                }
            }
            p1posX = p1posX + p1Size;
            for (int i = 0; i < p1Size; i++)
            {
                for (int j = 0; j < p1Size; j++)
                {
                    ColorPixel(hwnd, p1posX + i, p1posY + j, p1Color);
                }
            }
        }
        else if (wParam == 'w')
        {
            for (int i = 0; i < p1Size; i++)
            {
                for (int j = 0; j < p1Size; j++)
                {
                    ColorPixel(hwnd, p1posX + i, p1posY + j, RGB(0, 0, 0));
                }
            }
            p1posY = p1posY - p1Size;
            for (int i = 0; i < p1Size; i++)
            {
                for (int j = 0; j < p1Size; j++)
                {
                    ColorPixel(hwnd, p1posX + i, p1posY + j, p1Color);
                }
            }
        }
        else if (wParam == 's')
        {
            for (int i = 0; i < p1Size; i++)
            {
                for (int j = 0; j < p1Size; j++)
                {
                    ColorPixel(hwnd, p1posX + i, p1posY + j, RGB(0, 0, 0));
                }
            }
            p1posY = p1posY + p1Size;
            for (int i = 0; i < p1Size; i++)
            {
                for (int j = 0; j < p1Size; j++)
                {
                    ColorPixel(hwnd, p1posX + i, p1posY + j, p1Color);
                }
            }
        }

        // Player2
        if (wParam == 'j')
        {
            for (int i = 0; i < p2Size; i++)
            {
                for (int j = 0; j < p2Size; j++)
                {
                    ColorPixel(hwnd, p2posX + i, p2posY + j, RGB(0, 0, 0));
                }
            }
            p2posX = p2posX - p2Size;
            for (int i = 0; i < p2Size; i++)
            {
                for (int j = 0; j < p2Size; j++)
                {
                    ColorPixel(hwnd, p2posX + i, p2posY + j, p2Color);
                }
            }
        }
        else if (wParam == 'l')
        {
            for (int i = 0; i < p2Size; i++)
            {
                for (int j = 0; j < p2Size; j++)
                {
                    ColorPixel(hwnd, p2posX + i, p2posY + j, RGB(0, 0, 0));
                }
            }
            p2posX = p2posX + p2Size;
            for (int i = 0; i < p2Size; i++)
            {
                for (int j = 0; j < p2Size; j++)
                {
                    ColorPixel(hwnd, p2posX + i, p2posY + j, p2Color);
                }
            }
        }
        else if (wParam == 'i')
        {
            for (int i = 0; i < p2Size; i++)
            {
                for (int j = 0; j < p2Size; j++)
                {
                    ColorPixel(hwnd, p2posX + i, p2posY + j, RGB(0, 0, 0));
                }
            }
            p2posY = p2posY - p2Size;
            for (int i = 0; i < p2Size; i++)
            {
                for (int j = 0; j < p2Size; j++)
                {
                    ColorPixel(hwnd, p2posX + i, p2posY + j, p2Color);
                }
            }
        }
        else if (wParam == 'k')
        {
            for (int i = 0; i < p2Size; i++)
            {
                for (int j = 0; j < p2Size; j++)
                {
                    ColorPixel(hwnd, p2posX + i, p2posY + j, RGB(0, 0, 0));
                }
            }
            p2posY = p2posY + p2Size;
            for (int i = 0; i < p2Size; i++)
            {
                for (int j = 0; j < p2Size; j++)
                {
                    ColorPixel(hwnd, p2posX + i, p2posY + j, p2Color);
                }
            }
        }
        else if(wParam == '0'){ //testing (not working :P)
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            int posX = 100;
            int posY = 100;
            int cubeSize = 25;
            COLORREF cubeColor = RGB(255, 0, 0); // Red
            DrawCube(hwnd, posX, posY, cubeSize, cubeColor);

            EndPaint(hwnd, &ps);
        }
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

    // Step 1: Registering the Window Class
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + BACKGROUND_COLOR);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
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

    if (hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
                   MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Step 3: The Message Loop
    while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}
