#pragma once
#include <iostream>
#include <string>
using namespace std;

struct Passenger {
    int ticketNumber;
    string idNumber;
    string lastName;
    string firstName;
    string gender;
    Passenger* left;
    Passenger* right;
    Passenger* next;

    Passenger(int ticket, string id, string last, string first, string gen) 
        : ticketNumber(ticket), idNumber(id), lastName(last), firstName(first), gender(gen), left(nullptr), right(nullptr), next(nullptr) {}
};

struct Flight {
    string flightNumber;
    string departureTime;
    string arrivalAirport;
    int status; // 0: canceled, 1: available, 2: sold out, 3: completed
    string aircraftNumber;
    Passenger* passengerHead;
    Flight* next;

    Flight(string fNum, string dTime, string aAirport, int stat, string aNum) 
        : flightNumber(fNum), departureTime(dTime), arrivalAirport(aAirport), status(stat), aircraftNumber(aNum), passengerHead(nullptr), next(nullptr) {}
};

struct Airplane {
    string aircraftNumber;
    string aircraftType;
    int numberOfSeats;
};

// Maximum of 300 airplanes
const int MAX_AIRPLANES = 300;
Airplane* airplanes[MAX_AIRPLANES];
int airplaneCount = 0;

// Head pointer for the linked list of flights
Flight* flightHead = nullptr;

// Root pointer for the binary search tree of passengers
Passenger* passengerRoot = nullptr;

// Function to add a new airplane
void addAirplane(string aircraftNumber, string aircraftType, int numberOfSeats) {
    if (airplaneCount < MAX_AIRPLANES) {
        Airplane* newAirplane = new Airplane{aircraftNumber, aircraftType, numberOfSeats};
        airplanes[airplaneCount++] = newAirplane;
    } else {
        cout << "Cannot add more airplanes. Maximum limit reached." << endl;
    }
}

// Function to add a new flight
void addFlight(string flightNumber, string departureTime, string arrivalAirport, int status, string aircraftNumber) {
    Flight* newFlight = new Flight{flightNumber, departureTime, arrivalAirport, status, aircraftNumber};
    
    if (flightHead == nullptr) {
        flightHead = newFlight;
    } else {
        Flight* temp = flightHead;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newFlight;
    }
}

// Function to insert a passenger into the BST
Passenger* insertPassenger(Passenger* root, Passenger* newPassenger) {
    if (root == nullptr) {
        return newPassenger;
    }
    if (newPassenger->idNumber < root->idNumber) {
        root->left = insertPassenger(root->left, newPassenger);
    } else {
        root->right = insertPassenger(root->right, newPassenger);
    }
    return root;
}

// Function to add a passenger to a flight
void addPassengerToFlight(string flightNumber, int ticketNumber, string idNumber, string lastName, string firstName, string gender) {
    Flight* temp = flightHead;
    while (temp != nullptr) {
        if (temp->flightNumber == flightNumber) {
            Passenger* newPassenger = new Passenger(ticketNumber, idNumber, lastName, firstName, gender);
            newPassenger->next = temp->passengerHead;
            temp->passengerHead = newPassenger;
            passengerRoot = insertPassenger(passengerRoot, newPassenger);
            return;
        }
        temp = temp->next;
    }
    cout << "Flight not found." << endl;
}

// Function to display all airplanes
void displayAirplanes() {
    for (int i = 0; i < airplaneCount; i++) {
        cout << "Aircraft Number: " << airplanes[i]->aircraftNumber << ", Type: " << airplanes[i]->aircraftType
             << ", Number of Seats: " << airplanes[i]->numberOfSeats << endl;
    }
}

// Function to display all flights
void displayFlights() {
    Flight* temp = flightHead;
    while (temp != nullptr) {
        cout << "Flight Number: " << temp->flightNumber << ", Departure Time: " << temp->departureTime
             << ", Arrival Airport: " << temp->arrivalAirport << ", Status: " << temp->status
             << ", Aircraft Number: " << temp->aircraftNumber << endl;
        cout << "Passenger List:" << endl;
        Passenger* passengerTemp = temp->passengerHead;
        while (passengerTemp != nullptr) {
            cout << "  Ticket Number: " << passengerTemp->ticketNumber << ", ID Number: " << passengerTemp->idNumber
                 << ", Name: " << passengerTemp->lastName << " " << passengerTemp->firstName << ", Gender: " << passengerTemp->gender << endl;
            passengerTemp = passengerTemp->next;
        }
        temp = temp->next;
    }
}

// Function to search for a passenger by ID in the BST
Passenger* searchPassenger(Passenger* root, string idNumber) {
    if (root == nullptr || root->idNumber == idNumber) {
        return root;
    }
    if (idNumber < root->idNumber) {
        return searchPassenger(root->left, idNumber);
    }
    return searchPassenger(root->right, idNumber);
}

int main() {
    // Adding airplanes
    addAirplane("A123", "Boeing 747", 300);
    addAirplane("A124", "Airbus A320", 180);

    // Adding flights
    addFlight("F001", "2024-08-01 10:00", "JFK", 1, "A123");
    addFlight("F002", "2024-08-01 12:00", "LAX", 1, "A124");

    // Adding passengers to flights
    addPassengerToFlight("F001", 1, "123456789", "Nguyen", "An", "M");
    addPassengerToFlight("F001", 2, "987654321", "Tran", "Binh", "F");
    addPassengerToFlight("F002", 1, "111111111", "Le", "Cuong", "M");

    // Displaying all airplanes
    displayAirplanes();

    // Displaying all flights
    displayFlights();

    // Searching for a passenger by ID
    Passenger* foundPassenger = searchPassenger(passengerRoot, "123456789");
    if (foundPassenger != nullptr) {
        cout << "Passenger found: " << foundPassenger->lastName << " " << foundPassenger->firstName 
             << ", Gender: " << foundPassenger->gender << endl;
    } else {
        cout << "Passenger not found." << endl;
    }

    return 0;
}
