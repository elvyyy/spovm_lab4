#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<iostream>
#include <conio.h>

#define MAX_QTY_OF_THREADS 100

using namespace std;

CRITICAL_SECTION criticalSection;
size_t id = 0;

DWORD WINAPI print(LPVOID LParam) {
	while (id < MAX_QTY_OF_THREADS) {
		EnterCriticalSection(&criticalSection); // вход в критическую секцию
		char buffer[512];
		sprintf(buffer, "Work thread %d", GetCurrentThreadId());
		//cout << "Work thread " << GetCurrentThreadId() << endl; // вывод сообщения
		for (size_t i = 0; i < strlen(buffer); i++)
		{
			std::cout << buffer[i];
		}
		std::cout << std::endl;
		LeaveCriticalSection(&criticalSection); //выход из секции
		WaitForSingleObject(GetCurrentThread(), 1500); // если передать сюда Handle потока,
		//то функция остановит выполнения того потока, в рамках которого она запущена, до завершения работы того потока,
		//Handle которого ей передали - но не дольше указанного временного интервала.
	}
	ExitThread(0);
}

int main() {
	InitializeCriticalSection(&criticalSection);
	char userChoice;
	HANDLE threads[MAX_QTY_OF_THREADS];
	cout << "+ - Create Thread" << endl
		<< "- - Kill Thread " << endl
		<< "q - Exit" << endl;
	while (true) {
		userChoice = _getch();
		if (userChoice == 'q') {
			DeleteCriticalSection(&criticalSection); //убераем критическую секцию
			for (size_t i = 0; i < id; i++) {
				TerminateThread(threads[id - 1], 0); // удаляем все потоки
			}
			return 1; // завершаем процесс
		}
		else if (userChoice == '+') {
			if (id < MAX_QTY_OF_THREADS) {
				threads[id] = CreateThread(nullptr, //может ли создаваемый поток быть унаследован дочерним процессом 
					0, //значение по умолчанию (1МБ)
					print, //адрес функции, которая будет выполняться потоком
					nullptr, //указатель на переменную, которая будет передана в поток
					CREATE_SUSPENDED, //флаги создания --- поток не активен
					nullptr); //указатель на переменную, куда будет сохранен индентификатор потока
				cout << "start thread  " << GetThreadId(threads[id]) << endl; // выводим сообщение о создании нового процесса
				ResumeThread(threads[id++]); // запустить созданный поток
			}
		}
		else if (userChoice == '-') {
			if (id > 0) {
				cout << "kill thread " << GetThreadId(threads[--id]) << endl; //выводим сообщение о том, что поток будет удален
				TerminateThread(threads[id], 0); // удаляем поток
			}
			else {
				cout << "No threads" << endl; //если нет потоков, выводим сообщение
			}
		}
	}
	return 0;
}