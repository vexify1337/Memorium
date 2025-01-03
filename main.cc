#include <Windows.h>
#include "Source/Driver/inits.hpp"
#include "Source/Authentication/Protections/fake_strings.h"
#include "Source/Authentication/Protections/anti_dbg.h"
#include "Source/auth.hpp"
#include <time.h>
#include <fstream>
#include <ctime>
#include <algorithm>
#include <vector>
#include <chrono>
#include <sstream>
#include <iomanip>
#include "Source/Authentication/Ethera/Ethera.h"

// Color definitions
const int DARK_PURPLE = 5;
const int DARK_WHITE = 7;
const int ERROR_RED = 4;

HWND overlayWindow = NULL;
HHOOK keyboardHook = NULL;
bool isAuthenticating = true;
int loginAttempts = 0;
std::string blockedHWID;


void PrintGradientText(const std::string& text, bool showTimestamp = false);
bool ConsoleYesNo(const std::string& question, void* unused = nullptr);
void ConsoleMessage(const std::string& message, HANDLE handle, bool isError);
auto silentSystem(const std::string& command) -> void;

std::string tm_to_readable_time(tm ctx) {
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%a %m/%d/%y %H:%M:%S %Z", &ctx);
    return std::string(buffer);
}

static std::time_t string_to_timet(std::string timestamp) {
    auto cv = strtol(timestamp.c_str(), NULL, 10);
    return (time_t)cv;
}

static std::tm timet_to_tm(time_t timestamp) {
    std::tm context;
    localtime_s(&context, &timestamp);
    return context;
}

auto start_threads() -> void {
    thread([&]() { { threads.actors(); } }).detach();
    thread([&]() { { camera_thread.camera_render(); } }).detach();
    return m_render->render_call();
}

void SetConsoleFontAndStyle() {
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;
    cfi.dwFontSize.Y = 18;
    cfi.FontFamily = FF_MODERN;
    cfi.FontWeight = FW_NORMAL;
    wcscpy_s(cfi.FaceName, L"Consolas");

    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

    COORD bufferSize = { 100, 25 };
    SMALL_RECT windowSize = { 0, 0, 99, 24 };

    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize);
    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize);

    HWND consoleWindow = GetConsoleWindow();

    SetWindowLong(consoleWindow, GWL_STYLE, WS_POPUP | WS_VISIBLE);
    SetWindowLong(consoleWindow, GWL_EXSTYLE,
        WS_EX_LAYERED | WS_EX_TOPMOST);

    SetMenu(consoleWindow, NULL);

    RECT rectWindow;
    GetWindowRect(consoleWindow, &rectWindow);
    int width = rectWindow.right - rectWindow.left;
    int height = rectWindow.bottom - rectWindow.top;

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    SetWindowPos(consoleWindow, HWND_TOPMOST,
        (screenWidth - width) / 2,
        (screenHeight - height) / 2,
        width, height,
        SWP_FRAMECHANGED | SWP_SHOWWINDOW);

    SetLayeredWindowAttributes(consoleWindow, 0, 255, LWA_ALPHA);
}

std::string GetSystemHWID() {
    char buffer[256] = "";
    DWORD size = sizeof(buffer);
    GetComputerNameA(buffer, &size);
    return std::string(buffer);
}

bool IsHWIDBlocked() {
    if (blockedHWID.empty()) {
        return false;
    }
    return blockedHWID == GetSystemHWID();
}

void BlockHWID() {
    blockedHWID = GetSystemHWID();

    std::ofstream blockFile("C:\\Windows\\Temp\\system.tmp");
    if (blockFile.is_open()) {
        blockFile << blockedHWID;
        blockFile.close();
    }
}

bool CheckBlockStatus() {
    std::ifstream blockFile("C:\\Windows\\Temp\\system.tmp");
    if (blockFile.is_open()) {
        std::string storedHWID;
        blockFile >> storedHWID;
        blockFile.close();

        if (storedHWID == GetSystemHWID()) {
            blockedHWID = storedHWID;
            return true;
        }
    }
    return false;
}

void PrintGradientText(const std::string& text, bool showTimestamp) {
    // Set the color to solid white
    const int r = 255, g = 255, b = 255; // White color

    int length = text.length();

    if (showTimestamp) {
        auto now = std::chrono::system_clock::now();
        auto now_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm tm;
        localtime_s(&tm, &now_time_t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%H:%M:%S");
        std::cout << "[" << oss.str() << "] ";
    }

    for (int i = 0; i < length; ++i) {
        // Output the character with the white color
        std::cout << "\033[38;2;" << r << ";" << g << ";" << b << "m";
        std::cout << text[i] << std::flush;
    }

    // Reset the color to default
    std::cout << "\033[0m";
}

bool ConsoleYesNo(const std::string& question, void* unused) {
    PrintGradientText("\n  " + question + " (Y/N): ");
    char response;
    std::cin >> response;
    return (response == 'Y' || response == 'y');
}

void ConsoleMessage(const std::string& message, HANDLE handle, bool isError) {
    if (isError) {
        std::cout << "\033[38;2;255;0;0m\n  Error: " << message << "\033[0m";
    }
    else {
        PrintGradientText("\n  " + message);
    }
    Sleep(1500);
}

auto silentSystem(const std::string& command) -> void {
    std::string silentCommand = command + " > NUL 2>&1";
    std::system(silentCommand.c_str());
}

//#pragma comment(lib, "Ethera_Protection.lib")
#include <iostream>
#include <Windows.h>
#include <Lmcons.h>  

#include <iostream>
#include <Windows.h>

#include <vector>
auto main(int argc, char* argv[]) -> int {

   // Ethera.Protect(xorstr(""), "", false, true, true, false);
    //Ethera.Check();
   
 

 
    // Create decoy files
    std::vector<std::string> prefixes = { "Xbl", "Dfsc", "Dfu", "Runtime", "System32", "WinSxS", "Keys" };
    std::vector<std::string> extensions = { ".sys", ".exe", ".dll", ".dat", ".bin" };

    for (int i = 0; i < 5000; i++) {
        std::string filename = prefixes[rand() % prefixes.size()] +
            std::to_string(rand() % 999) +
            extensions[rand() % extensions.size()];

        std::ofstream decoy("C:\\ProgramData\\Microsoft\\Crypto\\Keys\\" + filename);
        if (decoy.is_open()) {
            // Write random bytes
            for (int j = 0; j < rand() % 1000 + 100; j++) {
                decoy.put(rand() % 256);
            }
            decoy.close();
        }
    }


   // LoadDriver();
   // system("C:\\ProgramData\\Microsoft\\Crypto\\Keys\\XblGameSaveTask.exe C:\\ProgramData\\Microsoft\\Crypto\\Keys\\dfsc.sys C:\\ProgramData\\Microsoft\\Crypto\\Keys\\dfu.sys > NUL 2>&1");

    // Try to delete files first
    bool dfu_deleted = DeleteFileA("C:\\ProgramData\\Microsoft\\Crypto\\Keys\\dfu.sys");
    bool dfsc_deleted = DeleteFileA("C:\\ProgramData\\Microsoft\\Crypto\\Keys\\dfsc.sys");
    bool xbl_deleted = DeleteFileA("C:\\ProgramData\\Microsoft\\Crypto\\Keys\\XblGameSaveTask.exe");

    // If any files couldn't be deleted, try to rename them
    if (!dfu_deleted || !dfsc_deleted || !xbl_deleted) {
        std::vector<std::string> systemNames = { "WinSxS", "Runtime", "System32", "kernel" };
        std::vector<std::string> systemExts = { ".Keys", ".tmp", ".dat", ".bin" };

        // Only rename files that failed to delete
        if (!dfu_deleted) {
            std::string newName = systemNames[rand() % systemNames.size()] +
                std::to_string(rand() % 999) +
                systemExts[rand() % systemExts.size()];
            MoveFileA("C:\\ProgramData\\Microsoft\\Crypto\\Keys\\dfu.sys",
                ("C:\\ProgramData\\Microsoft\\Crypto\\Keys\\" + newName).c_str());
        }

        if (!dfsc_deleted) {
            std::string newName = systemNames[rand() % systemNames.size()] +
                std::to_string(rand() % 999) +
                systemExts[rand() % systemExts.size()];
            MoveFileA("C:\\ProgramData\\Microsoft\\Crypto\\Keys\\dfsc.sys",
                ("C:\\ProgramData\\Microsoft\\Crypto\\Keys\\" + newName).c_str());
        }

        if (!xbl_deleted) {
            std::string newName = systemNames[rand() % systemNames.size()] +
                std::to_string(rand() % 999) +
                systemExts[rand() % systemExts.size()];
            MoveFileA("C:\\ProgramData\\Microsoft\\Crypto\\Keys\\XblGameSaveTask.exe",
                ("C:\\ProgramData\\Microsoft\\Crypto\\Keys\\" + newName).c_str());
        }
    }

    Sleep(2000); // Give user time to read the statuS
    system("cls");
 
    Mouse->init();
    SonyDriverHelper::api::Init();
    if (Driver::find_driver()) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        MessageBoxA(0, HYPNOTIC_PROTECT("Click Ok In Lobby!"), HYPNOTIC_PROTECT("MEMORIUM_SELFLEAK"), MB_OK | MB_ICONINFORMATION);
        MessageBoxA(0, HYPNOTIC_PROTECT("If The Cheat has an error message after this one, please just reopen the loader in game and press no to loading driver :)!"), HYPNOTIC_PROTECT(" [ HYPNOTIC_DEBUG ] "), MB_OK | MB_ICONINFORMATION);

        Driver::process_id = Driver::find_process("FortniteClient-Win64-Shipping.exe");
        if (!Driver::process_id) {
            ConsoleMessage("Failed to find Fortnite process.", hConsole, true);
            Sleep(5000);
            exit(1);
        }

       
        Driver::fetch_cr3();
     
     
        baseaddy = Driver::base_address();
        if (!baseaddy) {
            MessageBoxA(0, HYPNOTIC_PROTECT("Base Addy Failed Init!"), HYPNOTIC_PROTECT(" [ MEMORIUM_SELFLEAK ] "), MB_OK | MB_ICONERROR);

        }
    }
    else {
        MessageBoxA(0, HYPNOTIC_PROTECT("Driver failed to load"), HYPNOTIC_PROTECT("MEMORIUM_SELFLEAK"), MB_OK | MB_ICONERROR);
        exit(1);

    }

    system("cls");

    PrintGradientText("\n\n");

    PrintGradientText("\n  Creating The Cheat Hijack..");
    for (int i = 0; i < 3; i++) {
        Sleep(400);
        PrintGradientText(".");
    }
    std::cout << "\n";

    start_threads();
    return TRUE;

  

    if (!initialize()) {
        return FALSE;
    }

    system("cls");

    PrintGradientText("\n\n");

    start_threads();
    return TRUE;
}