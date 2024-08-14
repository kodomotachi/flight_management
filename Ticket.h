#pragma once
#ifndef TICKET_H
#define TICKET_H

#include "Passenger.h"		//Task: define BookTicket function

struct Ticket
{
	string idPassenger = "";


	void adjust() {
		int choice = 0;
		while (choice != 2) {
			cout << "1. Passenger ID  2. Exit" << endl;	cin >> choice;
			switch (choice)
			{
			case 1:
				cout << "Enter new Passenger ID: ";
				cin >> this->idPassenger;
				break;
			default:
				break;
			}
		}
	}
};
struct TicketList
{
	Ticket* tickets;
	int size = 0;
	TicketList() {}
	TicketList(int size) {
		this->size = size;
		tickets = new Ticket[size];
	}
	~TicketList() {}
	void adjustTicket(int idx) {
		tickets[idx].adjust();
	}
	void cancelTicket(int idx) {
		tickets[idx].idPassenger = "";
	}
	bool isBooked(int idx) {
		return tickets[idx].idPassenger != "";
	}
	bool isFull() {
		for (int i = 0; i < size; i++) {
			if (tickets[i].idPassenger == "") {
				return false;
			}
		}
		return true;
	}
	void print_Booked_TicketList(PassengerList& passengerList) {
		for (int i = 0; i < size; i++) {
			if (isBooked(i)) {
				NodePassenger* passenger = passengerList.findPassenger(tickets[i].idPassenger);
				cout << "Ticket " << i + 1 << ": " << endl;
				cout << "Passenger ID: " << tickets[i].idPassenger << " Name: " << passenger->data.lastName << ' ' << passenger->data.firstName<<endl;
			}
		}
	}
};
#endif
