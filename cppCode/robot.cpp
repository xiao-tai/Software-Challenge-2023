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

	//朝向不是end，转向至end
	double deta_x = end_x - begin_x;
	double deta_y = end_y - begin_y;
	double distance = pow(deta_x, 2) + pow(deta_y, 2);
	if (distance < 0.1)
	{
		//一开始目标是自己，无事发生
		//printf("rotate %d %f\n", this->robotId, 1.0);
		return;
	}

	double vector_a[2] = { deta_x, deta_y };
	double vector_e[2] = { cos(this->faceTo), sin(this->faceTo) };
	//计算分母
	double the_dg = (sqrt(pow(vector_a[0], 2) + pow(vector_a[1], 2)) * sqrt(pow(vector_e[0], 2) + pow(vector_e[1], 2)));
	//计算sin theta
	double sin_theta = vector_a[0] * vector_e[1] - vector_a[1] * vector_e[0];
	sin_theta = sin_theta / the_dg;
	//计算cos theta
	double cos_theta = vector_a[0] * vector_e[0] + vector_a[1] * vector_e[1];
	cos_theta = cos_theta / the_dg;

	//a叉乘e，sintheta大于零，反转
	if (sin_theta > 0)
	{
		//反转
		//角度比较小，转慢一点		
		if (cos_theta > 0.95) {
			angleSpeed = -0.4;
		}
		else {
			angleSpeed = -3.1415926 * angleWeight;
		}
	}
	else
	{
		//theta过大，转快
		if (cos_theta > 0.95)
		{
			angleSpeed = 0.4;
		}
		else
		{
			angleSpeed = 3.1415926 * angleWeight;
		}
	}
	//判断正反
	//位置不是end，前进至end
	//离工作台近或者离边界近，移速降低
	

	if (cos_theta > 0.50)
		lineSpeed = 6;
		/*else if (cos_theta > 0.5)
		lineSpeed = 5;*/
		//夹角大的时候速度稍微慢点
	else if (cos_theta > 0.3)
		lineSpeed = 3 / angleWeight;		//最初版本是1
	else
		lineSpeed = 0.6;

	//如果工作台离边界比较近，就靠近他时减速
	if (this->tagetS.x_location < 5 || this->tagetS.x_location > 45 || this->tagetS.y_location < 5 || this->tagetS.y_location > 45)
	{
		if (distance < 4)
			lineSpeed = 3;
		if (cos_theta < 0.8)
			lineSpeed = 1;
	}

	//判断朝向与墙壁的关系
	//平行 就正常速度
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
	//这个机器人  提前五帧判断，乘0.06
	double advanceFrame = 0.3;
	//根据转向来
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
		//除自己以外，对于其他人进行避障
		if (i != index)
		{
			double vector_him[2] = { cos(vr[i].faceTo), sin(vr[i].faceTo) };
			double x_himNext = 0.0;
			double y_himNext = 0.0;
			double angle_him = 0.0;
			//double the_dg = (sqrt(pow(vector_self[0], 2) + pow(vector_self[1], 2)) * sqrt(pow(vector_him[0], 2) + pow(vector_him[1], 2)));
			double cos_theta = vector_self[0] * vector_him[0] + vector_self[1] * vector_him[1];
			cos_theta = cos_theta;
			//如果夹角是钝角再进行下一步判断
			if(cos_theta < 0)
			{
				perror("是钝角");

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
				//小于某一个极限值，需要避障了
				if (dNextFrame < 1.6)
				{
					//判断一下在我左边还是右边
					if (this->judgeRight(vr[i]))
					{
						// 如果对方在自己右边， 就好逆时针转
						angle_self = 3.0;
					}
					else
						angle_self = -3.0;
					perror("咱俩要转弯了");
					printf("rotate %d %f\n", this->robotId, angle_self);
					//printf("rotate %d %f\n", vr[i].robotId, 2.0);
				}
			}
			//避障条件，在极限距离的后面 距离会缩小，就要避障了，
			//什么地方不要避障

		}
	}
}


bool Robot::judgeRight(Robot r)
{
	//构建自己方向上的kx+b
	double theta = 0;
	if (this->faceTo > 0)
		theta = this->faceTo;
	else
		theta = this->faceTo + 3.1415926;
	double k = tan(theta);
	double b = this->y_loca - k * this->x_loca;
	//说明在左边，所以右转——顺时针
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
