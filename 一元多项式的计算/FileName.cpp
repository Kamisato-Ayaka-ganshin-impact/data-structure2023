#include <graphics.h>
#include <iostream>
#include <string>
#include<Windows.h>
#include <sstream>
#include"showintroduce.h"
using namespace std;
// 多项式节点
struct Term {
    double coefficient;//底数  好像叫这个名字
    int exponent;//指数
    Term* next;//下一项的指针

    Term(double coef, int exp) : coefficient(coef), exponent(exp), next(nullptr) {}
};

// 多项式类
class Polynomial {
private:
    Term* head;

public:
    Polynomial() : head(nullptr) {}

    void addTerm(double coef, int exp) {
        Term* newNode = new Term(coef, exp);
        if (head == nullptr || exp > head->exponent) {
            newNode->next = head;
            head = newNode;
        }
        else {
            Term* current = head;
            while (current->next != nullptr && exp < current->next->exponent) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
    }

    void print(stringstream& ss) {
    Term* current = head;
    while (current != nullptr) {
        ss << current->coefficient << "a^" << current->exponent;  // 在指数前加上字母a
        current = current->next;
        if (current != nullptr) {
            ss << " + ";
        }
    }
    ss << "\r\n";  // 换行
}


    Polynomial add(const Polynomial& other) {
        Polynomial result;
        Term* current1 = head;
        Term* current2 = other.head;

        while (current1 != nullptr || current2 != nullptr) {
            double coef1 = (current1 != nullptr) ? current1->coefficient : 0;
            double coef2 = (current2 != nullptr) ? current2->coefficient : 0;
            int exp1 = (current1 != nullptr) ? current1->exponent : INT_MIN;
            int exp2 = (current2 != nullptr) ? current2->exponent : INT_MIN;

            if (exp1 > exp2) {
                result.addTerm(coef1, exp1);
                current1 = current1->next;
            }
            else if (exp1 < exp2) {
                result.addTerm(coef2, exp2);
                current2 = current2->next;
            }
            else {
                result.addTerm(coef1 + coef2, exp1);
                current1 = current1->next;
                current2 = current2->next;
            }
        }

        return result;
    }
};

HWND hwndInput1; // 编辑框1句柄
HWND hwndInput2; // 编辑框2句柄
HWND hwndButton; // 按钮句柄
HWND hwndOutput; // 输出框句柄

void drawTextBox(int x, int y, int width, int height, const char* text) {
    setfillcolor(LIGHTGRAY);
    setlinecolor(BLACK);
    fillrectangle(x, y, x + width, y + height);
    rectangle(x, y, x + width, y + height);

    settextcolor(BLACK);
    outtextxy(x + 5, y + 5, text);
}

void drawButton(int x, int y, int width, int height, const char* text) {
    setfillcolor(LIGHTGRAY);
    setlinecolor(BLACK);
    fillrectangle(x, y, x + width, y + height);
    rectangle(x, y, x + width, y + height);

    settextcolor(BLACK);
    int textWidth = textwidth(text);
    int textHeight = textheight(text);
    outtextxy(x + (width - textWidth) / 2, y + (height - textHeight) / 2, text);
}

void drawOutputBox(int x, int y, int width, int height, const char* text) {
    setfillcolor(LIGHTGRAY);
    setlinecolor(BLACK);
    fillrectangle(x, y, x + width, y + height);
    rectangle(x, y, x + width, y + height);

    settextcolor(BLACK);
    outtextxy(x + 5, y + 5, text);
}
//判断你的输入是否合法
void panduahefa(char inputText[100]) {
    for (int i = 0; inputText[i] != '\0'; i++) {
        if (!isdigit(inputText[i]) && inputText[i] != ' ' && inputText[i] != '.') {
            MessageBox(NULL, "杂鱼，我给了你那么多提示，你还是不会用吗，输入框只能输入数字、小数点和空格啊", "错误提示", MB_OK | MB_ICONERROR);
            break;
        }
    }
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        // 创建编辑框1
        hwndInput1 = CreateWindow(
            "EDIT",               // 类名
            "",                   // 窗口标题
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, // 窗口样式
            50, 50, 200, 30,      // 窗口位置和大小
            hwnd,                // 父窗口句柄
            NULL,                // 菜单句柄
            NULL,                // 实例句柄
            NULL                 // 参数
        );

        // 创建编辑框2
        hwndInput2 = CreateWindow(
            "EDIT",               // 类名
            "",                   // 窗口标题
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, // 窗口样式
            50, 90, 200, 30,      // 窗口位置和大小
            hwnd,                // 父窗口句柄
            NULL,                // 菜单句柄
            NULL,                // 实例句柄
            NULL                 // 参数
        );

        // 创建按钮
        hwndButton = CreateWindow(
            "BUTTON",            // 类名
            "Add",               // 按钮文本
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, // 按钮样式
            50, 130, 80, 30,      // 按钮位置和大小
            hwnd,                // 父窗口句柄
            NULL,                // 菜单句柄
            NULL,                // 实例句柄
            NULL                 // 参数
        );

        // 创建输出框
        hwndOutput = CreateWindow(
            "EDIT",               // 类名
            "",                   // 窗口标题
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL, // 窗口样式
            50, 180, 340, 100,    // 窗口位置和大小
            hwnd,                // 父窗口句柄
            NULL,                // 菜单句柄
            NULL,                // 实例句柄
            NULL                 // 参数
        );
        break;

    case WM_COMMAND:
        if (lParam == (LPARAM)hwndButton) {
            stringstream resultStream;

            // 获取输入的多项式文本
            char inputText1[100] = "";
            char inputText2[100] = "";
            GetWindowText(hwndInput1, inputText1, sizeof(inputText1));
            GetWindowText(hwndInput2, inputText2, sizeof(inputText2));
            panduahefa(inputText1);
            panduahefa(inputText2);
            // 将文本转换为多项式
            stringstream ss1(inputText1);
            stringstream ss2(inputText2);
            Polynomial poly1, poly2;
            double coef;
            int exp;

            while (ss1 >> coef >> exp) {
                poly1.addTerm(coef, exp);
            }

            while (ss2 >> coef >> exp) {
                poly2.addTerm(coef, exp);
            }

            // 计算并输出多项式
            resultStream << "Input Polynomial 1: ";
            poly1.print(resultStream);
            resultStream << "Input Polynomial 2: ";
            poly2.print(resultStream);
            resultStream << "Sum: ";
            poly1.add(poly2).print(resultStream);

            string resultText = resultStream.str();

            // 在输出框中追加文本
            SetWindowText(hwndOutput, resultText.c_str());
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

int main() {
    initIntroduce();
    showExtraIntroduce();
    showThirdPage();
    // 注册窗口类
    WNDCLASS wndClass = { 0 };
    wndClass.lpfnWndProc = WindowProc;
    wndClass.hInstance = GetModuleHandle(NULL);
    wndClass.lpszClassName = "MyWindowClass";
    RegisterClass(&wndClass);

    // 创建窗口
    HWND hwnd = CreateWindow(
        "MyWindowClass",      // 类名
        "Polynomial Calculator", // 窗口标题
        WS_OVERLAPPEDWINDOW,  // 窗口样式
        CW_USEDEFAULT,        // x坐标
        CW_USEDEFAULT,        // y坐标
        500,                  // 宽度
        400,                  // 高度
        NULL,                 // 父窗口句柄
        NULL,                 // 菜单句柄
        GetModuleHandle(NULL), // 实例句柄
        NULL                  // 参数
    );

    // 显示窗口
    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
   
    return 0;
}
