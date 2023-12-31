#include "FindDMAAddy.h"
#include "GetModuleBase.h"
#include "procID.h"
using namespace std;

int main()
{
 
    DWORD procId = GetProcId(L"Car Mechanic Simulator 2021.exe");
    cout << "Grabbing Process ID" << endl;
  
    uintptr_t moduleBase = GetModuleBaseAddress(procId, L"GameAssembly.dll");

    cout << "Grabbing Module Base" << endl;
   
    HANDLE hProcess = 0;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

    cout << "Opening Process" << endl;

    uintptr_t dynamicPtrBaseAddr = moduleBase + 0x028098A8;

    cout << "Getting Ptr Base Address" << endl;
    
    std::vector<unsigned int> moneyOffsets = {0x98, 0x0, 0x68, 0xC8, 0xF80, 0xB8, 0x2A0};
    std::vector<unsigned int> scrapOffsets = {0x40, 0x80, 0xDB0, 0xC8, 0xAC0, 0xB8, 0x2AC};
    uintptr_t scrapAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, scrapOffsets);
    uintptr_t moneyAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, moneyOffsets);
    cout << "Resolving Offsets" << endl;
    bool exitProgram = false;
    do {
        int scrapVal = 0;
        int moneyVal = 0;
        ReadProcessMemory(hProcess, (BYTE*)moneyAddr, &moneyVal, sizeof(moneyVal), nullptr);
        ReadProcessMemory(hProcess, (BYTE*)scrapAddr, &scrapVal, sizeof(scrapVal), nullptr);
        cout << moneyVal << " is your current money amount." << endl;
        cout << scrapVal << " is your current scrap amount." << endl;

        int changedScrapVal = 1000000;
        int changedMoneyVal = 1000000;
        int addrToChange = 0;
        cout << "Would you like to change your scrap(1) or money(2)? Enter a number:" << endl;
        cin >> addrToChange;
        if (addrToChange == 1) {
            cout << "Enter your desired scrap amount:" << endl;
            cin >> changedScrapVal;
            WriteProcessMemory(hProcess, (BYTE*)scrapAddr, &changedScrapVal, sizeof(changedScrapVal), nullptr);
            cout << "Scrap Changed Successfully!" << endl;
            ReadProcessMemory(hProcess, (BYTE*)scrapAddr, &scrapVal, sizeof(scrapVal), nullptr);

            cout << scrapVal << " is your new scrap amount." << endl;
        }
        else if (addrToChange == 2) {
            cout << "Enter your desired money amount:" << endl;
            cin >> changedMoneyVal;
            WriteProcessMemory(hProcess, (BYTE*)moneyAddr, &changedMoneyVal, sizeof(changedMoneyVal), nullptr);
            cout << "Money Changed Successfully!" << endl;
            ReadProcessMemory(hProcess, (BYTE*)moneyAddr, &moneyVal, sizeof(moneyVal), nullptr);

            cout << moneyVal << " is your new money amount." << endl;
        }
        else
            cout << "Enter a valid input.";

        cout << "Do you want to perform another change? (0 for Yes, 1 for No): " << endl;
        cin >> exitProgram;
        addrToChange = 0;

    } while (exitProgram == false);
    
    cout << "Trainer Closing...." << endl;


    
    
    



    return 0;
}
