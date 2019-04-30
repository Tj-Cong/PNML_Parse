//
// Created by hecong on 19-4-16.
//

#ifndef PNML_PARSE_PETRI_NET_H
#define PNML_PARSE_PETRI_NET_H

#endif //PNML_PARSE_PETRI_NET_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "tinyxml.h"
using namespace std;

#define MultiFactor 1.3
/****************Global Functions**************/
unsigned int BKDRHash(string str);
int stringToNum(const string& str);
int my_atoi(string str);
/**********************************************/

typedef struct Small_Arc
{
    int weight;
    int idx;
} SArc;

typedef struct Place
{
    string id = "";
    vector<SArc> producer;
    vector<SArc> consumer;
    int initialMarking = 0;

} *Place_P;

typedef struct Transition
{
    string id = "";
    vector<SArc> producer;
    vector<SArc> consumer;
} *Transition_P;

typedef struct Arc
{
    string id = "";
    bool P2T;
    string source_id;
    string target_id;
    int source_idx;
    int target_idx;
    int weight;
} *Arc_P;

typedef struct Nodes_Count
{
    int placesum = 0;       //the count of places
    int transitionsum = 0;  //the count of transitions
    int arcsum = 0;         //the count of arcs
}PT_Size;

class Petri
{
private:
    Place *place;
    Transition *transition;
    Arc *arc;
    int placecount;
    int transitioncount;
    int arccount;
    PT_Size size;
    int phash_conflict_times;
    int thash_conflict_times;
    int ahash_conflict_times;
public:
    Petri();
    void getSize(char *filename);
    void allocHashTable();
    int arrange(Place place);
    int arrange(Transition transition);
    int arrange(Arc arc);
    int getPPosition(string str);
    int getTPosition(string str);
    int getAPosition(string str);
    int getPosition(string str, bool &isplace);
    void readPNML(char *filename);
    void printPlace();
    void printTransition();
    ~Petri();
};