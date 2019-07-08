#include "parser.h"

Parser::Parser(string uri) :db() {
	uri = uri.substr(1, uri.length()); // remove the '/' from the begining
	string action = uri.substr(0, uri.find('?')); //extract the action parameter
	uri = uri.substr(uri.find('?') + 1); // remove the action parameter
	parsed["action"] = action;
	while (uri.length() != 0) {
		string key = uri.substr(0, uri.find('=')); //extract the key parameter
		uri = uri.substr(uri.find('=') + 1); // remove the key parameter
		string value;
		if (uri.find('&') != string::npos) { //if the parameter is not at the finish
			value = uri.substr(0, uri.find('&')); //extract the value parameter
			uri = uri.substr(uri.find('&') + 1); // remove the value parameter
		} else { //the parameter is at the finish of the uri
			value = uri.substr(uri.find('=') + 1);
			uri = ""; //stop the loop
		}
		parsed[key] = value;
	}
}

bool Parser::isNumber(string str) {
	for (char& c : str)
		if (c < '0' || c > '9')
			return false;
	return true;
}


void Parser::addAction()  {
	if (!isNumber(parsed["ID"]))
		throw "ID argument must be an integer.";
	if (!isNumber(parsed["price"]))
		throw "price argument must be an integer.";
	if (!isNumber(parsed["quantity"]))
		throw "quantity argument must be an integer.";
	int ID = atoi(parsed["ID"].c_str());
	string category = parsed["category"];
	string name = parsed["name"];
	double price = atof(parsed["price"].c_str());
	int quantity = atoi(parsed["quantity"].c_str());

	Product newProduct(ID, category, name, price, quantity);

	db.addProduct(newProduct);
}

void Parser::removeAction()  {
	if (!isNumber(parsed["ID"]))
		throw "ID argument must be an integer.";
	db.deleteProduct(atoi(parsed["ID"].c_str()));
}

void Parser::updateAction()  {
	if (!isNumber(parsed["ID"]))
		throw "ID argument must be an integer.";
	if (!isNumber(parsed["quantity"]))
		throw "quantity argument must be an integer.";
	db.updateProduct(atoi(parsed["ID"].c_str()), atoi(parsed["quantity"].c_str()));
}

void Parser::showAllAction()  {
	db.showAll();
}

void Parser::showSpecificAction()  {
	if (!isNumber(parsed["ID"]))
		throw "ID argument must be an integer.";
	db.showSpecific(atoi(parsed["ID"].c_str()));
}

void Parser::invoke()  {
	try {
		if (parsed["action"] == "add") {
			addAction();
		} else if (parsed["action"] == "remove") {
			removeAction();
		} else if (parsed["action"] == "update") {
			updateAction();
		} else if (parsed["action"] == "showAll") {
			showAllAction();
		} else if (parsed["action"] == "showSpecific") {
			showSpecificAction();
		} else {
			cout << "Invalid action!" << "\n";
		}
	}
	catch (char const* errorMess) {
		cout << errorMess << endl;
		cerr << errorMess << endl;
	}
}
