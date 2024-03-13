#include <iostream>
using namespace std;
//
#include <math.h>
#include <string>
#include <vector>

#include "robot.h"
#include "relevantALog.h"



void Robot::locToLoc(double angleWeight)
{
	if (this->flag_wait)
	{
		printf("forward %d %f\n", this->robotId, 0.0);
		printf("rotate %d %f\n", this->robotId, 1.0);
		return;
	}

	double angleSpeed = 0;
	double lineSpeed = 0;

	double begin_x = this->x_loca;
	double begin_y = this->y_loca;
	double end_x = this->tagetS.x_location;
	double end_y = this->tagetS.y_location;

	//������end��ת����end
	double deta_x = end_x - begin_x;
	double deta_y = end_y - begin_y;
	double distance = pow(deta_x, 2) + pow(deta_y, 2);
	if (distance < 0.1)
	{
		//һ��ʼĿ�����Լ������·���
		//printf("rotate %d %f\n", this->robotId, 1.0);
		return;
	}

	double vector_a[2] = { deta_x, deta_y };
	double vector_e[2] = { cos(this->faceTo), sin(this->faceTo) };
	//�����ĸ
	double the_dg = (sqrt(pow(vector_a[0], 2) + pow(vector_a[1], 2)) * sqrt(pow(vector_e[0], 2) + pow(vector_e[1], 2)));
	//����sin theta
	double sin_theta = vector_a[0] * vector_e[1] - vector_a[1] * vector_e[0];
	sin_theta = sin_theta / the_dg;
	//����cos theta
	double cos_theta = vector_a[0] * vector_e[0] + vector_a[1] * vector_e[1];
	cos_theta = cos_theta / the_dg;

	//a���e��sintheta�����㣬��ת
	if (sin_theta > 0)
	{
		//��ת
		//�ǶȱȽ�С��ת��һ��		
		if (cos_theta > 0.95) {
			angleSpeed = -0.4;
		}
		else {
			angleSpeed = -3.1415926 * angleWeight;
		}
	}
	else
	{
		//theta����ת��
		if (cos_theta > 0.95)
		{
			angleSpeed = 0.4;
		}
		else
		{
			angleSpeed = 3.1415926 * angleWeight;
		}
	}
	//�ж�����
	//λ�ò���end��ǰ����end
	//�빤��̨��������߽�������ٽ���
	

	if (cos_theta > 0.50)
		lineSpeed = 6;
		/*else if (cos_theta > 0.5)
		lineSpeed = 5;*/
		//�нǴ��ʱ���ٶ���΢����
	else if (cos_theta > 0.3)
		lineSpeed = 3 / angleWeight;		//����汾��1
	else
		lineSpeed = 0.6;

	//�������̨��߽�ȽϽ����Ϳ�����ʱ����
	if (this->tagetS.x_location < 5 || this->tagetS.x_location > 45 || this->tagetS.y_location < 5 || this->tagetS.y_location > 45)
	{
		if (distance < 4)
			lineSpeed = 3;
		if (cos_theta < 0.8)
			lineSpeed = 1;
	}

	//�жϳ�����ǽ�ڵĹ�ϵ
	//ƽ�� �������ٶ�
	/*if ((this->faceTo < 0.2 && this->faceTo > -0.2) || (this->faceTo > 1.37 && this->faceTo < 1.77))
		lineSpeed = 6;
	if ((this->faceTo > 2.94 || this->faceTo < -2.94) || (this->faceTo < -1.37 && this->faceTo > -1.77))
		lineSpeed = 6;*/
	printf("forward %d %f\n", this->robotId, lineSpeed);
	printf("rotate %d %f\n", this->robotId, angleSpeed);
}


void Robot::avoidCollision(vector<Robot>& vr)
{
	double distance1 = dRobotToStage(*this, this->tagetS);
	if (distance1 < 4)
		return;
	int index = this->robotId;
	//���������  ��ǰ��֡�жϣ���0.06
	double advanceFrame = 0.3;
	//����ת����
	double x_selfNext = 0.0;
	double y_selfNext = 0.0;
	double angle_self = 0.0;
	double vector_self[2] = {cos(this->faceTo), sin(this->faceTo)};
	if(this->faceTo > 0 && this->faceTo < 1.5707)
	{
		x_selfNext = this->x_loca + this->x_lineSpeed * advanceFrame;
		y_selfNext = this->y_loca + this->y_lineSpeed * advanceFrame;
	}
	else if (this->faceTo > 1.5707)
	{
		x_selfNext = this->x_loca - this->x_lineSpeed * advanceFrame;
		y_selfNext = this->y_loca + this->y_lineSpeed * advanceFrame;
	}
	if (this->faceTo < 0 && this->faceTo > -1.5707)
	{
		x_selfNext = this->x_loca + this->x_lineSpeed * advanceFrame;
		y_selfNext = this->y_loca - this->y_lineSpeed * advanceFrame;
	}
	else if (this->faceTo < -1.5707)
	{
		x_selfNext = this->x_loca - this->x_lineSpeed * advanceFrame;
		y_selfNext = this->y_loca - this->y_lineSpeed * advanceFrame;
	}
	for (int i = 0; i < vr.size(); i++)
	{
		//���Լ����⣬���������˽��б���
		if (i != index)
		{
			double vector_him[2] = { cos(vr[i].faceTo), sin(vr[i].faceTo) };
			double x_himNext = 0.0;
			double y_himNext = 0.0;
			double angle_him = 0.0;
			//double the_dg = (sqrt(pow(vector_self[0], 2) + pow(vector_self[1], 2)) * sqrt(pow(vector_him[0], 2) + pow(vector_him[1], 2)));
			double cos_theta = vector_self[0] * vector_him[0] + vector_self[1] * vector_him[1];
			cos_theta = cos_theta;
			//����н��Ƕ۽��ٽ�����һ���ж�
			if(cos_theta < 0)
			{
				perror("�Ƕ۽�");

				if (vr[i].faceTo > 0 && vr[i].faceTo < 1.5707)
				{
					x_himNext = vr[i].x_loca + vr[i].x_lineSpeed * advanceFrame;
					y_himNext = vr[i].y_loca + vr[i].y_lineSpeed * advanceFrame;
				}
				else if (vr[i].faceTo > 1.5707)
				{
					x_himNext = vr[i].x_loca - vr[i].x_lineSpeed * advanceFrame;
					y_himNext = vr[i].y_loca + vr[i].y_lineSpeed * advanceFrame;
				}
				if (vr[i].faceTo < 0 && vr[i].faceTo > -1.5707)
				{
					x_himNext = vr[i].x_loca + vr[i].x_lineSpeed * advanceFrame;
					y_himNext = vr[i].y_loca - vr[i].y_lineSpeed * advanceFrame;
				}
				else if (vr[i].faceTo < -1.5707)
				{
					x_himNext = vr[i].x_loca - vr[i].x_lineSpeed * advanceFrame;
					y_himNext = vr[i].y_loca - vr[i].y_lineSpeed * advanceFrame;
				}
				double dNextFrame = pow(x_selfNext - x_himNext, 2) + pow(y_selfNext - y_himNext, 2);
				dNextFrame = sqrt(dNextFrame);
				//С��ĳһ������ֵ����Ҫ������
				if (dNextFrame < 1.6)
				{
					//�ж�һ��������߻����ұ�
					if (this->judgeRight(vr[i]))
					{
						// ����Է����Լ��ұߣ� �ͺ���ʱ��ת
						angle_self = 3.0;
					}
					else
						angle_self = -3.0;
					perror("����Ҫת����");
					printf("rotate %d %f\n", this->robotId, angle_self);
					//printf("rotate %d %f\n", vr[i].robotId, 2.0);
				}
			}
			//�����������ڼ��޾���ĺ��� �������С����Ҫ�����ˣ�
			//ʲô�ط���Ҫ����

		}
	}
}


bool Robot::judgeRight(Robot r)
{
	//�����Լ������ϵ�kx+b
	double theta = 0;
	if (this->faceTo > 0)
		theta = this->faceTo;
	else
		theta = this->faceTo + 3.1415926;
	double k = tan(theta);
	double b = this->y_loca - k * this->x_loca;
	//˵������ߣ�������ת����˳ʱ��
	if (this->faceTo > 0)
	{
		if ((r.y_loca > k * r.x_loca + b && k > 0) || (r.y_loca < k * r.x_loca + b && k < 0))
			return false;
		else
			return true;
	}
	else
	{
		if ((r.y_loca > k * r.x_loca + b && k > 0) || (r.y_loca < k * r.x_loca + b && k < 0))
			return true;
		else
			return false;
	}
}
