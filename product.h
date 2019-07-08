#ifndef PRODUCT_H
#define PRODUCT_H

#include <iostream>
#include <string>
#include <sqlite3.h>
#include <cstdlib>

using namespace std;

class Product {
private:
	int identifier;
	string category;
	string name;
	double price;
	int quantity;
public:
	Product(int _identifier, string _category, string _name, double _price, int _quantity);
	void setIdentifier(int identifier);
	void setCategory(string category);
	void setName(string name);
	void setPrice(double price);
	void setQuantity(int quantity);
	int getIdentifier();
	string getCategory();
	string getName();
	double getPrice();
	int getQuantity();
};
#endif 
