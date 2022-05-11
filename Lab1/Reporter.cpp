#include <iostream>
#include <fstream>

using namespace std;

struct employee {
	int num;
	char name[10];
	double hours;
};

int comparator(const void* a, const void* b) {
	employee* e1 = (employee*)a;
	employee* e2 = (employee*)b;
	return e1->num - e2->num;
}

int main(int argc, char* argv[]) {
	fstream fin(argv[1], ios::in | ios::binary | ios::ate);
	ofstream fout(argv[2]);
	fin.seekg(0, ios::end);
	int n = fin.tellg() / sizeof(employee);
	fin.seekg(0, ios::beg);
	double salary = atoi(argv[3]);
	fout << "File report" << argv[1] << endl;
	fout << "Number, Name, Hours, Salary" << endl;
	employee* e = new employee[n];
	fin.read((char*)e, sizeof(employee) * n);
	for (int i = 0; i < n; i++) {
		fout << e[i].num << " " << e[i].name << " " << e[i].hours << " " << static_cast<double>(e[i].hours * salary) << endl;
	}
	fout.close();
	fin.close();
	return 0;
}