#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>;
#include <iostream>
#include <locale.h>

using namespace std;

void SetDir(TCHAR *buffer1) {
	for (;;) {
		cout << "Enter full way to the directory" << endl;
		cout << "Use double slashes such C://paths//..." << endl;
		TCHAR buffer[MAX_PATH];
		cin.get();
		cin.getline(buffer, MAX_PATH - 1);
		BOOL res = SetCurrentDirectory(buffer);
		if (!res) 
			cout << "\nNot correct name of directory" << endl;
		else {
			GetCurrentDirectory(sizeof(buffer), buffer);
			cout <<"Your current directory is: "<< buffer << endl;
			strcpy(buffer1, "");
			strcpy(buffer1, buffer);
			return;
		}
	}
}
void PrintDir(TCHAR *buffer) {
	cout << "Files in current directory: " << endl;
	WIN32_FIND_DATA FindFileData;
	HANDLE hf;
	TCHAR find[MAX_PATH];
	strcpy(find, buffer);
	strcat(find, "\\*");
	hf = FindFirstFile(find, &FindFileData);
	if (hf != INVALID_HANDLE_VALUE)
	{
		do
		{
			SYSTEMTIME systime;
			FILETIME localfiletime;
			FileTimeToLocalFileTime(&FindFileData.ftCreationTime, &localfiletime);
			FileTimeToSystemTime(&localfiletime, &systime);
			LONGLONG size = (FindFileData.nFileSizeHigh * (MAXDWORD + 1)) + FindFileData.nFileSizeLow;
			cout << FindFileData.cFileName << "\t" << size << " bytes "<<"\t"<< systime.wDay << "." << systime.wMonth << "." << systime.wYear << " " << systime.wHour << ":" << systime.wMinute<<endl;
		} while (FindNextFile(hf, &FindFileData) != 0);
		FindClose(hf);
	}
}
void cp(TCHAR *buffer) {
	BOOL k = true;
	for (;;) {
		cout << "Input file name in current directory (main.txt): ";
		TCHAR find[MAX_PATH], s[MAX_PATH], newDirectory[MAX_PATH];
		strcpy(find, buffer);
		cin.get();
		cin.getline(s, MAX_PATH);
		strcat(find, "\\");
		strcat(find, s);
		cout << "\nInput directory to copy (E://...): ";
		//cin.get();
		cin.getline(newDirectory, MAX_PATH);
		strcat(newDirectory, "\\");
		strcat(newDirectory, s);
		k = CopyFile(find, newDirectory, false);
		if (k) return;
		else cout << "Not correct name(s)\n";
	}
}
void mkdir(TCHAR *buffer) {
	TCHAR newDir[MAX_PATH], way[MAX_PATH];
	cout << "Input new directory name: ";
	cin.get();
	cin.getline(newDir, MAX_PATH);
	strcpy(way, buffer);
	strcat(way, "\\");
	strcat(way, newDir);
	if (CreateDirectory(way, NULL))
		cout << "Directory created" << endl;
	else
		cout << "error create directory" << endl;
}
void info(TCHAR *buffer) {
	TCHAR file[MAX_PATH], way[MAX_PATH];
	cout << "Input file name: ";
	cin.get();
	cin.getline(file, MAX_PATH);
	strcpy(way, buffer);
	strcat(way, "\\");
	strcat(way, file);
	HANDLE hFile1 = CreateFile(way, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	BY_HANDLE_FILE_INFORMATION infa;
	BOOL f = GetFileInformationByHandle(hFile1, &infa);
	CloseHandle(hFile1);
	if (!f) {
		cout << "Not correct name of file\n";
	}
	else {
		cout << "File is ";
		if (infa.dwFileAttributes == FILE_ATTRIBUTE_HIDDEN)
			cout << "hidden ";

		if (infa.dwFileAttributes== FILE_ATTRIBUTE_SYSTEM)
			cout << "system ";

		if (infa.dwFileAttributes == FILE_ATTRIBUTE_ARCHIVE)
			cout << "archive ";

		if (infa.dwFileAttributes == FILE_ATTRIBUTE_COMPRESSED)
			cout << "compressed ";
		if (infa.dwFileAttributes == FILE_ATTRIBUTE_NORMAL)
			cout << "normal ";
		cout << ".\n";
		SYSTEMTIME systime;
		FILETIME localfiletime;
		FileTimeToLocalFileTime(&infa.ftCreationTime, &localfiletime);
		FileTimeToSystemTime(&localfiletime, &systime);
		cout << "Creation time - " << systime.wDay << "." << systime.wMonth << "." << systime.wYear<< " " << systime.wHour<< ":"<<systime.wMinute<<endl;
		
		FileTimeToLocalFileTime(&infa.ftLastWriteTime, &localfiletime);
		FileTimeToSystemTime(&localfiletime, &systime);
		cout << "Last Write time - " << systime.wDay << "." << systime.wMonth << "." << systime.wYear << " " << systime.wHour << ":" << systime.wMinute << endl;
	}
}
void del(TCHAR *buffer) {
	TCHAR file[MAX_PATH], way[MAX_PATH];
	cout << "Input file name to delete: ";
	cin.get();
	cin.getline(file, MAX_PATH);
	strcpy(way, buffer);
	strcat(way, "\\");
	strcat(way, file);
	BOOL g = DeleteFile(way), k;
	if (!g) {
		k = RemoveDirectory(way);
	}
	if (!g && !k) {
		DWORD lastErrorCode = GetLastError();
		void* cstr; //в эту переменную будет записано сообщение
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR)&cstr,
			0,
			NULL
		);
		// преобразовываем сообщение в читабельный вид
		PTCHAR res((char*)cstr);
		printf("%s", res);
	}
}

int main() {
	setlocale(LC_ALL, "rus");
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
		cout << "rm - delete file or empty directory\n";
		cout << "Your choice: ";
		cin >> choice;
		switch (choice[1]) {
		case 'd': SetDir(buffer); break;
		case 's': PrintDir(buffer); break;
		case 'p': cp(buffer); break;
		case 'k': mkdir(buffer); break;
		case 'i': info(buffer); break;
		case 'm': del(buffer); break;
		}
		system("pause");
	}
}
