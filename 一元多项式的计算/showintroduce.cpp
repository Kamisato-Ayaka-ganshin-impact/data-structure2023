#include"showintroduce.h"
#include<graphics.h>

IMAGE imgbg;
IMAGE imgbgButton;
IMAGE extraIntroduce;
void showThirdPage() {
    initgraph(1280, 720);
    loadimage(&extraIntroduce, "extraintroduce.jpg");
    putimage(0, 0, &extraIntroduce);
}
void showExtraIntroduce() {
    IMAGE buttonStart, bottonEnd;
    loadimage(&buttonStart, "button-weianxia.png");
    loadimage(&bottonEnd, "button-yianxia.png");
    BeginBatchDraw();//·ÀÖ¹ÉÁÆÁ
    int flag = 0;
    putimage(438, 8, flag ? &buttonStart : &bottonEnd);
    ExMessage msig;
    while (true)
    {
        if (peekmessage(&msig)) {
            if (msig.message == WM_LBUTTONDOWN &&
                msig.x > 438 && msig.x < 438 + 384 &&
                msig.y>8 && msig.y < 8 + 64) {
                flag = 1;
                //cout << flag;
            }
            else if (WM_LBUTTONUP && flag) {
                break;
                return;
                //showThirdPage();
            }
            EndBatchDraw();
        }
    }
}
void initIntroduce() {
    IMAGE imgbg;
    initgraph(909, 509, 1);
    loadimage(&imgbg, "shuoming.png");
    putimage(0, 0, &imgbg);
}