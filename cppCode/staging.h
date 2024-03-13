#pragma once
#ifndef _staging_H
#define _staging_H

#include <iostream>
using namespace std;

#include <math.h>
#include <string>
#include <vector>
#include <stack>
#include <deque>


#include "robot.h"
#include "staging.h"
class Robot;

class Staging {
public:
    bool deliver = false;
    bool flag_waitRobot = false;


    bool flag_wait4 = false;
    bool flag_wait5 = false;
    bool flag_wait6 = false;

    //产品格占用
    bool productEmploy = false;

    bool rawMtlEmploy = false;

	int staging_id = 0;
    int staging_num = 0;    //用于机器人的“所处工作台”
	double x_location = 0;
	double y_location = 0;

	int time_remain = 0;

	int raw_material_status = 0;
	int product_status = 0;
    
    Staging(){}

	Staging(int id, int num, double x, double y):staging_id(id),staging_num(num), x_location(x), y_location(y)
	{ }

	void codeToInfo(string str) {
        int found = 0;
        vector<string> v;

        while (str.find(" ") != string::npos) {
            found = str.find(" ");
            v.push_back(str.substr(0, found));
            str = str.substr(found + 1);
        }
        v.push_back(str);

        vector<string>::iterator it = v.begin();
        this->staging_id = stoi(*it);
        it += 3;
        this->time_remain = stoi(*it);
        it++;
        this->raw_material_status = stoi(*it);
        it++;
        this->product_status = stoi(*it);      
	}

    //void callRobotTo456(Robot*);
    void callRobotTo456(Robot* pr);

    void callRobotTo789(Robot* pr);

    bool is_lack1();
    bool is_lack2();
    bool is_lack3();

    bool is_lack4();
    bool is_lack5();
    bool is_lack6();
    bool is_lack7();

    bool is_lackAll();
    

};

#endif
