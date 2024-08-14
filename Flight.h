#pragma once
#ifndef FLIGHT_H
#define FLIGHT_H

#include "Plane.h"
#include "Day.h"
#include "Ticket.h"

struct Flight {
	string id_Flight = "";
	Date boardingTime;
	string destination = "";
	STATUS status = CANCLE_FLIGHT;
	string idPlane = "";
	TicketList ticketList;
	Flight() {}
	Flight(const Flight& flight) {
		this->id_Flight = flight.id_Flight;
		this->boardingTime = flight.boardingTime;
		this->destination = flight.destination;
		this->status = flight.status;
		this->idPlane = flight.idPlane;
		this->ticketList = flight.ticketList;
	}
	void adjust() {
		cout << "Enter new Boarding Time:(dd mm yyyy hour minute) ";
		cin >> this->boardingTime.day >> this->boardingTime.month >> this->boardingTime.year >> this->boardingTime.hour >> this->boardingTime.minute;
		format(this->boardingTime);
		checkTime
	}
	
};
struct NodeFlight {
	Flight data;
	NodeFlight* next = NULL;
	NodeFlight() {}
	~NodeFlight() {
		data.ticketList.~TicketList();
	}
};
struct FlightList {
	NodeFlight* head;
	NodeFlight* tail;

	FlightList() {
		head = tail = NULL;
	}
	bool isEmpty() {
		return head == NULL;
	}
	NodeFlight* createNodeFlight(Flight& flight) {
		NodeFlight* p = new NodeFlight;
		p->data = flight;
		p->next = NULL;
		return p;
	}
	void insertFlight(Flight& flight) {
		NodeFlight* p = createNodeFlight(flight);
		if (isEmpty()) {
			head = tail = p;
		}
		else {
			tail->next = p;
			tail = p;
		}
	}
	NodeFlight* searchFlight(string id_Flight) const {
		NodeFlight* p = head;
		while (p != NULL && p->data.id_Flight != id_Flight) {
			p = p->next;
		}
		return p;
	}
	void adjustFlight(string id_Flight) {
		NodeFlight* p = searchFlight(id_Flight);
		if (p == NULL) {
			cout << "Flight not found" << endl;//----------------------------------TEXT HERE------------------------------------
			return;
		}
		p->data.adjust();
	}
	//void removeFlight(string id_Flight) {
	//	NodeFlight* p = head;
	//	NodeFlight* prev = NULL;
	//	while (p != NULL && p->data.id_Flight != id_Flight) {
	//		prev = p;
	//		p = p->next;
	//	}
	//	if (p == NULL) {
	//		cout << "Flight not found" << endl;//----------------------------------TEXT HERE------------------------------------
	//		return;
	//	}
	//	if (p == head) {
	//		head = head->next;
	//		delete p;
	//	}
	//	else if (p == tail) {
	//		tail = prev;
	//		delete p;
	//	}
	//	else {
	//		prev->next = p->next;
	//		delete p;
	//	}
	//}
	
};
void printFlightList(NodeFlight* p) {
	if (p == NULL) return;
	cout <<"ID Flight: " << p->data.id_Flight << "\n" <<" Destination: " << p->data.destination << "\n" << "Boarding Time: ";
	printf("%s\n", getDateString(p->data.boardingTime));
	cout<<"ID Plane: " << p->data.idPlane << "\n" <<"Status: ";
	switch (p->data.status)
	{
	case CANCLE_FLIGHT:
		cout << "Canceled\n";
		break;
	case HAVE_TICKET:
		cout << "Have Ticket\n";
		break;
	case OUT_OF_TICKET:
		cout << "Sold Out\n";
		break;
	case COMPLETE_FLIGHT:
		cout << "Complete\n";
		break;
	}
	printFlightList(p->next);
}

string generateID_Flight(FlightList flightList) {
	string id = "CB";
	if (flightList.isEmpty()) {
		id = "CB00001";
		return id;
	}
	NodeFlight* p = flightList.tail;
	string lastID = p->data.id_Flight;
	int number = stoi(lastID.substr(2, lastID.length() - 2)) + 1;
	for(int i = 10000; number < i; i /= 10) {
		id += "0";
	}
	id = id + to_string(number);
	return id;
}
void deleteFlightList(NodeFlight* p) {
	while (p != NULL) {
		NodeFlight* q = p;
		p = p->next;
		q->data.ticketList.~TicketList();
		delete q;
	}
}
NodeFlight* findPlaneInFlight(NodeFlight* p, string idPlane) {
	while (p != NULL && p->data.idPlane != idPlane) {
		p = p->next;
	}
	return p;
}
//void loadFlightList(FlightList& flightList) {
//	ifstream file("FlightData.txt");
//	if (!file.is_open()) {
//		cout << "Can't open file!" << endl;//----------------------------------TEXT HERE------------------------------------
//		return;
//	}
//	Flight flight;
//	int n;
//	file >> n;
//	file.ignore();
//	for (int i = 0; i < n; i++)
//	{
//		getline(file, flight.id_Flight);
//		file >> flight.boardingTime.day >> flight.boardingTime.month >> flight.boardingTime.year >> flight.boardingTime.hour >> flight.boardingTime.minute;
//		file.ignore();
//		getline(file, flight.destination);
//		int status;
//		file >> status;
//		flight.status = (STATUS)status;
//		file.ignore();
//		getline(file, flight.idPlane);
//		int m;
//		file >> m;
//		file.ignore();
//		for (int j = 0; j < m; j++)
//		{
//			Ticket ticket;
//			getline(file, ticket.id_Ticket);
//			file >> ticket.id_Passenger >> ticket.seat;
//			file.ignore();
//			flight.ticketList.addTicket(ticket);
//		}
//		flightList.insertFlight(flight);
//	}
//	file.close();
//}
#endif
