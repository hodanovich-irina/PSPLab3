#include "stdafx.h"
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>
using namespace std;


#pragma warning(disable: 4996)

SOCKET Connections[100];
double* slau(double a[2][2], double* y, int n);
int Counter = 0;
double* res;
double matr[2][2];
double vec[2];
string s;
void ClientHandler(int index) {
	char msg[256];
	while(true) {
		recv(Connections[index], msg, sizeof(msg), NULL);
		char* buf = strtok(msg, " .,!?\n");
		
		int col = 0;
		while (col < 12)
		{
			if (col < 8) {
				for (int i = 0; i < 2; i++)
				{
					for (int j = 0; j < 2; j++) {

						matr[i][j] = atof(&buf[col]);

						col += 2;

					}
				}
			}
			else 
			{
				for (int z = 0; z < 2; z++) {

					vec[z] = atof(&buf[col]);
					col += 2;
				}
			}
		}
		double* result = slau(matr, vec, 2);

		s = to_string(result[0]);
		char ress[255];
		strcpy(ress, s.c_str());
		
		for(int i = 0; i < Counter; i++) {
			
			if(i == index) {
				
				send(Connections[i], ress, sizeof(ress), NULL);
			}
			continue;
			//send(Connections[i], msg, sizeof(msg), NULL);
		}
	}
}

int main(int argc, char* argv[]) {
	//WSAStartup
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if(WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	SOCKET newConnection;
	for(int i = 0; i < 100; i++) {
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

		if(newConnection == 0) {
			std::cout << "Error #2\n";
		} else {
			std::cout << "Client Connected!\n";
			char msg[256] = "Hello. It`s my first network program!";
			send(newConnection, msg, sizeof(msg), NULL);

			Connections[i] = newConnection;
			Counter++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);
		}
	}


	system("pause");
	return 0;
}


double* slau(double a[2][2], double* y, int n)
{
	double* x, max;
	int k, index;
	const double eps = 0.00001;  // точность
	x = new double[n];
	k = 0;
	while (k < n)
	{
		// Поиск строки с максимальным a[i][k]
		max = abs(a[k][k]);
		index = k;
		for (int i = k + 1; i < n; i++)
		{
			if (abs(a[i][k]) > max)
			{
				max = abs(a[i][k]);
				index = i;
			}
		}
		// Перестановка строк
		if (max < eps)
		{
			// нет ненулевых диагональных элементов
			 std:: cout<< "Решение получить невозможно из-за нулевого столбца";
			 return 0;
		}
		for (int j = 0; j < n; j++)
		{
			double temp = a[k][j];
			a[k][j] = a[index][j];
			a[index][j] = temp;
		}
		double temp = y[k];
		y[k] = y[index];
		y[index] = temp;
		// Нормализация уравнений
		for (int i = k; i < n; i++)
		{
			double temp = a[i][k];
			if (abs(temp) < eps) continue; // для нулевого коэффициента пропустить
			for (int j = 0; j < n; j++)
				a[i][j] = a[i][j] / temp;
			y[i] = y[i] / temp;
			if (i == k)  continue; // уравнение не вычитать само из себя
			for (int j = 0; j < n; j++)
				a[i][j] = a[i][j] - a[k][j];
			y[i] = y[i] - y[k];
		}
		k++;
	}
	// обратная подстановка
	for (k = n - 1; k >= 0; k--)
	{
		x[k] = y[k];
		for (int i = 0; i < k; i++)
			y[i] = y[i] - a[i][k] * x[k];
	}
	//for (int g = 0; g < sizeof(x); g++)
		//res[g] = x[g];
	return x;
}
