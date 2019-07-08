#include "product.h"

Product::Product(int _identifier, string _category, string _name, double _price, int _quantity): identifier(_identifier),category(_category), name(_name), price(_price), quantity(_quantity) {}

void Product::setIdentifier(int identifier){
	this->identifier = identifier;
}

void Product::setCategory(string category){
	this->category = category;
}

void Product::setName(string name){
	this->name = name;
}

void Product::setPrice(double price){
	this->price = price;
}

void Product::setQuantity(int quantity){
	this->quantity = quantity;
}

int Product::getIdentifier(){
	return identifier;
}

string Product::getCategory(){
	return category;
}

string Product::getName(){
	return name;
}

double Product::getPrice(){
	return price;
}

int Product::getQuantity(){
	return quantity;
}
