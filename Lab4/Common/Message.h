#include "Readers.h"

struct Message
{
	char name[10];
	char text[20];

public:
	Message() {}

	Message(const char* name, const char* text) {
		if (name != nullptr)
			strcpy_s(this->name, name);
		if (text != nullptr)
			strcpy_s(this->text, text);
	}

	Message(string& name, string& text) {
		if (name != "")
			strcpy_s(this->name, name.c_str());
		if (text != "")
			strcpy_s(this->text, text.c_str());
	}
};

std::ostream& operator<<(std::ostream& out, const Message& message) {
	out << "Имя: " << message.name << endl << "Сообщение: " << message.text << endl;
	return out;
}

std::ostream& operator<<(std::ostream& out, const Message* message) {
	out << "Имя: " << message->name << endl << "Сообщение: " << message->text << endl;
	return out;
}

struct Meta
{
	int begin;
	int end;
	int numberOfMessage;

	Meta() {}

	Meta(int begin, int end, int numberOfMessage) {
		this->begin = begin;
		this->end = end;
		this->numberOfMessage = numberOfMessage;
	}
};

std::ostream& operator<<(std::ostream& out, const Meta& meta) {
	out << "end: " << meta.end << endl << "begin: " << meta.begin << endl << "numberOfMessage: " << meta.numberOfMessage << endl;
	return out;
}

std::ostream& operator<<(std::ostream& out, const Meta* meta) {
	out << "end: " << meta->end << endl << "begin: " << meta->begin << endl << "numberOfMessage: " << meta->numberOfMessage << endl;
	return out;
}