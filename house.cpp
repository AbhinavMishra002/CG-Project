
#include <windows.h>

void polygon(HDC hdc, POINT p[], int n, COLORREF color)
{
    HBRUSH b = CreateSolidBrush(color);
    HBRUSH old = (HBRUSH)SelectObject(hdc, b);

    Polygon(hdc, p, n);

    SelectObject(hdc, old);
    DeleteObject(b);
}

void drawHouse(HDC hdc)
{
    // Sky
    RECT r = {0, 0, 700, 600};
    FillRect(hdc, &r, (HBRUSH)GetStockObject(WHITE_BRUSH));

    // Sun
    HBRUSH sun = CreateSolidBrush(RGB(255, 220, 0));
    SelectObject(hdc, sun);
    Ellipse(hdc, 550, 40, 630, 120);
    DeleteObject(sun);

    // Ground
    RECT ground = {0, 420, 700, 600};
    HBRUSH grass = CreateSolidBrush(RGB(50, 180, 70));
    FillRect(hdc, &ground, grass);
    DeleteObject(grass);

    // House body
    POINT body[] = {
        {200, 250}, {500, 250}, {500, 480}, {200, 480}};
    polygon(hdc, body, 4, RGB(255, 210, 150));

    // Roof
    POINT roof[] = {
        {170, 260}, {350, 100}, {530, 260}};
    polygon(hdc, roof, 3, RGB(180, 40, 40));

    // Door
    RECT door = {320, 350, 390, 480};
    HBRUSH brown = CreateSolidBrush(RGB(120, 65, 25));
    FillRect(hdc, &door, brown);
    DeleteObject(brown);

    // Windows
    HBRUSH blue = CreateSolidBrush(RGB(80, 180, 255));
    HBRUSH old = (HBRUSH)SelectObject(hdc, blue);

    Rectangle(hdc, 225, 290, 295, 350);
    Rectangle(hdc, 405, 290, 475, 350);

    SelectObject(hdc, old);
    DeleteObject(blue);

    // Window divisions
    MoveToEx(hdc, 260, 290, NULL);
    LineTo(hdc, 260, 350);
    MoveToEx(hdc, 225, 320, NULL);
    LineTo(hdc, 295, 320);

    MoveToEx(hdc, 440, 290, NULL);
    LineTo(hdc, 440, 350);
    MoveToEx(hdc, 405, 320, NULL);
    LineTo(hdc, 475, 320);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg,
                            WPARAM wp, LPARAM lp)
{
    if (msg == WM_PAINT)
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        drawHouse(hdc);
        EndPaint(hwnd, &ps);
        return 0;
    }

    if (msg == WM_DESTROY)
    {
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
    wc.lpszClassName = "House";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

    RegisterClassA(&wc);

    HWND w = CreateWindowA("House", "2D House Drawing",
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
