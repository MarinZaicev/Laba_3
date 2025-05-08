#include <iostream>
#include <math.h>
#include <iomanip>
#define M_PI 3.14159265358979323846
using namespace std;

void table() {
	cout << "|   x   |    y    |" << endl;
	cout << "___________________" << endl;
}
void printTable(float x, float y) {
	cout << "|" << setw(6) << fixed << setprecision(1) << x << " |" 
		<< setw(8) << fixed << setprecision(2) << y << " |" << endl;
}
void printEnd() {
	cout << "-------------------" << endl;
}

int main() {
	setlocale(LC_ALL, "Russian");
	float x = -9;
	float y = 0;
	cout << "Таблица значения функции:" << endl;
	printEnd();
	table();
	for(x; x <= -5; x++) {
		y = -sqrt((x + 9) * (-x - 5)) + 2;
		printTable(x, y);
	}
	for(x; x <= -4; x++) {
		y = 2;
		printTable(x, y);
	}
	for (x; x <= 0; x++) {
		y = -0.5 * x;
		printTable(x, y);
	}
	for (x; x <= M_PI; x++){
		y = sin(x);
		printTable(x, y);
	}
	for (x; x <= 5; x++) {
		y = x - M_PI;
		printTable(x, y);
	}
	printEnd();
	return 0;
}
