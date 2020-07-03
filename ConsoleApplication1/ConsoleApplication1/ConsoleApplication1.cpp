// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <Windows.h>
#include <Wincrypt.h>
#include <string>
#include <sstream> 
#include <iostream>
using namespace std;
string md5(string input)
{
    HCRYPTPROV CryptProv;
    HCRYPTHASH CryptHash;
    BYTE BytesHash[33];//!
    DWORD dwHashLen;
    string final;	if (CryptAcquireContext(&CryptProv,
        NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET))
    {
        if (CryptCreateHash(CryptProv, CALG_MD5, 0, 0, &CryptHash))
        {
            if (CryptHashData(CryptHash, (BYTE*)input.c_str(), input.length(), 0))
            {
                if (CryptGetHashParam(CryptHash, HP_HASHVAL, BytesHash, &dwHashLen, 0))
                {
                    final.clear();
                    string hexcharset = "0123456789abcdef";
                    for (int j = 0; j < 16; j++)
                    {
                        final += hexcharset.substr(((BytesHash[j] >> 4) & 0xF), 1);
                        final += hexcharset.substr(((BytesHash[j]) & 0x0F), 1);
                    }
                }
            }
        }
    }	CryptDestroyHash(CryptHash);
    CryptReleaseContext(CryptProv, 0);
    return final;
}

string sNameOfFile = "pins.txt";
string sPeek;
ofstream fout(sNameOfFile,ios::app);
string makeStringTenDigit(int iTmp) {
    char* arrTmp = new char[10];
    for (int i = 0; i < 10; i++)arrTmp[i] = '0';
    int iCtd = 0;
    while (iTmp > 0) {
        arrTmp[iCtd] = char(iTmp % 10+'0');
        iTmp /= 10;
        iCtd++;
    }
    string sTmp = "0000000000";
    for (int i = 0; i < 10; i++)sTmp[i] = arrTmp[i];
    reverse(sTmp.begin(), sTmp.end());
    return sTmp;
}
bool containsOnlyDigits(const string &sTmp) {
    return sTmp.find_first_not_of("0123456789") == string::npos;
}
void writeHashToFile(string sTmp) {
    char data[32];
    for (int i = 0; i < 32; i++) {
        data[i] = sTmp[i];
    }
    fout.write((char*)&data, 32);
    fout<<" ";
}
string getNumberFromFile(string sTmpHash) {
    ifstream fin(sNameOfFile);
    string sTmp;
    int i = 0;
    if (!fin.is_open()) return "-1";
    for (fin >> sTmp; !fin.eof(); fin >> sTmp) {
#ifdef OUTD
        cout << i << " " << sTmp << "\n";
#endif // OUTD
        if (sTmpHash == sTmp) { fin.close(); return makeStringTenDigit(i); }
        ++i;
    }
    fin.close();
    sPeek = makeStringTenDigit(i-1);
    return "-1";
}
int main() {
    puts("Enter string: ");
    string sInput;
    cin >> sInput;
    if (sInput.length() != 32) {
        puts("Wrong string.");
        return 0;
    }
    string sTmp = getNumberFromFile(sInput);//if we found that hash in file contains string with number;else - "-1"
    if (sTmp != "-1") { cout << "Your number:" << sTmp; return 0; }
    for (int i = stoi(sPeek); i < pow(10, 8); ++i) {
        string sTenDigitI = makeStringTenDigit(i);
        string sTmp = md5(sTenDigitI);
        cout <<sTenDigitI <<":"<< sTmp << "\n";
        writeHashToFile(sTmp);
        if (sTmp == sInput) { cout << sTenDigitI; return 0; }
    }
    //getNumberFromFile("5523b6179c4b96efae639db2140d95b8");
    //peekWords("5523b6179c4b96efae639db2140d95b8");
    //getWords();
    
    }