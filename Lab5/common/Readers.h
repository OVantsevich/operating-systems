#pragma once
#include <errno.h>
#include <functional>
#include <limits.h>
#include <string>
#include <iostream>

using std::string;
using std::getline;
using std::cout;
using std::cin;
using std::endl;

template<class T>
class TReader {

public:
	void read(T& dest, const char* message = "Введите значение: ", 
			  bool(*f)(T&) = nullptr, const char* exeptionMessage = "Введённое значение некорректно!!") {

		string str;
		T currValue;
		bool conv = false;

		cout << message;
		getline(cin, str);
		while ((conv = !convert(currValue, str)) || f != nullptr ? f(currValue) : false) {
			system("cls");
			!conv ? cout << exeptionMessage << endl : cout << "Ошибка. Проверьте входное значение!" << endl;
			cout << message;
			getline(cin, str);
		}

		dest =  currValue;
	}

	virtual bool convert(T&, const string&) = 0;
};

class IntReader : public TReader<int> {

	bool isInt(const string& str)
	{
		string strCheck = str;
		int i = 0;
		while (i < strCheck.size() && strCheck[i] == ' ') i++;
		if(i != strCheck.size())
			strCheck = strCheck.substr(i, strCheck.size() - i);
		return !strCheck.empty()
			&& strCheck[0] == '-'
			? std::find_if(strCheck.begin() + 1, strCheck.end(), [](unsigned char c) { return !std::isdigit(c); }) == strCheck.end() && stoll(strCheck) >= INT_MIN
			: std::find_if(strCheck.begin(), strCheck.end(), [](unsigned char c) { return !std::isdigit(c); }) == strCheck.end() && stoll(strCheck) <= INT_MAX;
	}


	virtual bool convert(int& dest, const string& str) {

		if (!isInt(str)) return false;
		dest = stoi(str);
		return true;
	}
};

class DoubleReader : public TReader<double> {

	bool isDouble(const string& str)
	{
		char* endptr = 0;
		double d = strtod(str.c_str(), &endptr);

		if (*endptr != '\0')
			return false;
		return true;
	}

	virtual bool convert(double& dest, const string& str) {

		if (!isDouble(str)) return false;
		dest = stod(str);
		return true;
	}
};

class StringReader : public TReader<string> {

	virtual bool convert(string& dest, const string& str) {

		if (str.c_str() == "") return false;
		dest = str.c_str();
		return true;
	}
};
