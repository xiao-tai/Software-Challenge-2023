#pragma once
#ifndef _relevantAlog_H
#define _relevantAlog_H


#include "robot.h"
#include "staging.h"


double dRobotToStage(Robot r, Staging s);
double dStageToStage(Staging s1, Staging s2);

void withoutGoodsGreedyAlg(Robot* r, vector<Staging>& vs, int* tagets);
void withoutGoodsGreedyOnlyOne7(Robot* r, vector<Staging>& vs, int* tagets);
void takeGoodsGreedyAlg(Robot* r, vector<Staging>& vs, int* tagets);

bool notEnoughTime(Robot& r, vector<Staging>& vs, int frameID);

void workByInts(Robot* r, vector<Staging>& vs, int* num);

void buyByInts(Robot* r, vector<Staging>& vs, int* num);
void sellByInts(Robot* r, vector<Staging>& vs, int* num);


int getMaxIndexByD(vector<Robot>& vr, Staging& s);




#endif
