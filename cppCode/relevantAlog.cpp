#include <iostream>
using namespace std;

#include <math.h>
#include <vector>
#include <set>

#include "robot.h"
#include "staging.h"


double dRobotToStage(Robot r, Staging s)
{
	double distance = pow(r.x_loca - s.x_location, 2) + pow(r.y_loca - s.y_location, 2);
	return sqrt(distance);
}

double dStageToStage(Staging s1, Staging s2)
{
	double distance = pow(s1.x_location - s2.x_location, 2) + pow(s1.y_location - s2.y_location, 2);
	return sqrt(distance);
}


void buyByInts(Robot* r, vector<Staging>& vs, int* num)
{
	if (r->flag_deliver)
	{
		if (r->tagetS.staging_num == num[0])
		{
			//买成功了就换目标2
			if (r->buyGoods(r->tagetS, r->tagetS.staging_num))
			{
				perror("被买的工作台可以重新参与贪心算法了");
				vs[num[0]].flag_waitRobot = false;
				r->tagetS = vs[num[1]];
			}
			//perror("目一直是num1？");
		}
	}
}

void sellByInts(Robot* r, vector<Staging>& vs, int* num)
{
	if (r->flag_deliver)
	{
		if (r->sellGoods(r->tagetS, r->tagetS.staging_num))
			r->flag_deliver = false;
	}
}


void workByInts(Robot* r, vector<Staging>& vs, int* num)
{	
	if (r->flag_deliver)
	{
		if (r->tagetS.staging_num == num[0])
		{
			//买成功后
			if (r->buyGoods(r->tagetS, r->tagetS.staging_num))
			{
				//perror("被买的工作台可以重新参与贪心算法了");
				//被买的工作台释放掉
				vs[num[0]].productEmploy = false;
				vs[num[1]].rawMtlEmploy = true;
				//要卖的工作台被占用

				r->tagetS = vs[num[1]];
				//vs[num[1]].flag_waitRobot = true;
			}
			//perror("目一直是num1？");
		}
		else
		{
			//perror("目标变了吗");
			if (r->sellGoods(r->tagetS, r->tagetS.staging_num))
			{
				vs[num[0]].flag_waitRobot = false;
				vs[num[1]].flag_waitRobot = false;
				vs[num[1]].flag_wait4 = false;
				vs[num[1]].flag_wait5 = false;
				vs[num[1]].flag_wait6 = false;


				r->flag_deliver = false;
			}
		}
	}
}




int getMaxIndexByD(vector<Robot>& vr, Staging& s)
{
	int maxIndex = 0;
	double distance[4] = {};
	for (int i= 0; i< vr.size(); i++)
	{
		distance[i] = dRobotToStage(vr[i], s);
	}
	for (int j = 0; j < vr.size(); j++)
	{
		if (distance[j] > distance[maxIndex])
			maxIndex = j;
	}
	return maxIndex;
}





//手上没有物品的贪心算法
void withoutGoodsGreedyAlg(Robot* r, vector<Staging>& vs, int* tagets)
{
	//workByInts(r, vs, tagets);

	//如果机器人手上不为空，或者有传送任务
	if (r->take_type != 0 || r->flag_deliver)
		return;
	//最大价值，用于后面的比较
	double maxValue = 0.0;
	double weight4 = 1.0;
	double weight5 = 1.0;
	double weight6 = 1.0;
	//用于判断是否有路径选择，如果最后相等， 就说明没有
	int judgeNum = 0;
	for (vector<Staging>::iterator it_halfway = vs.begin(); it_halfway != vs.end(); it_halfway++)
	{
		//如果中途工作台生产好了，且没有需要等待机器人，就可以分配终点工作台了
		if ((*it_halfway).product_status == 1 && !(*it_halfway).flag_waitRobot)
		{
			//遍历所有工作台，缺中途工作台生产好的 作为终点工作台(确定)
			for (vector<Staging>::iterator it_end = vs.begin(); it_end != vs.end(); it_end++)
			{

				//如果end工作台缺的正是halfway工作台所生产的，end工作台不可能是123
				if (
					(*it_halfway).staging_id == 1 && (*it_end).is_lack1() && !(*it_end).flag_waitRobot ||
					(*it_halfway).staging_id == 2 && (*it_end).is_lack2() && !(*it_end).flag_waitRobot ||
					(*it_halfway).staging_id == 3 && (*it_end).is_lack3() && !(*it_end).flag_waitRobot ||
					(*it_halfway).staging_id == 4 && (*it_end).is_lack4() && !(*it_end).flag_waitRobot ||
					(*it_halfway).staging_id == 5 && (*it_end).is_lack5() && !(*it_end).flag_waitRobot ||
					(*it_halfway).staging_id == 6 && (*it_end).is_lack6() && !(*it_end).flag_waitRobot ||
					(*it_halfway).staging_id == 7 && (*it_end).is_lack7() && !(*it_end).flag_waitRobot 
					)
				{
					double distance1 = dRobotToStage(*r, *it_halfway);
					double distance2 = dStageToStage(*it_halfway, *it_end);
					double distance = distance1 + distance2;
					double value = 0.0;
					switch ((*it_halfway).staging_id)
					{
					case 1:
						value = 3000.0;
						//perror("进来了");
						break;
					case 2:
						value = 3200.0;
						break;
					case 3:
						value = 3400.0;
						break;
					case 4:
						value = 7100.0 * weight4;
						break;
					case 5:
						value = 7800.0 * weight5;
						break;
					case 6:
						value = 8300.0 * weight6;
						break;
					case 7:
						value = 29000.0;
						break;
					default:
						break;
					}
					//如果中途工作台是4，end工作台只缺4，权重增大
					if ((*it_halfway).staging_id == 4 && (*it_end).staging_id == 7 &&
						(*it_end).is_lack4() && !((*it_end).is_lack5()) && !((*it_end).is_lack6()))
					{
						value += 29000.0;
						distance += 6.0 * (*it_end).time_remain;
					}
					/*if ((*it_halfway).staging_id == 5 && (*it_end).staging_id == 7 &&
						(*it_end).is_lack5() && !((*it_end).is_lack4()) && !((*it_end).is_lack6()))
					{
						value += 29000.0;
						distance += 6.0 * (*it_end).time_remain;
					}*/
					/*if ((*it_halfway).staging_id == 6 && (*it_end).staging_id == 7 &&
						(*it_end).is_lack6() && !((*it_end).is_lack4()) && !((*it_end).is_lack5()))
					{
						value += 29000.0;
						distance += 6.0 * (*it_end).time_remain;
					}*/

					double averValue = value / (distance1 + distance2);
					if (averValue > maxValue)
					{
						maxValue = averValue;
						tagets[0] = (*it_halfway).staging_num;
						tagets[1] = (*it_end).staging_num;
						judgeNum++;
					}
				}

			}
			if(judgeNum != 0)
			{
				/*if (vs[tagets[1]].staging_id == 7)
				{
					if (vs[tagets[0]].staging_id == 4)
						vs[tagets[1]].flag_wait4 = true;
					if (vs[tagets[0]].staging_id == 5)
						vs[tagets[1]].flag_wait5 = true;
					if (vs[tagets[0]].staging_id == 6)
						vs[tagets[1]].flag_wait6 = true;
				}*/
				r->tagetS = vs[tagets[0]];
				//------------------------------------------------------------------------------------------
				//vs[tagets[0]].flag_waitRobot = true;

				r->flag_deliver = true;
				r->flag_wait = false;
			}
			else
			{
				//Staging s(-1, -1, r->x_loca, r->y_loca);
				//r->tagetS = s;
				r->flag_wait = true;
			}
			//if (vs[tagets[0]].staging_id != 8 && vs[tagets[0]].staging_id != 9)
				//vs[tagets[0]].flag_waitRobot = true;
			//if (vs[tagets[0]].staging_id != 8 && vs[tagets[0]].staging_id != 9)
				//vs[tagets[1]].flag_waitRobot = true;
		}	
	}	
}


void withoutGoodsGreedyOnlyOne7(Robot* r, vector<Staging>& vs, int* tagets)
{
	if (r->take_type != 0 || r->flag_deliver)
		return;
	//最大价值，用于后面的比较
	double maxValue = 0.0;
	double weight4 = 1.0;
	double weight5 = 1.0;
	double weight6 = 1.0;
	//用于判断是否有路径选择，如果最后相等， 就说明没有
	int judgeNum = 0;
	for (vector<Staging>::iterator it_halfway = vs.begin(); it_halfway != vs.end(); it_halfway++)
	{
		//如果中途工作台生产好了，且没有需要等待机器人，就可以分配终点工作台了
		if ((*it_halfway).product_status == 1 && !(*it_halfway).flag_waitRobot)
		{
			//遍历所有工作台，缺中途工作台生产好的 作为终点工作台(确定)
			for (vector<Staging>::iterator it_end = vs.begin(); it_end != vs.end(); it_end++)
			{
				if ((*it_halfway).staging_id == 4 && (*it_end).staging_id == 7 && (*it_end).product_status == 1)
					weight4 = 4.2;
				if ((*it_halfway).staging_id == 5 && (*it_end).staging_id == 7 && (*it_end).product_status == 1)
					weight5 = 4.1;
				if ((*it_halfway).staging_id == 6 && (*it_end).staging_id == 7 && (*it_end).product_status == 1)
					weight6 = 4.0;
				//如果end工作台缺的正是halfway工作台所生产的，end工作台不可能是123
				if (
					(*it_halfway).staging_id == 1 && (*it_end).is_lack1() && !(*it_end).flag_waitRobot ||
					(*it_halfway).staging_id == 2 && (*it_end).is_lack2() && !(*it_end).flag_waitRobot ||
					(*it_halfway).staging_id == 3 && (*it_end).is_lack3() && !(*it_end).flag_waitRobot ||
					(*it_halfway).staging_id == 4 && (*it_end).is_lack4() && !(*it_end).flag_waitRobot ||
					(*it_halfway).staging_id == 5 && (*it_end).is_lack5() && !(*it_end).flag_waitRobot ||
					(*it_halfway).staging_id == 6 && (*it_end).is_lack6() && !(*it_end).flag_waitRobot ||
					(*it_halfway).staging_id == 7 && (*it_end).is_lack7() && !(*it_end).flag_waitRobot ||
					(*it_halfway).staging_id == 4 && (*it_end).is_lack4() && !(*it_end).flag_wait4 ||
					(*it_halfway).staging_id == 5 && (*it_end).is_lack5() && !(*it_end).flag_wait5 ||
					(*it_halfway).staging_id == 6 && (*it_end).is_lack6() && !(*it_end).flag_wait6 
					)
				{
					double distance1 = dRobotToStage(*r, *it_halfway);
					double distance2 = dStageToStage(*it_halfway, *it_end);
					double distance = distance1 + distance2;
					double value = 0.0;
					switch ((*it_halfway).staging_id)
					{
					case 1:
						value = 3000.0;
						//perror("进来了");
						break;
					case 2:
						value = 3200.0;
						break;
					case 3:
						value = 3400.0;
						break;
					case 4:
						value = 7100.0 * weight4;
						break;
					case 5:
						value = 7800.0 * weight5;
						break;
					case 6:
						value = 8300.0 * weight6;
						break;
					case 7:
						value = 29000.0;
						break;
					default:
						break;
					}
					//如果中途工作台是4，end工作台只缺4，权重增大
					if ((*it_halfway).staging_id == 4 && (*it_end).staging_id == 7 &&
						(*it_end).is_lack4() && !((*it_end).is_lack5()) && !((*it_end).is_lack6()))
					{
						value += 29000.0;
						distance += 6.0 * (*it_end).time_remain;
					}
					/*if ((*it_halfway).staging_id == 5 && (*it_end).staging_id == 7 &&
						(*it_end).is_lack5() && !((*it_end).is_lack4()) && !((*it_end).is_lack6()))
					{
						value += 29000.0;
						distance += 6.0 * (*it_end).time_remain;
					}*/
					/*if ((*it_halfway).staging_id == 6 && (*it_end).staging_id == 7 &&
						(*it_end).is_lack6() && !((*it_end).is_lack4()) && !((*it_end).is_lack5()))
					{
						value += 29000.0;
						distance += 6.0 * (*it_end).time_remain;
					}*/

					double averValue = value / (distance1 + distance2);
					if (averValue > maxValue)
					{
						maxValue = averValue;
						tagets[0] = (*it_halfway).staging_num;
						tagets[1] = (*it_end).staging_num;
						judgeNum++;
					}
				}

			}
			if (judgeNum != 0)
			{

				r->tagetS = vs[tagets[0]];
				//------------------------------------------------------------------------------------------
				//vs[tagets[0]].flag_waitRobot = true;

				r->flag_deliver = true;
				r->flag_wait = false;
			}
			else
			{
				//Staging s(-1, -1, r->x_loca, r->y_loca);
				//r->tagetS = s;
				r->flag_wait = true;
			}
		}
	}
}




bool notEnoughTime(Robot& r, vector<Staging>& vs, int frameID)
{
	double remTime = frameID * 20.0;		//单位是ms
	double time = 0;
	//时间来不及买卖就不买了。
	if (r.take_type == 0)
	{
		//perror("here?");
		double distance1 = dRobotToStage(r, vs[r.tagetsNum[0]]);
		double distance2 = dStageToStage(vs[r.tagetsNum[0]], vs[r.tagetsNum[1]]);
		//前进时间
		time = (distance1 + distance2) / 6.0;
		//加上转弯时间
		double vector_1[2] = { vs[r.tagetsNum[0]].x_location - r.x_loca, vs[r.tagetsNum[0]].y_location - r.y_loca };
		double vector_2[2] = { vs[r.tagetsNum[1]].x_location - vs[r.tagetsNum[0]].x_location, vs[r.tagetsNum[1]].y_location - vs[r.tagetsNum[0]].y_location };
		double the_dg = (sqrt(pow(vector_1[0], 2) + pow(vector_1[1], 2)) * sqrt(pow(vector_2[0], 2) + pow(vector_2[1], 2)));
		double cos_theta = vector_1[0] * vector_2[0] + vector_1[1] * vector_2[1];
		cos_theta = cos_theta / the_dg;
		double theta = acos(cos_theta);
		time = time + (theta / 3.0);
		time = time * 1000.0;
	}
	if (time > remTime)
	{
		perror("来不及了，不买了");
		return true;		
	}		
	else
	{
		//perror("还来的及");
		return false;
	}

}






//到工作台4，此时7恰好缺4，顺带放过去
void greedyOnlyFor7()
{

}




void takeGoodsGreedyAlg(Robot* r, vector<Staging>& vs, int* tagets)
{
	if(r->take_type != 0 && !r->flag_deliver)
	{
		double maxValue = 0.0;
		double value = 0.0;
		double distance = 0.0;
		for (vector<Staging>::iterator it = vs.begin(); it != vs.end(); it++)
		{
			if (r->take_type == 1 && (*it).is_lack1())
				value = 3000 * r->time_value * r->collision_value;
			else if (r->take_type == 2 && (*it).is_lack2())
				value = 3200 * r->time_value * r->collision_value;
			else if (r->take_type == 3 && (*it).is_lack3())
				value = 3400 * r->time_value * r->collision_value;
			distance = dRobotToStage(*r, *it);
			double averValue = value / distance;
			if (averValue > maxValue)
			{
				maxValue = averValue;
				tagets[1] = (*it).staging_num;
				/*tagets[0] = (*it_halfway).staging_num;
				tagets[1] = (*it_end).staging_num;*/
			}
		}
		r->tagetS = vs[tagets[1]];
		r->flag_deliver = true;
	}
}





