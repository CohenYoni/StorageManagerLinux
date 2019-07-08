#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <string>
#include <map>
#include "product.h"
#include "database.h"
using namespace std;
class Parser {
	Database db;
	map<string, string> parsed;
	void addAction() ;
	void removeAction() ;
	void updateAction() ;
	void showAllAction() ;
	void showSpecificAction();
	bool isNumber(string);
public:
	Parser(string);
	void invoke() ;
};
#endif
