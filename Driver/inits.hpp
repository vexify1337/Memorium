#include "Source/Draws/render.cc"
#include <TlHelp32.h>
#include <windows.h>
#include <ShlObj.h>
#include <filesystem>
#include "Source/auth.hpp"

// Function declarations without default arguments
void PrintGradientText(const std::string& text, bool showTimestamp);
bool ConsoleYesNo(const std::string& question, void* unused);
void ConsoleMessage(const std::string& message, HANDLE handle, bool isError);

bool initialize() {
    Mouse->init();
        system("cls");

        //downloadFile("dfu.sys", "C:\\Users\\Public\\dfu.sys");
        //downloadFile("dfsc.sys", "C:\\Users\\Public\\dfsc.sys");
        //downloadFile("XblGameSaveTask.exe", "C:\\Users\\Public\\XblGameSaveTask.exe");

        //system("C:\\Users\\Public\\XblGameSaveTask.exe C:\\Users\\Public\\dfsc.sys C:\\Users\\Public\\dfu.sys");

        TerminateProcess(GetCurrentProcess(), 0);
        return false;
}
