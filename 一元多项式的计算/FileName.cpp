#include <graphics.h>
#include <iostream>
#include <string>
#include<Windows.h>
#include <sstream>
#include"showintroduce.h"
using namespace std;
// ����ʽ�ڵ�
struct Term {
    double coefficient;//����  ������������
    int exponent;//ָ��
    Term* next;//��һ���ָ��

    Term(double coef, int exp) : coefficient(coef), exponent(exp), next(nullptr) {}
};

// ����ʽ��
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
        ss << current->coefficient << "a^" << current->exponent;  // ��ָ��ǰ������ĸa
        current = current->next;
        if (current != nullptr) {
            ss << " + ";
        }
    }
    ss << "\r\n";  // ����
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

HWND hwndInput1; // �༭��1���
HWND hwndInput2; // �༭��2���
HWND hwndButton; // ��ť���
HWND hwndOutput; // �������

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
//�ж���������Ƿ�Ϸ�
void panduahefa(char inputText[100]) {
    for (int i = 0; inputText[i] != '\0'; i++) {
        if (!isdigit(inputText[i]) && inputText[i] != ' ' && inputText[i] != '.') {
            MessageBox(NULL, "���㣬�Ҹ�������ô����ʾ���㻹�ǲ������������ֻ���������֡�С����Ϳո�", "������ʾ", MB_OK | MB_ICONERROR);
            break;
        }
    }
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        // �����༭��1
        hwndInput1 = CreateWindow(
            "EDIT",               // ����
            "",                   // ���ڱ���
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, // ������ʽ
            50, 50, 200, 30,      // ����λ�úʹ�С
            hwnd,                // �����ھ��
            NULL,                // �˵����
            NULL,                // ʵ�����
            NULL                 // ����
        );

        // �����༭��2
        hwndInput2 = CreateWindow(
            "EDIT",               // ����
            "",                   // ���ڱ���
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, // ������ʽ
            50, 90, 200, 30,      // ����λ�úʹ�С
            hwnd,                // �����ھ��
            NULL,                // �˵����
            NULL,                // ʵ�����
            NULL                 // ����
        );

        // ������ť
        hwndButton = CreateWindow(
            "BUTTON",            // ����
            "Add",               // ��ť�ı�
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, // ��ť��ʽ
            50, 130, 80, 30,      // ��ťλ�úʹ�С
            hwnd,                // �����ھ��
            NULL,                // �˵����
            NULL,                // ʵ�����
            NULL                 // ����
        );

        // ���������
        hwndOutput = CreateWindow(
            "EDIT",               // ����
            "",                   // ���ڱ���
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL, // ������ʽ
            50, 180, 340, 100,    // ����λ�úʹ�С
            hwnd,                // �����ھ��
            NULL,                // �˵����
            NULL,                // ʵ�����
            NULL                 // ����
        );
        break;

    case WM_COMMAND:
        if (lParam == (LPARAM)hwndButton) {
            stringstream resultStream;

            // ��ȡ����Ķ���ʽ�ı�
            char inputText1[100] = "";
            char inputText2[100] = "";
            GetWindowText(hwndInput1, inputText1, sizeof(inputText1));
            GetWindowText(hwndInput2, inputText2, sizeof(inputText2));
            panduahefa(inputText1);
            panduahefa(inputText2);
            // ���ı�ת��Ϊ����ʽ
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

            // ���㲢�������ʽ
            resultStream << "Input Polynomial 1: ";
            poly1.print(resultStream);
            resultStream << "Input Polynomial 2: ";
            poly2.print(resultStream);
            resultStream << "Sum: ";
            poly1.add(poly2).print(resultStream);

            string resultText = resultStream.str();

            // ���������׷���ı�
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
    // ע�ᴰ����
    WNDCLASS wndClass = { 0 };
    wndClass.lpfnWndProc = WindowProc;
    wndClass.hInstance = GetModuleHandle(NULL);
    wndClass.lpszClassName = "MyWindowClass";
    RegisterClass(&wndClass);

    // ��������
    HWND hwnd = CreateWindow(
        "MyWindowClass",      // ����
        "Polynomial Calculator", // ���ڱ���
        WS_OVERLAPPEDWINDOW,  // ������ʽ
        CW_USEDEFAULT,        // x����
        CW_USEDEFAULT,        // y����
        500,                  // ���
        400,                  // �߶�
        NULL,                 // �����ھ��
        NULL,                 // �˵����
        GetModuleHandle(NULL), // ʵ�����
        NULL                  // ����
    );

    // ��ʾ����
    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
   
    return 0;
}
