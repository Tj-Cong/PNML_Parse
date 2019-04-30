//
// Created by hecong on 19-4-16.
//

#include<iostream>
#include"Petri_Net.h"
using namespace std;

/***********Global Functions***********/
/*unsigned int BKDRHash(string str);
 * function: get a string's hash value
 * BKDR: HashValue(str) = sum(str[i]*seed^(length-i-1))
 * in: string str
 * out: unsigned int hash value
 * */
unsigned int BKDRHash(string str)
{
    unsigned int seed = 31;
    unsigned int hash = 0;
    int length = str.length();
    int i = 0;
    for(i; i<length; i++)
    {
        hash += hash*seed + str[i];
    }
    hash = hash * seed;
    //保证最高位为0，这样在赋值给int型变量时，不用进行符号转换
    int result = hash & 0x7fffffff;
    return result;
}

/*int my_atoi(string str)
 * function: 将字符串转换为整型数据
 * in:string str, 由数字组成的字符串
 * out：int型数据
 * attention：该函数应当只能转换正整数，
 * 对于负数和小数，函数应当报错提示
 * */
int my_atoi(string str)
{
    int len = str.length();
    int i;
    int result = 0;
    for(i=0; i<len; i++)
    {
        if(str[i]>='0' && str[i]<='9') {
            result = result*10 + (str[i] - '0');
        }
        else {
            cerr<<"my_atoi error! The string contains illegal characters!"<<endl;
            return -1;
        }
    }
    return result;
}

int stringToNum(const string& str)
{
    istringstream iss(str);
    int num;
    iss >> num;
    return num;
}

/*****************Petri***************/
/*构造函数
 * */
Petri::Petri() {
    place = NULL;
    transition = NULL;
    arc = NULL;
    placecount = 0;
    transitioncount = 0;
    arccount = 0;
    phash_conflict_times = 0;
    thash_conflict_times = 0;
    ahash_conflict_times = 0;
}

/*析构函数
 * */
Petri::~Petri() {
    delete [] place;
    delete [] transition;
    delete [] arc;
}

/*void getSize();
 * function:this function is the first scan of PNML file.
 * After the first scan, we should get the size of Petri
 * net and store it in argument 'size';
 * in:
 * out: ‘PT_Size &size’;
 * */
void Petri::getSize(char *filename) {
    //读取文档
    TiXmlDocument *mydoc = new TiXmlDocument(filename);
    if(!mydoc->LoadFile()) {
        cerr << mydoc->ErrorDesc() <<endl;
    }

    //获得根元素
    TiXmlElement *root = mydoc->RootElement();
    if(root == NULL) {
        cerr<<"Failed to load file: no root element!"<<endl;
        mydoc->Clear();
    }
    string rootvalue = root->Value();

    TiXmlElement *net = root->FirstChildElement("net");
    TiXmlElement *page = net->FirstChildElement("page");

    //遍历每一页
    while(page)
    {
        TiXmlElement *pageElement = page->FirstChildElement();
        while(pageElement)
        {
            string value = pageElement->Value();
            if(value == "place") {
                placecount++;
            }
            else if(value == "transition") {
                transitioncount++;
            }
            else if(value == "arc")
            {
                arccount++;
            }
            pageElement = pageElement->NextSiblingElement();
        }
        page = page->NextSiblingElement();
    }

    size.placesum = placecount * MultiFactor;
    size.transitionsum = transitioncount * MultiFactor;
    size.arcsum = arccount * MultiFactor;
}

/*void Petri::allocHashTable()
 * function: allocate space for place, transition and arc
 * */
void Petri::allocHashTable() {
    if(size.transitionsum<=0 || size.placesum<=0 || size.arcsum<=0) {
        cerr<<"Error, wrong size!"<<endl;
        return;
    }

    //allocate space for place, transition and arc
    place = new Place [size.placesum];
    transition = new Transition [size.transitionsum];
    arc = new Arc [size.arcsum];
}

/*int Petri::arrange(Place p)
 * function: Given a place, put it into place hashtable
 * in:Place p, a place need putting into hashtable
 * out:its index in hashtable
 * */
int Petri::arrange(Place p) {
    int index = BKDRHash(p.id);
    if(index <= 0){
        cerr<<"BKDR error! index must be positive"<<endl;
        return -1;
    }
    index = index % size.placesum;
    int start = index;
    while (place[index].id != "" && (index+1) % size.placesum!=start) {
        index = (index+1)%size.placesum;
        phash_conflict_times++;
    }
    if(place[index].id == ""){
        place[index] = p;
        return index;
    }
    cerr<<"Error! Hashtable overflow!"<<endl;
    return -1;
}

/*similar with arrange(Place p)*/
int Petri::arrange(Transition t) {
    int index = BKDRHash(t.id);
    if(index<=0){
        cerr<<"BKDR error, index must be positive!"<<endl;
        return -1;
    }
    index = index % size.transitionsum;
    int start = index;
    while(transition[index].id != "" && (index+1)%size.transitionsum!=start) {
        index = (index+1) % size.transitionsum;
        thash_conflict_times++;
    }
    if(transition[index].id == ""){
        transition[index] = t;
        return index;
    }
    cerr<<"Error! Hashtable overflow!"<<endl;
    return -1;
}

/*similar with arrange(Place p)*/
int Petri::arrange(Arc a) {
    int index = BKDRHash(a.id);
    if(index <= 0){
        cerr<<"BKDR error, index must be positive!"<<endl;
        return -1;
    }
    index = index % size.arcsum;
    int start = index;
    while(arc[index].id != "" && (index+1)%size.arcsum!=start) {
        index = (index+1)%size.arcsum;
        ahash_conflict_times++;
    }
    if(arc[index].id == "") {
        arc[index] = a;
        return index;
    }
    cerr<<"BKDR error, index must be positive!"<<endl;
    return -1;
}

/*int Petri::getAPosition(string str)
 * function: Given a string(an arc's id), find its
 * index in arc Hashtable. if couldn't find, return -1;
 * in:string str, arc's id
 * out:arc's index
 * */
int Petri::getAPosition(string str) {
    int idx = BKDRHash(str);
    if(idx <= 0) {
        cerr<<"BKDR error! idx must be positive!"<<endl;
        return -1;
    }
    idx = idx % size.arcsum;
    int start = idx;
    while(arc[idx].id != str && (idx+1)%size.arcsum!=start){
        idx = (idx+1)%size.arcsum;
    }
    if(arc[idx].id == str) {
        return idx;
    }
    else {
        cerr<<"Error! Can not find "<<str<<" in Hashtable!"<<endl;
        return -1;
    }
}

/*int Petri::getPPosition(string str)
 * function: Given a string(a place's id), find its
 * index in place Hashtable. if couldn't find, return -1;
 * in:string str, place's id
 * out:place's index
 * */
int Petri::getPPosition(string str) {
    int idx = BKDRHash(str);
    if(idx <= 0) {
        cerr<<"BKDR error! idx must be positive!"<<endl;
        return -1;
    }
    idx = idx % size.placesum;
    int start = idx;
    while(place[idx].id != str && (idx+1)%size.placesum!=start){
        idx = (idx+1)%size.placesum;
    }
    if(place[idx].id == str) {
        return idx;
    }
    else {
        cerr<<"Error! Can not find "<<str<<" in Hashtable!"<<endl;
        return -1;
    }
}

/*int Petri::getTPosition(string str)
 * function: Given a string(a transition's id), find its
 * index in transition Hashtable. if couldn't find, return -1;
 * in:string str, transition's id
 * out:transition's index
 * */
int Petri::getTPosition(string str) {
    int idx = BKDRHash(str);
    if(idx <= 0) {
        cerr<<"BKDR error! idx must be positive!"<<endl;
        return -1;
    }
    idx = idx % size.transitionsum;
    int start = idx;
    while(transition[idx].id!=str && (idx+1)%size.transitionsum!=start) {
        idx = (idx+1)%size.transitionsum;
    }
    if(transition[idx].id == str) {
        return idx;
    }
    else{
        cerr<<"Error! Can not find "<<str<<" in Hashtable!"<<endl;
        return -1;
    }
}

int Petri::getPosition(string str, bool &isplace) {
    int idx = BKDRHash(str);
    if(idx <= 0) {
        cerr<<"BKDR error!, idx must be positive!"<<endl;
        return -1;
    }
    int pidx = idx % size.placesum;
    int tidx = idx % size.transitionsum;
    if(place[pidx].id == str){
        isplace = true;
        return pidx;
    }
    else if(transition[tidx].id == str) {
        isplace = false;
        return tidx;
    }
    else {
        int pstart = pidx;
        int tstart = tidx;
        while(place[pidx].id != str && transition[tidx].id != str
            && ((pidx+1)%size.placesum!=pstart || (tidx+1)%size.transitionsum!=tstart))
        {
            if((pidx+1)%size.placesum!=pstart)
                pidx = (pidx+1)%size.placesum;
            if((tidx+1)%size.transitionsum!=tstart)
                tidx =(tidx+1)%size.transitionsum;
        }
        if(place[pidx].id == str){
            isplace = true;
            return pidx;
        }
        else if(transition[tidx].id == str){
            isplace = false;
            return tidx;
        }
        else if((pidx+1)%size.placesum == pstart && (tidx+1)%size.transitionsum == tstart) {
            cerr<<"Error! Can not find "<<str<<" in both place and transition Hashtables"<<endl;
            return -1;
        }
    }
}

/*void Petri::readPNML(char *filename)
 * fucntion: Parse a PNML file, get its all
 * places, transitions and arcs, store them
 * in corresponding hashtables. After getting
 * them, construct a petri net(connect places
 * and arcs according to arcs)
 * */
void Petri::readPNML(char *filename) {
    //get size
    getSize(filename);

    //allocate space for three hashtables
    allocHashTable();

    //parse PNML
    //读取文档
    TiXmlDocument *mydoc = new TiXmlDocument(filename);
    if(!mydoc->LoadFile()) {
        cerr << mydoc->ErrorDesc() <<endl;
    }

    //获得根元素
    TiXmlElement *root = mydoc->RootElement();
    if(root == NULL) {
        cerr<<"Failed to load file: no root element!"<<endl;
        mydoc->Clear();
    }

    TiXmlElement *net = root->FirstChildElement("net");
    TiXmlElement *page = net->FirstChildElement("page");

    //遍历每一页
    while(page)
    {
        TiXmlElement *pageElement = page->FirstChildElement();
        while(pageElement)
        {
            string value = pageElement->Value();

            //库所
            if(value == "place") {
                Place p;
                TiXmlAttribute *id = pageElement->FirstAttribute();
                p.id = id->Value();
                TiXmlElement *initialmarking = pageElement->FirstChildElement("initialMarking");
                if(!initialmarking) {
                    p.initialMarking = 0;
                }
                else{
                    TiXmlElement *token = initialmarking->FirstChildElement("text");
                    if(token)
                    {
                        string marking = token->GetText();
                        p.initialMarking = stringToNum(marking);
                    }
                }
                arrange(p);
            }
            //变迁
            else if(value == "transition") {
                Transition t;
                TiXmlAttribute *id = pageElement->FirstAttribute();
                t.id = id->Value();
                arrange(t);
            }
            //弧
            else if(value == "arc")
            {
                Arc a;
                TiXmlAttribute *attr = pageElement->FirstAttribute();
                a.id = attr->Value();
                attr = attr->Next();
                a.source_id = attr->Value();
                attr = attr->Next();
                a.target_id = attr->Value();

                TiXmlElement *inscription = pageElement->FirstChildElement("inscription");
                if(!inscription){
                    a.weight = 1;
                }
                else {
                    TiXmlElement *text = inscription->FirstChildElement("text");
                    if(text){
                        string weight = text->GetText();
                        a.weight = stringToNum(weight);
                    }
                }
                arrange(a);
            }
            pageElement = pageElement->NextSiblingElement();
        }
        page = page->NextSiblingElement();
    }

    //construct a petri net
    bool issourcePlace;
    int i=0;
    int end = size.arcsum;
    for(i; i<end; i++)
    {
        if(arc[i].id == "")
            continue;
        int index = getPosition(arc[i].source_id,issourcePlace);
        if(index >=0)
        {
            if(issourcePlace){
                arc[i].P2T = true;
                int tidx = getTPosition(arc[i].target_id);
                if(tidx>=0)
                {
                    SArc place_post,tranx_pre;
                    place_post.weight = tranx_pre.weight = arc[i].weight;
                    place_post.idx = tidx;
                    tranx_pre.idx = index;
                    place[index].consumer.push_back(place_post);
                    transition[tidx].producer.push_back(tranx_pre);
                }
            }
            else {
                arc[i].P2T = false;
                int pidx = getPPosition(arc[i].target_id);
                if(pidx>=0){
                    SArc tranx_post, place_pre;
                    tranx_post.weight = place_pre.weight =arc[i].weight;
                    tranx_post.idx = pidx;
                    place_pre.idx = index;
                    place[pidx].producer.push_back(place_pre);
                    transition[index].consumer.push_back(tranx_post);
                }
            }
        }
    }
}

/*void printPlace();
 * function:按以下格式打印出所有的库所：
 * Total places：
 *
 * id：
 * index:
 * hashvalue:
 * initialMarking:
 * producer:
 * consumer:
id:p19
index:0
hashvalue:0
initialMarking0
producer:t21
consumer:t13

 * */
void Petri::printPlace() {
    ofstream outplace("places.txt", ios::out);
    outplace<<"Total places："<<placecount<<endl;
    outplace<<"conflict times:"<<phash_conflict_times<<endl;

    int i, placesize = size.placesum;
    for(i=0; i<placesize; i++)
    {
        Place p = place[i];
        if(p.id=="")
            continue;
        outplace<<endl;
        outplace<<"id:"<<p.id<<endl;
        outplace<<"index:"<<i<<endl;
        int hashvalue = BKDRHash(p.id) % size.placesum;
        outplace<<"hashvalue:"<<hashvalue<<endl;
        outplace<<"initialMarking:"<<p.initialMarking<<endl;
        outplace<<"producer:";
        vector<SArc>::iterator iterp;
        for(iterp=p.producer.begin(); iterp!=p.producer.end(); iterp++){
            outplace<<transition[iterp->idx].id<<" ";
        }
        outplace<<endl;
        outplace<<"consumer:";

        vector<SArc>::iterator iterc;
        for(iterc=p.consumer.begin(); iterc!=p.consumer.end(); iterc++){
            outplace<<transition[iterc->idx].id<<" ";
        }
        outplace<<endl;
    }

}

/*void printTransition()
 * function:打印出所有的变迁信息
 * 打印格式：
 * Total Transitions：
 *
 * id:
 * index:
 * hashvalue:
 * producer:
 * consumer:
 * */
void Petri::printTransition() {
    ofstream outTransition("transition.txt", ios::out);
    outTransition<<"Total transitions:"<<transitioncount<<endl;
    outTransition<<"conflict times:"<<thash_conflict_times<<endl;

    int i, transitionsize = size.transitionsum;
    for(i=0; i<transitionsize; i++)
    {
        Transition t = transition[i];
        if(t.id == "")
            continue;
        outTransition<<endl;
        outTransition<<"id:"<<t.id<<endl;
        outTransition<<"index:"<<i<<endl;
        int hashvalue = BKDRHash(t.id) % size.transitionsum;
        outTransition<<"hashvalue:"<<hashvalue<<endl;

        outTransition<<"producer:";
        vector<SArc>::iterator iterp;
        for(iterp=t.producer.begin(); iterp!=t.producer.end(); iterp++){
            outTransition<<place[iterp->idx].id<<" ";
        }
        outTransition<<endl;
        outTransition<<"consumer:";

        vector<SArc>::iterator iterc;
        for(iterc=t.consumer.begin(); iterc!=t.consumer.end(); iterc++){
            outTransition<<place[iterc->idx].id<<" ";
        }
        outTransition<<endl;
    }

}


