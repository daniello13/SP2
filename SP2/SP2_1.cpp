#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <vector>
using namespace std;
const char RECB[]= "records.db";

struct rec {
	UINT num;
	FILETIME tyme;
	TCHAR str[81];
	UINT changes;
};
struct header {
	UINT not_Null;
	UINT SizeFile;
};
/*
	Считывает структуру header из файла
*/
header ReadCounts(){
	HANDLE hFile1 = CreateFile(RECB, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	SetFilePointer(hFile1, NULL, NULL, FILE_BEGIN);
	header h;
	DWORD jk;
	ReadFile(hFile1, &h, sizeof(header), &jk, NULL);
	if (jk == 0) {
		h.not_Null=0;
		h.SizeFile = 0;
	}
	CloseHandle(hFile1);
	return h;
}
/*
	Считывает все записи, пропуская структуру header
*/
vector<rec> ReadRecs() {
	HANDLE hFile1 = CreateFile(RECB, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	SetFilePointer(hFile1, sizeof(header), NULL,FILE_BEGIN);
	vector<rec> datas(0);
	
	rec i;
	DWORD jk = 0;
	for (;;) {
		bool k= ReadFile(hFile1, &i, sizeof(rec), &jk, NULL);
		if (jk == 0) break;
		datas.push_back(i);
	}
	CloseHandle(hFile1);
	
	return datas;
}
void PrintAllInformation() {
	SYSTEMTIME cm;
	vector <rec> datas = ReadRecs();
	header count = ReadCounts();
	cout << "Size of file: " << count.SizeFile << " bytes; Not null records: " << count.not_Null << endl;
	for (int i = 0; i < datas.size(); i++) {
		FileTimeToSystemTime(&datas[i].tyme, &cm);
		cout << datas[i].num << "; ";
		cout << cm.wDay << "." << cm.wMonth << "." << cm.wYear << " " << cm.wHour << ":" << cm.wMinute << "; ";;
		cout << datas[i].str << "; ";
		cout << datas[i].changes << "; \n";
	}
}
/*
	Функция для добавления записи в файл
*/
void ADD_rec() {
	header count = ReadCounts();
	vector <rec> data = ReadRecs();
	SYSTEMTIME k,s;
	GetLocalTime(&k);
	
	FILETIME ft;
	rec f;
	//переводим полученное время (системное) в файловое (по условию задания)
	SystemTimeToFileTime(&k, &ft);
	FileTimeToSystemTime(&ft, &s);
	cout << "Input string in 80 or less symbols, do not use space or input * to create null record : ";
	cin >> f.str;
	if (strcmp(f.str, "*") == 0) {
		f.num = data.size();
		f.tyme = ft;
		strcpy(f.str , "");
		f.changes = 0;
	}
	else {
		f.num = data.size();
		f.tyme = ft;
		f.changes = 0;
		count.not_Null++;
	}
	data.push_back(f);
	count.SizeFile = sizeof(count)+sizeof(rec)*data.size();
	HANDLE hFile1 = CreateFile(RECB, GENERIC_WRITE, NULL, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	LPDWORD musor=NULL;
	//добавление записи в файл
	SetFilePointer(hFile1, NULL, NULL, FILE_BEGIN);
	WriteFile(hFile1, &count, sizeof(header), musor, NULL);
	SetFilePointer(hFile1, NULL, NULL, FILE_END);
	//WriteFile(hFile1, &f, sizeof(rec), musor, NULL);
	for (int i = 0; i < data.size(); i++) {
		WriteFile(hFile1, &data[i], sizeof(rec), musor, NULL);
	}
	CloseHandle(hFile1);
}
void change_rec() {
	SYSTEMTIME cm;
	header count = ReadCounts();
	vector <rec> data = ReadRecs();
	UINT n;
	cout << "Enter num of record you want to change: "; cin >> n;
	cout << "\nIt's your record\n";
	FileTimeToSystemTime(&data[n].tyme, &cm);
	cout << data[n].num << "; ";
	cout << cm.wDay << "." << cm.wMonth << "." << cm.wYear << " " << cm.wHour << ":" << cm.wMinute << "; ";;
	cout << data[n].str << "; ";
	cout << data[n].changes << "; \n";
	cout << "\nEnter new string(don`t use space, less 80 symbols)";
	cout << "\nFor empty string enter '*'): ";
	TCHAR str[81]; 
	//data[n].str
	strcpy(str, data[n].str);
	cin >> data[n].str;
	if (strcmp(data[n].str, "*") == 0) {
		strcpy(data[n].str, "");
	}
	if (strcmp(str, "") == 0 && strcmp(data[n].str, "") != 0) {
		count.not_Null++;
	}
	if (strcmp(str, "") != 0 && strcmp(data[n].str, "") == 0) {
		count.not_Null--;
	}
	data[n].changes++;
	HANDLE hFile1 = CreateFile(RECB, GENERIC_WRITE, NULL, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	LPDWORD musor = NULL;
	//добавление записи в файл
	SetFilePointer(hFile1, NULL, NULL, FILE_BEGIN);
	WriteFile(hFile1, &count, sizeof(header), musor, NULL);
	SetFilePointer(hFile1, NULL, NULL, FILE_END);
	//WriteFile(hFile1, &f, sizeof(rec), musor, NULL);
	for (int i = 0; i < data.size(); i++) {
		WriteFile(hFile1, &data[i], sizeof(rec), musor, NULL);
	}
	CloseHandle(hFile1);

}
void delete_rec() {
	header count = ReadCounts();
	vector <rec> data = ReadRecs();
	int i;
	cout << "Enter num record to delete: ";
	cin >> i;
	if (strcmp(data[i].str, "") != 0)
		count.not_Null--;
	
	data.erase(data.begin() + i);
	vector<rec>(data).swap(data);
	for (int k = 0; k < data.size(); k++) {
		data[k].num = k;
	}
	count.SizeFile = sizeof(count) + sizeof(rec)*data.size();
	HANDLE hFile1 = CreateFile(RECB, GENERIC_WRITE, NULL, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	LPDWORD musor = NULL;
	//добавление записи в файл
	SetFilePointer(hFile1, NULL, NULL, FILE_BEGIN);
	WriteFile(hFile1, &count, sizeof(header), musor, NULL);
	SetFilePointer(hFile1, NULL, NULL, FILE_END);
	//WriteFile(hFile1, &f, sizeof(rec), musor, NULL);
	for (int i = 0; i < data.size(); i++) {
		WriteFile(hFile1, &data[i], sizeof(rec), musor, NULL);
	}
	CloseHandle(hFile1);
}
int main() {
	int key;
	for (;;) {
		cout << "Programm for your document with records" << endl;
		cout << "Enter 1 to change record" << endl;
		cout << "Enter 2 to delete record" << endl;
		cout << "Enter 3 to add record" << endl;
		cout << "Enter 4 to read all records" << endl;
		cout << "Enter 0 to exit" << endl;
		cout << "Your choice: ";
		cin >> key;
		switch (key) {
		case 0: exit(EXIT_SUCCESS); break;
		case 1:  change_rec(); break;
		case 2: delete_rec(); break;
		case 3:	 ADD_rec(); break;
		case 4:	PrintAllInformation(); break;
		default: cout << "Change your choice" << endl;
			Sleep(100);
			system("cls");
		}
		system("pause");
		system("cls");
	}
	//
	//header h = {1, 0};
	//HANDLE hFile1 = CreateFile(RECB, GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//LPDWORD musor = NULL;
	////добавление записи в файл
	//SYSTEMTIME k;
	//GetLocalTime(&k);
	//FILETIME ft;
	//rec f;
	////переводим полученное время (системное) в файловое (по условию задания)
	//SystemTimeToFileTime(&k, &ft);
	//cout << "Input string in 80 or less symbols, do not use space or input * to create null record : ";
	//cin >> f.str;
	//if (strcmp(f.str, "*") == 0) {
	//	f.num = 0;
	//	f.tyme = ft;
	//	strcpy(f.str, "");
	//	f.changes = 0;
	//}
	//SetFilePointer(hFile1, NULL, NULL, FILE_BEGIN);
	//WriteFile(hFile1, &h, sizeof(header), musor, NULL);
	//SetFilePointer(hFile1, NULL, NULL, FILE_END);
	//WriteFile(hFile1, &f, sizeof(rec), musor, NULL);
	//CloseHandle(hFile1);
	//ReadAll();
}