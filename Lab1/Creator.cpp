#include <iostream>
#include <fstream>

using namespace std;

struct employee {
	int num;
	char name[10];
	double hours;
};

int main(int argc, char* argv[]) {
	int n = atoi(argv[1]);
	fstream fout(argv[2], ios::out | ios::binary);
	employee* e = new employee[n];
	for (int i = 0; i < n; i++) {
		cout << "Enter number: ";
		cin >> e[i].num;
		cout << "Enter name: ";
		cin >> e[i].name;
		cout << "Enter  hours: ";
		cin >> e[i].hours;
	}
	fout.write((const char*)e, static_cast<std::streamsize>(sizeof(employee)) * n);
	fout.close();
	return 0;
}