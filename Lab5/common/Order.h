#include "Readers.h"

struct Order
{
public:
	int n;
	char name[10];
	int amount;
	double price;

	Order() {}

	Order(int n, const char* name, int amount, double price) {
		this->n = n;
		if (name != nullptr)
			strcpy_s(this->name, name);
		this->amount = amount;
		this->price = price;
	}

	Order(int n, string& name, int amount, double price) {
		this->n = n;
		if (name != "")
			strcpy_s(this->name, name.c_str());
		this->amount = amount;
		this->price = price;
	}

	void operator=(Order* order) {
		this->n = order->n;
		if (name != nullptr)
			strcpy_s(this->name, order->name);
		this->amount = order->amount;
		this->price = order->price;
	}

public:
	void inputOrder() {

		StringReader sr;
		IntReader ir;
		DoubleReader dr;

		string name;
		ir.read(n, "Введите номер заказа: ", [](int& i) {return i < 1; }, "Невалидный номер заказа!");
		sr.read(name, "Введите имя товара: ", [](string& str) {if (str.size() > 9) return true; return false; }, "Невалидное имя товара!");
		strcpy_s(this->name, name.c_str());
		ir.read(amount, "Введите кол-во едениц товара: ", [](int& i) {return i < 1; }, "Невалидное кол-во едениц товара:!");
		dr.read(price, "Введите стоимость еденицы товара: ");
	}
};

std::ostream& operator<<(std::ostream& out, const Order& order) {
	out << "Номер заказа: " << order.n << endl << "Имя: " << order.name << endl
		<< "Кол-во едениц товара: " << order.amount << endl << "Стоимость еденицы товара: " << order.price << endl;
	return out;
}

std::ostream& operator<<(std::ostream& out, const Order* order) {
	out << "номер заказа: " << order->n << endl << "Имя: " << order->name << endl
		<< "Кол-во едениц товара: " << order->amount << endl << "Стоимость еденицы товара: " << order->price << endl;
	return out;
}

struct Meta
{
	int begin;
	int end;
	int numberOfOrder;

	Meta() {}

	Meta(int begin, int end, int numberOfOrder) {
		this->begin = begin;
		this->end = end;
		this->numberOfOrder = numberOfOrder;
	}
};

std::ostream& operator<<(std::ostream& out, const Meta& meta) {
	out << "end: " << meta.end << endl << "begin: " << meta.begin << endl << "numberOfOrder: " << meta.numberOfOrder << endl;
	return out;
}

std::ostream& operator<<(std::ostream& out, const Meta* meta) {
	out << "end: " << meta->end << endl << "begin: " << meta->begin << endl << "numberOfOrder: " << meta->numberOfOrder << endl;
	return out;
}