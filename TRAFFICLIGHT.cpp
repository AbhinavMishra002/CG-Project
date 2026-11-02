
#include <windows.h>

int signal = 0;

void drawLight(HDC hdc)
{
    HBRUSH black = CreateSolidBrush(RGB(30, 30, 30));
    HBRUSH old = (HBRUSH)SelectObject(hdc, black);

    Rectangle(hdc, 250, 60, 450, 500);

    COLORREF colors[] = {
        RGB(255, 0, 0), RGB(255, 220, 0), RGB(0, 200, 0)};

    for (int i = 0; i < 3; i++)
    {
        HBRUSH b = CreateSolidBrush(
            i == signal ? colors[i] : RGB(65, 65, 65));

        SelectObject(hdc, b);
        Ellipse(hdc, 300, 90 + i * 125, 400, 190 + i * 125);
        DeleteObject(b);
    }

    SelectObject(hdc, old);
    DeleteObject(black);

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(0, 0, 0));

    const char *names[] = {"STOP", "WAIT", "GO"};
    TextOutA(hdc, 300, 530, names[signal], 4);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg,
                            WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_CREATE:
        SetTimer(hwnd, 1, 1500, NULL);
        return 0;

    case WM_TIMER:
        signal = (signal + 1) % 3;
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        drawLight(hdc);
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
    wc.lpszClassName = "TrafficLight";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

    RegisterClassA(&wc);

    HWND w = CreateWindowA("TrafficLight",
                           "Traffic Light Simulation",
                           WS_OVERLAPPEDWINDOW,
                           100, 100, 700, 650, NULL, NULL, h, NULL);

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
