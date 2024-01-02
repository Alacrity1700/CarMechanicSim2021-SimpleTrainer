#include "FindDMAAddy.h"
#include "GetModuleBase.h"
#include "procID.h"
using namespace std;

//template function allows to overload << operator to print a vector's content

template <typename ele_type> ostream& operator<<(ostream& os, const vector<ele_type>& vect_name) {

    // itr is iterator for each element traversal

    for (auto itr : vect_name) {

        os << itr << " ";
    }

    return os;
}

int main()
{
 
    DWORD procId = GetProcId(L"Car Mechanic Simulator 2021.exe"); 
    uintptr_t gameAssemblyBase = GetModuleBaseAddress(procId, L"GameAssembly.dll");   
    uintptr_t unityPlayerBase = GetModuleBaseAddress(procId, L"UnityPlayer.dll");
    HANDLE hProcess = 0;

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
    uintptr_t gameAssemblyPtr = gameAssemblyBase + 0x028098A8;   
    uintptr_t unityPlayerPtr = unityPlayerBase + 0x0279D8B8;


    std::vector<unsigned int> moneyOffsets = {0x98, 0x0, 0x68, 0xC8, 0xF80, 0xB8, 0x2A0}; // 152, 0, 104, 200, 3968, 184, 672
    std::vector<unsigned int> scrapOffsets = {0x40, 0x80, 0xDB0, 0xC8, 0xAC0, 0xB8, 0x2AC}; // 64, 128, 3504, 200, 2752, 184, 684
    std::vector<unsigned int> skillOffsets = {0xB8, 0x0, 0x80, 0xC0, 0x180, 0x178, 0x30  }; // got lazy ;p
    uintptr_t scrapAddr = FindDMAAddy(hProcess, gameAssemblyPtr, scrapOffsets);
    uintptr_t moneyAddr = FindDMAAddy(hProcess, gameAssemblyPtr, moneyOffsets);
    uintptr_t skillAddr = FindDMAAddy(hProcess, unityPlayerPtr, skillOffsets);

#ifdef _DEBUG
    cerr << "********DEBUG********" << endl;
    cerr << "gameassembly:    " << gameAssemblyBase << endl;
    cerr << "unityplayer:     " << unityPlayerBase << endl;
    cerr << "gameassemblyptr: " << gameAssemblyPtr << endl;
    cerr << "unityplayerptr:  " << unityPlayerPtr << endl;
    cerr << "money offsets:   " << moneyOffsets << endl;
    cerr << "scrap offsets:   " << scrapOffsets << endl;
    cerr << "skill offsets:   " << skillOffsets << endl;
    cerr << "money add:       " << moneyAddr << endl;
    cerr << "scrap add:       " << scrapAddr << endl;
    cerr << "skill add:       " << skillAddr << endl;
    cerr << "********DEBUG********\n\n" << endl;
#endif

    bool exitProgram = false;
    do {
        int scrapVal{ 0 };
        int moneyVal{ 0 };
        int skillVal{ 0 };
        ReadProcessMemory(hProcess, (BYTE*)moneyAddr, &moneyVal, sizeof(moneyVal), nullptr);
        ReadProcessMemory(hProcess, (BYTE*)scrapAddr, &scrapVal, sizeof(scrapVal), nullptr);
        cout << moneyVal << " is your current money amount." << endl;
        cout << scrapVal << " is your current scrap amount." << endl;

        int changedScrapVal{ 0 };
        int changedMoneyVal{ 0 };
        int changedSkillVal{ 0 };
        int addrToChange{ 0 };
        cout << "Would you like to change your:\n1. Scrap\n2. Money\n3. Skill Points" << endl;
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
        else if (addrToChange == 3) {
            cout << "Enter your desired skill point amount:" << endl;
            cin >> changedSkillVal;
            WriteProcessMemory(hProcess, (BYTE*)skillAddr, &changedSkillVal, sizeof(changedSkillVal), nullptr);
            cout << "Skill points Changed Successfully!" << endl;
            ReadProcessMemory(hProcess, (BYTE*)skillAddr, &skillVal, sizeof(skillVal), nullptr);

            cout << moneyVal << " is your new skill point amount." << endl;
        }
        else
            cout << "Enter a valid input.\n";

        cout << "Do you want to perform another change? (0 for Yes, 1 for No): " << endl;
        cin >> exitProgram;
        addrToChange = 0;

    } while (exitProgram == false);
    
    cout << "Trainer Closing...." << endl;    
    return 0;
}
