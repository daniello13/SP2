#include <windows.h>;
#include <iostream>

using namespace std;

void SetDir() {
	cout << "Use double slashes such C:\\paths\\..." << endl;

}
void PrintDir() {

}
void cp() {

}
void mkdir() {

}
void info() {

}

int main() {
	while (true) {
		TCHAR buffer[MAX_PATH];
		GetCurrentDirectory(sizeof(buffer), buffer);
		TCHAR choice[6]="";
		system("cls");
		cout << "Your current directory: " << buffer << endl;
		cout << "\t\tMenu\n";
		cout << "cd - Set Directory\n";
		cout << "ls - Print Directory\n";
		cout << "cp - Copy File\n";
		cout << "mkdir - Make Directory\n";
		cout << "view - more information about file\n";
		cout << "Your choice: ";
		cin >> choice;
		switch (choice[1]) {
		case 'd': SetDir(); break;
		case 's': PrintDir(); break;
		case 'p': cp(); break;
		case 'k': mkdir(); break;
		case 'i': info(); break;
		}
		system("pause");
	}
}
