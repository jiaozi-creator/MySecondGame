#include "main.h"
#include "AppDelegate.h"
#include "cocos2d.h"

// 定义Windows应用程序入口
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

USING_NS_CC;

// Windows应用程序入口
int WINAPI _tWinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR    lpCmdLine,
    int       nCmdShow)#
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // 创建应用程序委托实例
    AppDelegate app;

    // 启动Cocos2d-x引擎
    return Application::getInstance()->run();
}
