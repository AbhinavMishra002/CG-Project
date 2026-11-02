
#include <windows.h>
#include <cmath>

float angle = 0;

struct Point
{
    float x, y, z;
};

Point cube[8] = {
    {-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1}, {-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1}};

int edges[12][2] = {
    {0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6}, {6, 7}, {7, 4}, {0, 4}, {1, 5}, {2, 6}, {3, 7}};

void drawCube(HDC hdc)
{
    POINT p[8];

    for (int i = 0; i < 8; i++)
    {
        float x = cube[i].x, y = cube[i].y, z = cube[i].z;

        // Rotation around X axis
        float y1 = y * cos(angle) - z * sin(angle);
        float z1 = y * sin(angle) + z * cos(angle);

        // Rotation around Y axis
        float x2 = x * cos(angle) + z1 * sin(angle);
        float z2 = -x * sin(angle) + z1 * cos(angle);

        // 2D projection
        p[i].x = 350 + (int)(120 * x2);
        p[i].y = 280 - (int)(120 * y1);
    }

    HPEN pen = CreatePen(PS_SOLID, 3, RGB(0, 0, 200));
    HPEN old = (HPEN)SelectObject(hdc, pen);

    for (int i = 0; i < 12; i++)
    {
        MoveToEx(hdc, p[edges[i][0]].x,
                 p[edges[i][0]].y, NULL);

        LineTo(hdc, p[edges[i][1]].x,
               p[edges[i][1]].y);
    }

    SelectObject(hdc, old);
    DeleteObject(pen);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg,
                            WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_CREATE:
        SetTimer(hwnd, 1, 30, NULL);
        return 0;

    case WM_TIMER:
        angle += 0.03f;
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        RECT r;
        GetClientRect(hwnd, &r);
        FillRect(hdc, &r,
                 (HBRUSH)GetStockObject(WHITE_BRUSH));

        drawCube(hdc);

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_DESTROY:
        KillTimer(hwnd, 1);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcA(hwnd, msg, wp, lp);
}

int main()
{
    HINSTANCE h = GetModuleHandle(NULL);

    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = h;
    wc.lpszClassName = "Cube";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

    RegisterClassA(&wc);

    HWND w = CreateWindowA("Cube", "3D Rotating Cube",
                           WS_OVERLAPPEDWINDOW, 100, 100, 700, 600,
                           NULL, NULL, h, NULL);

    if (!w)
        return 1;

    ShowWindow(w, SW_SHOW);

    MSG msg;
    while (GetMessageA(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    return 0;
}
