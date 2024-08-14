#pragma once
#include <iostream>
#include <string>
#include <cstdio>
#include "DefineConst.h"

using namespace std;

struct Flight
{
    string planeId;
    string type;
    int seats;
    Flight *next;

    Flight(string tmp_planeId, string tmp_type, int tmp_seats) : planeId(tmp_planeId), type(tmp_type), seats(tmp_seats) {}
};

Flight *head = nullptr;
int cnt = 0;

void addFlight(string planeId, string type, int seats)
{
    Flight *check = new Flight(planeId, type, seats);

    check->next = head;
    head = check;
    ++cnt;
}

void loadDataFlight(string filename)
{
    freopen(filename.c_str(), "r", stdin);

    string planeId, type, tmp;
    int seats, cnt = 0;

    while (getline(cin, tmp))
    {
        if (cnt % 3 == 0)
            planeId = tmp;
        else if (cnt % 3 == 1)
            type = tmp;
        else
        {
            seats = stoi(tmp);
            addFlight(planeId, type, seats);
        }

        ++cnt;
    }
}

void saveDataFlight(string filename)
{
    freopen(filename.c_str(), "w", stdout);

    Flight *cur = head;

    while (cur != nullptr)
    {
        cout << cur->planeId << "\n";
        cout << cur->type << "\n";
        cout << cur->seats << "\n";

        cur = cur->next;
    }
}

int checkNumber()
{
    Flight* tmp = head;
    int cnt = 0;

    while (tmp != nullptr)
    {
        tmp = tmp->next;
        ++cnt;
    }

    return cnt;
}

void printInfo()
{
    Flight* tmp = head;

    while (tmp != nullptr)
    {
        cout << tmp->planeId << " " << tmp->type << " " << tmp->seats << "\n";
        tmp = tmp->next;
    }
}

bool check_before_add_plane(string s1, string s2, string s3)
{
    if (s1.size() > 15 || s1.size() == 0 || s2.size() > 40 || s2.size() == 0)
        return false;

    for (int x : s3)
        if (x < '0' || x > '9')
            return false;

    return true;
}

bool check_numberOfSeats_before_add(string s)
{
    int temp_check = stoi(s);

    if (temp_check < MIN_SEAT || temp_check > MAX_SEAT)
        return false;

    return true;
}

Flight* search(int cnt)
{
    Flight* cur = head;

    while (cnt--)
        cur = cur->next;

    return cur;
}

// int main()
// {
//     // int t;
//     // cin >> t;

//     // for (int i = 0; i < t; i++)
//     // {
//     //     string planeId, type;
//     //     int seats;

//     //     cin >> planeId >> type >> seats;

//     //     addFlight(planeId, type, seats);
//     // }

//     // Flight* tmp = head;

//     // while (tmp != nullptr)
//     // {
//     //     cout << tmp->planeId << " " << tmp->type << " " << tmp->seats << "\n";
//     //     tmp = tmp->next;
//     // }

//     // freopen("data.txt", "r", stdin);
//     loadDataFlight("data.txt");

//     cout << cnt << "\n";

//     printInfo();

//     return 0;
// }
