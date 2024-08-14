#pragma once
#ifndef PASSENGER_H
#define PASSENGER_H

#include <iostream>
#include <string>
#include "DefineConst.h"
#include <fstream>

using namespace std;

struct Passenger
{
	string idPassenger = "";
	string lastName = "";
	string firstName = "";
	GENDER gender = INVALID;
	bool booked_ticket = false;

	Passenger() {};
	Passenger(const Passenger& passenger) {
		this->idPassenger = passenger.idPassenger;
		this->lastName = passenger.lastName;
		this->firstName = passenger.firstName;
		this->gender = passenger.gender;
		this->booked_ticket = passenger.booked_ticket;
	}
	void adjust() {
		int choice = 0;
		while (choice != 3) {
			cout << "1. Name 2.Gender 3. Exit" << endl;	cin >> choice;//----------------------------------TEXT HERE------------------------------------
			switch (choice)
			{
			case 1:
				cout << "Enter new First Name: ";//----------------------------------TEXT HERE------------------------------------
				cin >> this->firstName;	cin.ignore();
				cout << "Enter new Last Name: ";//----------------------------------TEXT HERE------------------------------------
				getline(cin,this->lastName);
				break;
			case 2:
				cout << "Enter Gender: ";//----------------------------------TEXT HERE------------------------------------
				int genderChoice; cin >> genderChoice;
				this->gender = (GENDER)genderChoice;
				break;
			}
		}
	}
};
struct NodePassenger {
	Passenger data;
	NodePassenger* pleft;
	NodePassenger* pright;
	int height;
};
struct PassengerList {
	NodePassenger* root;

	PassengerList() {
		root = NULL;
	}
	NodePassenger* createNode(Passenger data) {
		NodePassenger* p = new NodePassenger;
		p->data = data;
		p->pleft = p->pright = NULL;
		p->height = 1;
		return p;
	}

	int getHeight(NodePassenger* p) {
		if (p == NULL) return 0;
		return p->height;
	}
	NodePassenger* rotateRight(NodePassenger* p) {
		NodePassenger* q = p->pleft;
		p->pleft = q->pright;
		q->pright = p;
		p->height = max(getHeight(p->pleft), getHeight(p->pright)) + 1;
		q->height = max(getHeight(q->pleft), getHeight(q->pright)) + 1;
		return q;
	}
	NodePassenger* rotateLeft(NodePassenger* p) {
		NodePassenger* q = p->pright;
		p->pright = q->pleft;
		q->pleft = p;
		p->height = max(getHeight(p->pleft), getHeight(p->pright)) + 1;
		q->height = max(getHeight(q->pleft), getHeight(q->pright)) + 1;
		return q;
	}
	int getBalance(NodePassenger* p) {
		if (p == NULL) return 0;
		return getHeight(p->pleft) - getHeight(p->pright);
	}

	NodePassenger* insert(NodePassenger* p, Passenger data) {// change primary key to FirstName LastName-----------------------Check here-----------------------
		if (p == NULL) return createNode(data);
		if (data.idPassenger < p->data.idPassenger) p->pleft = insert(p->pleft, data);
		else if (data.idPassenger > p->data.idPassenger) p->pright = insert(p->pright, data);
		else return p; // If ID is already in the tree, return node that contains that ID
		p->height = 1 + max(getHeight(p->pleft), getHeight(p->pright));
		int balance = getBalance(p);
		if (balance > 1 && data.idPassenger < p->pleft->data.idPassenger) return rotateRight(p);
		if (balance < -1 && data.idPassenger > p->pright->data.idPassenger) return rotateLeft(p);
		if (balance > 1 && data.idPassenger > p->pleft->data.idPassenger) {
			p->pleft = rotateLeft(p->pleft);
			return rotateRight(p);
		}
		if (balance < -1 && data.idPassenger < p->pright->data.idPassenger) {
			p->pright = rotateRight(p->pright);
			return rotateLeft(p);
		}
		return p;
	}   

	void addPassenger(Passenger data) {
		root = insert(root, data);
	}
	NodePassenger* traverse(NodePassenger* p, string idPassenger) { //retrun NodePassenger* or NULL
		if (p == NULL || p->data.idPassenger == idPassenger) return p;
		if (idPassenger < p->data.idPassenger) return traverse(p->pleft, idPassenger);
		return traverse(p->pright, idPassenger);
	}
	NodePassenger* findPassenger(string idPassenger) {
		return traverse(root, idPassenger);
	}
	void adjustPassenger(string idPassenger) {
		NodePassenger* p = findPassenger(idPassenger);
		if (p == NULL) {
			cout << "Can't find passenger!!\n";//----------------------------------TEXT HERE------------------------------------
			return;
		}
		p->data.adjust();
	}
	NodePassenger* remove(NodePassenger* p, string idPassenger) {
		if (p == NULL) {
			cout << "Can't find passenger!!\n";//----------------------------------TEXT HERE------------------------------------
			return p;
		}
		if (idPassenger < p->data.idPassenger) p->pleft = remove(p->pleft, idPassenger);
		else if (idPassenger > p->data.idPassenger) p->pright = remove(p->pright, idPassenger);
		else {
			if (p->pleft == NULL || p->pright == NULL) { //1 or 0 child
				NodePassenger* temp = p->pleft ? p->pleft : p->pright;
				if (temp == NULL) { //0 child
					temp = p;
					p = NULL;
				}
				else *p = *temp; //1 child
				delete temp;
			}
			else { //2 children
				NodePassenger* temp = p->pright;
				while (temp->pleft != NULL) temp = temp->pleft;
				p->data = temp->data;
				p->pright = remove(p->pright, temp->data.idPassenger);
			}
		}
		if (p == NULL) return p;//If the tree had only one node, return
		p->height = 1 + max(getHeight(p->pleft), getHeight(p->pright));
		int balance = getBalance(p);
		if (balance > 1 && getBalance(p->pleft) >= 0) return rotateRight(p);
		if (balance > 1 && getBalance(p->pleft) < 0) {
			p->pleft = rotateLeft(p->pleft);
			return rotateRight(p);
		}
		if (balance < -1 && getBalance(p->pright) <= 0) return rotateLeft(p);
		if (balance < -1 && getBalance(p->pright) > 0) {
			p->pright = rotateRight(p->pright);
			return rotateLeft(p);
		}
		return p;
	}
	void removePassenger(string idPassenger) {
		root = remove(root, idPassenger);
	}
	void deletePassengerList(NodePassenger* p) {
		if (p == NULL) return;
		deletePassengerList(p->pleft);
		deletePassengerList(p->pright);
		delete p;
	}

	
};
void printPassengerList(NodePassenger* p) {
	if (p == NULL) return;
	printPassengerList(p->pleft);
	string gender = (p->data.gender == MALE) ? "Male" : "Female";
	cout << p->data.idPassenger << " " << p->data.lastName << " " << p->data.firstName << " " << gender << endl;
	printPassengerList(p->pright);
}
void loadPassengerList(PassengerList& passengerList)
{
	ifstream file("PassengerData.txt");
	if (!file.is_open()) {
		cout << "Can't open file" << endl;
		return;
	}
	Passenger passenger;
	while (!file.eof()) {
		file >> passenger.idPassenger; file.ignore();
		getline(file, passenger.lastName);
		file >> passenger.firstName;
		int genderInput; file >> genderInput;
		passenger.gender = (GENDER)genderInput;
		passengerList.addPassenger(passenger);
	}
	file.close();
}
void add_newPassenger(PassengerList& passengerList) {
	Passenger passenger;
	cout << "Enter ID: "; cin >> passenger.idPassenger;
	cout << "Enter First Name: "; cin >> passenger.firstName; cin.ignore();
	cout << "Enter Last Name: "; getline(cin, passenger.lastName);
	cout << "Gender: 0)Male 1)Female: "; int choice; cin >> choice;
	passenger.gender = choice == 0 ? MALE : choice == 1 ? FEMALE : INVALID;
	if (passenger.gender == INVALID) {
		cout << "Invalid gender";
		return;
	}
	passengerList.addPassenger(passenger);
}
void add_newPassenger(PassengerList& passengerList,string idPassenger) {
	Passenger passenger;
	passenger.idPassenger = idPassenger;
	cout << "Enter First Name: "; cin >> passenger.firstName; cin.ignore();
	cout << "Enter Last Name: "; getline(cin, passenger.lastName);
	cout << "Gender: 0)Male 1)Female "; int choice; cin >> choice;
	passenger.gender = choice == 0 ? MALE : choice == 1 ? FEMALE : INVALID;
	passenger.booked_ticket = true;
	if (passenger.gender == INVALID) {
		cout << "Invalid gender";
		return;
	}
	passengerList.addPassenger(passenger);
}
void save(NodePassenger* p,ofstream& file)
{
	if (p == NULL) return;
	file << p->data.idPassenger << endl << p->data.lastName << endl << p->data.firstName << endl << p->data.gender << endl;
	save(p->pleft,file);
	save(p->pright,file);
}
void savePassengerList(NodePassenger* p)
{
	ofstream file("PassengerData.txt");
	if (!file.is_open()) {
		cout << "Can't open file" << endl;
		return;
	}
	save(p,file);
	file.close();
}

#endif