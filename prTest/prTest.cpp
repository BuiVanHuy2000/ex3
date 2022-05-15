#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <winsock2.h>
#include <stdio.h>
#include <conio.h>
#pragma comment(lib, "ws2_32")
using namespace std;

map<string, string> mp;

void inputFile() {
    string line;
    ifstream myfile("D:\\database\\output.txt");
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            int k = line.find_first_of(" ");
        ;
            mp[line.substr(0, k)] = line.substr(k + 1, line.size());
        }
        myfile.close();
    }
    else cout << "Unable to open file";
}

int IndexOf(const char* s, char c) {
    for (int i = 0; s[i] != '\0'; ++i) {
        if (s[i] == c) {
            return i;
        }
    }

    return -1;
}

char* substr(char* arr, int begin, int len)
{
    char* res = new char[len + 1];
    for (int i = 0; i < len; i++)
        res[i] = *(arr + begin + i);
    res[len] = 0;
    return res;
}

int checkInput(SOCKET client) {
    int ret;
    char buf[256];
    send(client, "Nhap tai khoan mat khau: ", 26, 0);
    ret = recv(client, buf, sizeof(buf), 0);
    buf[ret] = 0;
    printf("Du lieu nhan duoc: %s\n", buf);
    int k = IndexOf(buf, ' ');
    if (k < 0) {
        send(client, "Loi Dang Nhap\n", 15, 0);
        return 1;
    }
    int h = strcspn(buf, "\0");
    char user[32], pass[32];   
    strcpy_s(user, k + 1, substr(buf, 0, k));
    strcpy_s(pass, h - k , substr(buf, k + 1, h));
    int h1 = IndexOf(pass, '\n');
    strcpy_s(pass, h1+1, substr(pass, 0, h1));

    for (auto x : mp) {
        string a = (string)x.first;
        string b = (string)x.second;
        if (strcmp(a.c_str(), user) == 0 && strcmp(b.c_str(), pass) == 0) {
            return 0;
        }
    }
    send(client, "Loi Dang Nhap\n", 15, 0);

    return 1;
}

int main() {
    inputFile();
    /*inputFile();

    for (auto x : mp) {
        cout << x.first << ": " << x.second << endl;
    }*/
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    // Tao socket
    SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // Khai bao dia chi server
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8000);
    // Gan cau truc dia chi voi socket
    bind(listener, (SOCKADDR*)&addr, sizeof(addr));
    // Chuyen sang trang thai cho ket noi
    listen(listener, 5);

    SOCKET client = accept(listener, NULL, NULL);
    printf("Client moi ket noi: %d\n", client);

    while (1) {
        if (checkInput(client) == 0)
            break;
    }
    char a[1], url[256];
    int ret1;
    while (1) {
        send(client, "Ban co muon nhap du lieu vao file khong?\n Nhap 0 de tiep tuc; nhap khac de ket thuc!!", 83, 0);
        send(client, "\nNhap du lieu vao: ", 20, 0);
        
        recv(client, a, 1, 0);
        int k = atoi(a);

        if (k == 0)
        {
            send(client, "\nNhap duong dan: ", 18, 0);
            int ret1 = recv(client, url, sizeof(url), 0);
            url[k] = '\0';

            
            /*strcat(url, " > ");*/
            printf("%s", url);
        }
        else {
            break;
        }
    }

    
    
    return 0;
}