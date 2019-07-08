#include "database.h"

bool Database::exists = false; // Static var, existsCallback is a static function and that's the reason it's static too.

/* Function that prints every row of the query. Each time the function gets a single line of data.
data is unused, argc is the amount of cols in the DB, argv is a single line of data, azColName is the names of the cols.
*/
int Database::callback(void *data, int argc, char **argv, char **azColName) {
	int i;
	for (i = 0; i < argc; i++) {
		cout << azColName[i] << " = " << argv[i] ? argv[i] : "NULL";
		cout << endl;
	}
	cout << "______________________________" << endl;
	return 0;
}

/* Function that checks if the query returned an empty table or not.*/
int Database::existsCallback(void *data, int argc, char **argv, char **azColName){
	if (argv[0]) //The first index of argv array is the Identifier.
		exists = true;
    return 0;
}

/* Constructor. Gets a fileName and creates (if not exists) a new Database file.
sqlite3_open creates the file and gives DB the link to the file, just like working with files.
the query contains a "decleration" of how the table supposed to look like, columns names etc.
sqlite3_exec executes the query In that case, creates a table with 5 cols, their names and types.
*/
Database::Database(string fileName) {
	DBName = fileName;
	fileName += ".db";
	if (sqlite3_open(fileName.c_str(), &DB)) {
		throw "Database.cpp: Couldn't open database.";		
	}
	string query = "CREATE TABLE IF NOT EXISTS '" + DBName + "' ("\
	"Identifier INT NOT NULL,"\
	"Category VARCHAR(30) NOT NULL,"\
	"Name VARCHAR(30) NOT NULL,"\
	"Price REAL NOT NULL,"\
	"Quantity INT NOT NULL);";
	char* zErrMsg = 0;
	if (sqlite3_exec(DB, query.c_str(), 0, 0, &zErrMsg)) {
		cout << "SQL error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);
	}
}

Database::Database() :Database("Storage") {}

/* Destructor, closes the table.*/
Database::~Database() {
	sqlite3_close(DB);
}

/* Function that adds a product into the table.
First, checks if the identifier already exists, it's impossible to add the same product twice (an exception is being thrown).
After, Builds a query using the detains of p argument.
And then, executes the query.
*/
void Database::addProduct(Product& p) {
	if(p.getPrice() < 0)
		throw "Database.cpp: Illegal price!";
	if(p.getQuantity() < 0)
		throw "Database.cpp: Illegal quantity!";
	if(p.getIdentifier() < 0)
		throw "Database.cpp: Illegal identifier!";
	isExist(p.getIdentifier());
	if(exists)	
		throw "Database.cpp: Product already exists!";
	int rc;
	char *zErrMsg = 0;
	string query = "INSERT INTO '" + DBName + "' (Identifier, Category, Name, Price, Quantity) VALUES ('";
	query += to_string(p.getIdentifier()) + "', '" + p.getCategory() + "', '" + p.getName() + "', '";
	query += to_string(p.getPrice()) + "', '" + to_string(p.getQuantity()) + "');";
	rc = sqlite3_exec(DB, query.c_str(), 0, 0, &zErrMsg);
	if (rc != SQLITE_OK){
		cout << "SQL error:\n" << zErrMsg << endl;
	}else{
		cout << "Product added successfully." << endl;
	}	
}

/* Function that removes a product From the table.
First, checks if the identifier already exists, it's impossible to remove a product that doesn't exist (an exception is being thrown).
After, Builds a query using the requested identifier.
And then, executes the query.
*/
void Database::deleteProduct(int identifier) {
	isExist(identifier);
	if(!exists)	
		throw "Database.cpp: Product doesn't exist!";
	int rc;
	char *zErrMsg = 0;
	string query = "DELETE FROM '" + DBName + "' WHERE Identifier = '" + to_string(identifier) + "';";
	rc = sqlite3_exec(DB, query.c_str(), 0, 0, &zErrMsg);
	if (rc != SQLITE_OK){
		cout << "SQL error:\n" << zErrMsg << endl;
		sqlite3_free(zErrMsg);
	}else{
		cout << "Product deleted successfully." << endl;
	}
}

/* Function that update the quantity of a product From the table.
First, checks if the identifier already exists, it's impossible to update the quantity of a product that doesn't exist (an exception is being thrown).
After, Builds a query using the requested identifier and quantity.
And then, executes the query.
*/
void Database::updateProduct(int identifier, int quantity) {
	isExist(identifier);
	if(!exists)	
		throw "Database.cpp: Product doesn't exist!";
	int rc;
	char *zErrMsg = 0;
	string query = "UPDATE '" + DBName + "' SET Quantity = '" + to_string(quantity) + "' WHERE Identifier = '" + to_string(identifier) + "';";
	rc = sqlite3_exec(DB, query.c_str(), 0, 0, &zErrMsg);
	if (rc != SQLITE_OK){
		cout << "SQL error:\n" << zErrMsg << endl;
		sqlite3_free(zErrMsg);
	}else{
		cout << "Product updated successfully." << endl;
	}
}

/* Function that prints the table's content.
Builds a query that selects the whole table, checks if the table isn't empty and then executes the query.
*/
void Database::showAll() {
	char *zErrMsg = 0;
	string query = "SELECT * FROM '" + DBName + "';";
	exists = false;
	sqlite3_exec(DB, query.c_str(), existsCallback, (void*)"existsCallback function called", &zErrMsg);
	if(!exists)
		throw "Database.cpp: Database is Empty!";
	cout << "______________________________" << endl;
	if (sqlite3_exec(DB, query.c_str(), callback, (void*)"Callback function called", &zErrMsg)){
		cout <<  "SQL error:\n" << zErrMsg << endl;
		sqlite3_free(zErrMsg);
	}
}

/* Function that prints the details of a specific product.
Checks if the identifier exists and prints it in means of the callback function.
*/
void Database::showSpecific(int identifier) {
	isExist(identifier);
	if(!exists)
		throw "Database.cpp: Product doesn't exist!";
	char *zErrMsg = 0;
	string query = "SELECT * FROM '" + DBName + "' WHERE Identifier = '" + to_string(identifier) + "';";
	cout << "______________________________" << endl;
	if (sqlite3_exec(DB, query.c_str(), callback, (void*)"Callback function called", &zErrMsg)){
		cout <<  "SQL error:\n" << zErrMsg << endl;
		sqlite3_free(zErrMsg);
	}
}

/* Function that checks if the query returned an empty table, which means that there is no content for the requested query.*/
void Database::isExist(int identifier) {
	char *zErrMsg = 0;
	exists = false;
	string query = "SELECT * FROM '" + DBName + "' WHERE Identifier = '" + to_string(identifier) + "';";
	sqlite3_exec(DB, query.c_str(), existsCallback, (void*)"Callback function called", &zErrMsg);
}


