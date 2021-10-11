// made by https://github.com/Supsource

#include <iostream>
#include <fstream>
#include <regex>
#include <typeinfo>
#include "json/json.h"

using std::string;
using std::cout;
using std::cin;
using std::endl;

Json::Value getCarRecords() {
	// https://finbarr.ca/jsoncpp-example/
	Json::Value root;
	std::ifstream file("cars.json");
    file >> root;

	file.close();
	return root;
}


void setCarRecords(Json::Value root) {
	// Write the output to a file
	std::ofstream outFile;
	outFile.open("cars.json");
	outFile << root;
	outFile.close();    
}


string getName() {
	string str;
	cout << "Enter Car's Name: ";
	getline(cin, str);

	if(str == "") {
		cout << "Enter a name!" << endl;
		getName();
	}
	return str;
}


int carOptions() {
	cout << "1. Car Enquiry"<< endl;
	cout << "2. New Car"<< endl;
	cout << "3. Modify Car"<< endl;
	cout << "4. Remove Car"<< endl;
	cout << "5. Show All Cars"<< endl;
	cout << "6. Rent a Car"<< endl;
	cout << "7. Change Rent Records"<< endl;
	cout << "8. Show Rent Records"<< endl;
	cout << "0. Exit"<< endl;
	cout << "--- Choose any one option ---" << endl;
	cout << "Enter one option: ";

	int selectedOption = 0;
	cin >> selectedOption;
	cin.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' ); 

	return selectedOption;
}


void checkCar() {
	cout << "--- Check Car ---" << endl;

	string str = getName(); // get account holder's name
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	Json::Value list = getCarRecords();
	int len = list.size();

	for ( int i = 0; i < len; i++ )
    {
		string name = list[i]["name"].asString();
		string carName = name;
		transform(name.begin(), name.end(), name.begin(), ::tolower);

		if (name == str) {
			string model = list[i]["model"].asString();
			string carNum = list[i]["carNumber"].asString();
			string rentPrice = list[i]["rentPrice"].asString();

			cout << "Car Name: " << carName << endl;
			cout << "Car Number: " << carNum << endl;
			cout << "Model: " << model << endl;
			cout << "Model: " << rentPrice << endl;
			return;
		}

    }

	cout << "Record Not Found." << endl;
}


void addCar() {
	string carName, carNumber, id, model, rentPrice;

	Json::Value records = getCarRecords();
	int numOfCars = records.size();
	id = std::to_string(numOfCars);
	carNumber = "C100" + std::to_string(numOfCars);

	cout << endl << "--- Provide Car Details ---" << endl;
	cout << "Car Name: ";
	getline(cin, carName);
	cout << "Car Model: ";
	getline(cin, model);
	cout << "Rent Price: ";
	std::getline(std::cin, rentPrice);

	Json::Value record;
	record["id"] = id;
	record["name"] = carName;
	record["carNumber"] = carNumber;
	record["model"] = model;
	record["rentPrice"] = rentPrice;	

	records.append(record);
	setCarRecords(records);
	
   	cout << carName << " Car added. " << endl;
   	cout << "Car Number: " << carNumber << endl; 
}


void updateCar() {
	cout << endl << "--- Update Car ---" << endl;
	string str = getName();
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	Json::Value records = getCarRecords();
	int len = records.size();
	bool found = false;

    for ( int i = 0; i < len; i++ )
    {
		string value = records[i]["name"].asString();
		// convert to lowercase for comparision;
		std::transform(value.begin(), value.end(), value.begin(), ::tolower);

        if(str == value) {
			string name, model, rentPrice;

			cout << "New Name: ";
			std::getline(std::cin, name);

			cout << "Car Model: ";
			std::getline(std::cin, model);

			cout << "Rent Price: ";
			std::getline(std::cin, rentPrice);

			records[i]["name"] = name;
			records[i]["model"] = model;
			records[i]["rentPrice"] = rentPrice;

			cout << "Updated" << endl;
			found = true;
			break;
		}
    }

	if (!found) {
		cout << "Record Not Found." << endl;
	} else setCarRecords(records);
}


void removeCar() {
	string carNum;
	cout << endl << "--- Remove Car ---" << endl;
	cout << "Car's Number to remove " << endl;
	getline(cin, carNum);
	
	string& str = carNum;
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	Json::Value records = getCarRecords();
	int len = records.size();

	Json::Value newRecords;
	bool carExists = false;

    for ( int i = 0; i < len; i++ )
    {
		string value = records[i]["carNumber"].asString();
		// convert to lowercase for comparision;
		std::transform(value.begin(), value.end(), value.begin(), ::tolower);

        if(str == value) {
			carExists = true;
			continue;
		}
		newRecords.append(records[i]);
    }

	if(carExists) {
		setCarRecords(newRecords);
		cout << "Car Removed";
	} else {
		cout << "Car does not exist";
	}
}


void showCars() {
	cout << "--- List of Cars ---" << endl;

	Json::Value list = getCarRecords();
	int len = list.size();

	cout << "No. | Car Name | Car No. | Model | Rent Price (per day)" << endl;
	for ( int i = 0; i < len; i++ )
    {
		string name = list[i]["name"].asString();
		string num = list[i]["carNumber"].asString();
		string model = list[i]["model"].asString();
		string rentPrice = list[i]["rentPrice"].asString();

		int padding = 12 - name.length();
		if (padding > 0) {
			for(int j = 0; j < padding; j++) {
				name += " ";
			}
		}

		cout << i+1 << ".   " << name << "  " << num << "    " << model << "   $" << rentPrice << endl;
    }
}



// Rent
Json::Value getRentRecords() {
	// https://finbarr.ca/jsoncpp-example/
	Json::Value root;
	std::ifstream file("rents.json");
    file >> root;

	file.close();
	return root;
}

void setRentRecords(Json::Value root) {
	// Write the output to a file
	std::ofstream outFile;
	outFile.open("rents.json");
	outFile << root;
	outFile.close();    
}

struct renter {
	string name, phone, address;
};

typedef struct renter Renter;

void addRentRecord() {
	string carNumber, rentStartDate, rentEndDate, id;
	Renter r;

	Json::Value records = getRentRecords();
	int numOfRents = records.size();
	id = std::to_string(numOfRents);

	cout << endl << "--- Provide Rent Details ---" << endl;
	cout << "Car Number: ";
	getline(cin, carNumber);
	cout << "Renter's Name: ";
	getline(cin, r.name);
	cout << "Renter's Phone: ";
	getline(cin, r.phone);
	cout << "Renter's Address: ";
	getline(cin, r.address);

	// dd-mm-yy . format for date . eg: 01-03-2021
	cout << "Rent Start Date: ";
	getline(cin, rentStartDate);
	cout << "Rent End Date: ";
	getline(cin, rentEndDate);

	Json::Value record;
	record["id"] = id;
	record["renter"]["name"] = r.name;
	record["renter"]["phone"] = r.phone;
	record["renter"]["address"] = r.address;	
	record["car"]["carNumber"] = carNumber;
	record["rentStartDate"] = rentStartDate;
	record["rentEndDate"] = rentEndDate;

	records.append(record);
	setRentRecords(records);
	
   	cout << endl << r.name << " rented a car. " << endl;
   	cout << "Car Number: " << carNumber << endl; 
}


void showRentRecords() {
	cout << "--- Rent Records ---" << endl;

	Json::Value list = getRentRecords();
	int len = list.size();

	cout << "No. | " << "Renter's Name | " << "Car No. | " << "Rent End Date " << endl;
	for ( int i = 0; i < len; i++ )
    {
		string name = list[i]["renter"]["name"].asString();
		string num = list[i]["car"]["carNumber"].asString();
		string rentEndDate = list[i]["rentEndDate"].asString();

		int padding = 18 - name.length();
		if (padding > 0) {
			for(int j = 0; j < padding; j++) {
				name += " ";
			}
		}

		cout << i+1 << ". " << name << "  " << num << "    " << rentEndDate << endl;
    }
}


void changeRentRecords() {
	string carNumber, rentStartDate, rentEndDate;
	Renter r;
	bool found = false;

	cout << endl << "--- Update Rent Details ---" << endl;

	string str;
	cout << "Rent Id: ";
	getline(cin, str);
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	Json::Value records = getRentRecords();
	int len = records.size();

    for ( int i = 0; i < len; i++ )
    {
		string value = records[i]["id"].asString();
		// convert to lowercase for comparision;
		std::transform(value.begin(), value.end(), value.begin(), ::tolower);

        if(str == value) {
			cout << endl << "--- Provide New Rent Details ---" << endl;
			cout << "New Car Number: ";
			getline(cin, carNumber);
			cout << "New Renter's Name: ";
			getline(cin, r.name);
			cout << "New Renter's Phone: ";
			getline(cin, r.phone);
			cout << "New Renter's Address: ";
			getline(cin, r.address);

			// dd-mm-yy . format for date . eg: 01-03-2021
			cout << "Rent Start Date: ";
			getline(cin, rentStartDate);
			cout << "Rent End Date: ";
			getline(cin, rentEndDate);

			records[i]["renter"]["name"] = r.name;
			records[i]["renter"]["phone"] = r.phone;
			records[i]["renter"]["address"] = r.address;	
			records[i]["car"]["carNumber"] = carNumber;
			records[i]["rentStartDate"] = rentStartDate;
			records[i]["rentEndDate"] = rentEndDate;

			setRentRecords(records);
			cout << "Update Successful " << endl; 
			return;
		}
	}

	cout << "Rent not found." << endl;
}




void actions(int& option) {
	switch(option) {
		case 1: 
			checkCar();
			break;
		case 2: 
			addCar();
			break;
		case 3: 
			updateCar();
			break;
		case 4: 
			removeCar();
			break;
		case 5: 
			showCars();
			break;
		case 6: 
			addRentRecord();
			break;
		case 7: 
			changeRentRecords();
			break;
		case 8: 
			showRentRecords();
			break;
	}
}


void home () {
	int option = carOptions();
	if (option != 0 && option <= 8) {
		actions(option);
	} else if (option > 8) {
		cout << endl << "!!! Enter Valid Option !!!" << endl;
		option = carOptions();
	} else {
	 	exit(0);
	}
}


int main () {
	cout << "*** WELCOME ***";
	string yesOrNo;
	
	while(true) {
		cout << endl << "--- Car Rental System ---" << endl;
		home();
		cout << endl << "Continue? (y/n) :";
		cin >> yesOrNo;
		if(yesOrNo != "y") break;
	}

	cout << "Good Bye!" << endl;
}