#ifndef DATABASE_H
#define DATABASE_H

#include "product.h"
#include <exception>

class Database{
private:
	sqlite3* DB;
	string DBName;
	static bool exists;
public:
	static int callback(void *data, int argc, char **argv, char **azColName);
	static int existsCallback(void *data, int argc, char **argv, char **azColName);
	Database(string DBname);
	Database();
	~Database();
	void addProduct(Product& p);
	void deleteProduct(int identifier);
	void updateProduct(int identifier, int quantity);
	void showAll();
	void showSpecific(int identifer);
	void isExist(int identifier);
};

#endif
