#pragma once
#ifndef DEFINE_H
#define DEFINE_H

//--------------PLANE---------
#define MAX_ID_PLANE 15
#define MAX_TYPE_PLANE 40
#define MIN_SEAT  20
#define MAX_SEAT 300 //??
#define MAX_PLANE 300

//--------------FLIGHT--------
#define MAX_ID_FLIGHT 15
#define MAX_ARRIVE 40
enum STATUS { CANCLE_FLIGHT, HAVE_TICKET, OUT_OF_TICKET, COMPLETE_FLIGHT };

//--------------PASSENGER-----
#define MAX_LAST_NAME 10
#define MAX_FIRST_NAME 50
#define MAX_ID_PASS 12
enum GENDER {MALE, FEMALE ,INVALID};

#endif