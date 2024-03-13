#pragma once
#ifndef _robot_H
#define _robot_H

#include <iostream>
using namespace std;

#include <math.h>
#include <string>
#include <vector>
#include <stack>
#include <deque>

#include "robot.h"
#include "staging.h"

class Staging;

class Robot {
public:
	bool flag_take123 = false;	//�Ƿ���123
	bool flag_deliver = false;	//�Ƿ������ͻ�
	bool flag_level2M = false;	//ִ�и��߽�����
	bool flag_wait = false;


	int robotId = 0;
	int in_worktop_id = 0;

	int sell_time = 0;

	int take_type = -1;

	double angleSpeed = 0.0;
	double x_lineSpeed = 0.0;
	double y_lineSpeed = 0.0;

	double time_value = 0.0;
	double collision_value = 0.0;

	double faceTo = 0;

	double x_loca;
	double y_loca;

	double tagetX = 0;
	double tagetY = 0;
	Staging tagetS;


	int tagetsNum[2] = { 0, 0 };

	Robot(){}

	Robot(int id, double x, double y):robotId(id), x_loca(x), y_loca(y)
	{
		Staging s(-1, -1, x, y);
		this->tagetS = s;
	}

	//ͨ����������ȡ��Ϣ
	void codeToInfo(string str)
	{
		int found = 0;
		vector<string> v;

		while (str.find(" ") != string::npos) {
			found = str.find(" ");
			v.push_back(str.substr(0, found));
			str = str.substr(found + 1);
		}
		v.push_back(str);

		vector<string>::iterator it = v.begin();
		this->in_worktop_id = stoi(*it);
		it++;
		this->take_type = stoi(*it);
		it++;
		this->time_value = stod(*it);
		it++;
		this->collision_value = stod(*it);
		it++;
		this->angleSpeed = stod(*it);
		it++;
		this->x_lineSpeed = stod(*it);
		it++;
		this->y_lineSpeed = stod(*it);
		it++;
		this->faceTo = stod(*it);
		it++;
		this->x_loca = stod(*it);
		it++;
		this->y_loca = stod(*it);
	}

	void locToLoc(double);

	void avoidCollision(vector<Robot>& vr);

	bool judgeRight(Robot r);

	// ��������˴�һ�㵽һ���ָ��
	//void locToLoc()
	//{
	//	double angleSpeed = 0;
	//	double lineSpeed = 0;
	//	double begin_x = this->x_loca;
	//	double begin_y = this->y_loca;
	//	double end_x = this->tagetS.x_location;
	//	double end_y = this->tagetS.y_location;
	//	
	//	//������end��ת����end
	//	double deta_x = end_x - begin_x;
	//	double deta_y = end_y - begin_y;
	//	double distance = pow(deta_x, 2) + pow(deta_y, 2);
	//	if (distance < 0.1)
	//	{
	//		//printf("rotate %d %f\n", this->robotId, 1.0);
	//		return;
	//	}
	//	double vector_a[2] = { deta_x, deta_y };
	//	double vector_e[2] = { cos(this->faceTo), sin(this->faceTo) };
	//	double the_dg = (sqrt(pow(vector_a[0], 2) + pow(vector_a[1], 2)) * sqrt(pow(vector_e[0], 2) + pow(vector_e[1], 2)));
	//	//����sin theta
	//	double sin_theta = vector_a[0] * vector_e[1] - vector_a[1] * vector_e[0];
	//	sin_theta = sin_theta / the_dg;
	//	//����cos theta
	//	double cos_theta = vector_a[0] * vector_e[0] + vector_a[1] * vector_e[1];
	//	cos_theta = cos_theta / the_dg;
	//	//a���e��sintheta�����㣬��ת
	//	if (sin_theta > 0)
	//	{
	//		//��ת
	//		//�ǶȱȽ�С��ת��һ��		
	//		if (cos_theta > 0.8) {
	//			angleSpeed = -1.0;	
	//		}
	//			
	//		else {
	//			angleSpeed = -3.1415926;
	//		}				
	//	}
	//	else
	//	{
	//		//theta����ת��
	//		if (cos_theta > 0.8)
	//		{
	//			angleSpeed = 1.0;
	//		}
	//		else
	//		{
	//			angleSpeed = 3.1415926;
	//		}
	//	}	
	//	//�ж�����
	//	//λ�ò���end��ǰ����end
	//	//�빤��̨��������߽�������ٽ���
	//	if (cos_theta > 0.5)
	//		lineSpeed = 2;
	//	if (cos_theta > 0.90)
	//		lineSpeed = 6;
	//	
	//	/*if (distance < 4)
	//		lineSpeed = 2;*/
	//	// ����һ��Ҫ��ס------------------------------------------------------
	//	/*if (distance < 0.16)
	//		lineSpeed = 0;*/
	//		//��ǽ
	//	if (this->y_loca > 48 || this->x_loca < 2 || this->y_loca < 2 || this->x_loca > 48)
	//		lineSpeed = 2;
	//	if (this->y_loca > 49 || this->x_loca < 1 || this->y_loca < 1 || this->x_loca > 49)
	//	{
	//		lineSpeed = 0.5;
	//	}
	//	//�жϳ�����ǽ�ڵĹ�ϵ
	//		//ƽ�� �������ٶ�
	//	if ((this->faceTo < 0.2 && this->faceTo > -0.2) || (this->faceTo > 1.37 && this->faceTo < 1.77))
	//		lineSpeed = 6;
	//	else if (this->faceTo > 2.94 || this->faceTo < -2.94 || (this->faceTo < -1.37 && this->faceTo > -1.77))
	//		lineSpeed = 6;
	//	printf("forward %d %f\n", this->robotId, lineSpeed);
	//	printf("rotate %d %f\n", this->robotId, angleSpeed);
	//}


	// ��ֹ��������ײ�ĺ���
	void avoidCrash(Robot r) {
		double distance = pow(this->x_loca - r.x_loca, 2) + pow(this->y_loca - r.y_loca, 2);
		// ���������˾������1 ���ÿ���
		if (distance > 1.2)
			return;
		else
		{
			//������ײ
		} 
	}



	// ��������ָ��
	// num�ǹ���̨��ID����ʾ�ڼ���ɨ�������
	bool buyGoods(Staging s, int num)
	{
		if (this->in_worktop_id == num && this->take_type == 0)
		//if (s.product_status == 1 && this->in_worktop_id == num && this->take_type == 0)
		{
			printf("buy %d\n", this->robotId);
			return true;
		}
		return false;
	}

	//�������ָ��, �����s��this-��tagetS
	bool sellGoods(Staging s, int num) 
	{
		if (s.staging_id == 1 || s.staging_id == 2 || s.staging_id == 3)
			return false;
		if (this->in_worktop_id == num && this->take_type != 0)
		{
			printf("sell %d\n", this->robotId);
			return true;
		}
		return false;
	}

	//, vector<Staging>* s7, vector<Staging>* s8, vector<Staging>* s9
	void test1(deque<Staging>* s)
	{
		/*if (this->take_type == 4 || this->take_type == 5 || this->take_type == 6)
		{
			this->flag_deliver = false;
			return;
		}*/

		//�������û����Ʒ
		if (this->take_type == 0)
		{
			//��ȡ1�� 2�� 3
			//this->tagetX = s->front().x_location;
			//this->tagetY = s->front().y_location;
			this->tagetS = s->front();
			if (this->buyGoods(s->front(), s->front().staging_num))
			{
				this->flag_take123 = true;
				//
				Staging temp = s->front();
				s->pop_front();
				s->push_back(temp);
			}
		}						
	}
	
};


#endif