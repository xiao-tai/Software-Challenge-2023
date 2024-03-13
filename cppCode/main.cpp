#include <iostream>
using namespace std;

#include <math.h>
#include <string> 
#include <vector>
#include <map>
#include <stack>
#include <deque>


#include "robot.h"
#include "staging.h"
#include "relevantALog.h"

//#include "staging.cpp"
//#include "robot.cpp"
//#include "relevantALog.cpp"


//void callRobotTo789(Robot* pr, vector<Staging>* s7, vector<Staging>* s8, vector<Staging>* s9);
void ergodicStage(Robot* r, vector<Staging>* vStaging, bool minToMax);
void completeJob(Robot* r);
void othersTakeThe(Robot* r, vector<Staging>& vs, int* num);
int returnStage7(vector<Staging> vs);
//void workByInts(Robot* r, vector<Staging> vs, int* num);

bool readUntilOK() {
    char line[1024];
    while (fgets(line, sizeof line, stdin)) {
        if (line[0] == 'O' && line[1] == 'K') {
            return true;
        }
        //do something---进行初始化
        
    }
    return false;
}

bool readMap(vector<Staging>* vStaging, vector<Robot>* vRobot)
{
    char line[1024];
    int num = 0;    //Y坐标
    int robotId = 0;    //机器人ID
    int num_stage = 0;
    while (fgets(line, sizeof line, stdin)) {
        if (line[0] == 'O' && line[1] == 'K') {
            return true;
        }
        //do something---进行初始化
        int i = 0;
        while (line[i] != '\n')
        {   
            // 遇到地图中的机器人
            if (line[i] == 'A')
            {
                // 获得机器人编号和位置信息
                Robot* r = new Robot(robotId, 0.25 + 0.5 * i, 49.75 - 0.5 * num);
                vRobot->push_back(*r);
                robotId++;
                //cout << "机器人Y坐标：" << i * 0.25 << endl;
            }
            else if (line[i] != '.')
            {
                // 获取工作台的类型和位置
                Staging* s = new Staging((int)line[i] - 48, num_stage, 0.25 + 0.5 * i, 49.75 - 0.5 * num);
                vStaging->push_back(*s);
                num_stage++;
            }
            i++;
        }
        num++;
    }
    return false;
}

bool readInfo(vector<Robot>* r, vector<Staging>* s) 
{
    char line[1024];
    vector<string> input_code;
    //int num = 0;    //记录输入流的行数
    while (fgets(line, sizeof line, stdin)) {
        if (line[0] == 'O' && line[1] == 'K') {
            //返回之前 进行分析
            vector<string>::iterator it_code = input_code.begin() + 2;
            vector<Robot>::iterator it_r = r->begin();
            vector<Staging>::iterator it_s = s->begin();
            for (int i = 0; i < s->size(); i++, it_code++)
            {
                //读取工作台信息
                (*it_s).codeToInfo(*it_code);
                it_s++;
            }
            for (int j = 0; j < r->size(); j++, it_code++)
            {
                //读取机器人信息
                (*it_r).codeToInfo(*it_code);
                it_r++;
            }
            return true;
        }
        string str(line);
        input_code.push_back(str);      
    }
    return false;
}

void classStaging(vector<Staging>* s, vector<Staging>* s1, vector<Staging>* s4,
    vector<Staging>* s5, vector<Staging>* s6, vector<Staging>* s7, vector<Staging>* s8, vector<Staging>* s9)
{
    vector<Staging>::iterator it_s = s->begin();
    for (; it_s != s->end(); it_s++)
    {
        if ((*it_s).staging_id == 1 || (*it_s).staging_id == 2 || (*it_s).staging_id == 3)
        {
            s1->push_back(*it_s);
        }
        else if ((*it_s).staging_id == 4)
            s4->push_back(*it_s);
        else if ((*it_s).staging_id == 5)
            s5->push_back(*it_s);
        else if ((*it_s).staging_id == 6)
            s6->push_back(*it_s);
        else if ((*it_s).staging_id == 7)
            s7->push_back(*it_s);
        else if ((*it_s).staging_id == 8)
            s8->push_back(*it_s);
        else if ((*it_s).staging_id == 9)
            s9->push_back(*it_s);
    }
}



int main() {

    vector<Robot> vRobot;
    vector<Staging> vStaging;
    vector<Staging> vStaging_No1;
    vector<Staging> vStaging_No4;
    vector<Staging> vStaging_No5;
    vector<Staging> vStaging_No6;
    vector<Staging> vStaging_No7;
    vector<Staging> vStaging_No8;
    vector<Staging> vStaging_No9;

    //deque<Staging> dStageLoc1;
    //deque<Staging> dStageLoc2;
    //deque<Staging> dStageLoc3;
    //deque<Staging> dStageLoc4;
    
    bool all_lack = false;
    bool lack_4 = false;
    bool lack_5 = false;
    bool lack_6 = false;

    bool lack_1 = true;
    bool lack_2 = true;
    bool lack_3 = true;

 
    //vector<string> r_code;
    //获取地图信息，创建具有坐标的对象
    readMap(&vStaging, &vRobot);
    //对工作台进行分类
    classStaging(&vStaging, &vStaging_No1, &vStaging_No4, &vStaging_No5, &vStaging_No6, &vStaging_No7, &vStaging_No8, &vStaging_No9);
    
    //根据距离等信息制定初始策略，制定完成，输出OK
    //downProject(&vRobot, &vStaging, &vStaging_No1);
    //classStageByLoc(&vStaging, &dStageLoc1, &dStageLoc2, &dStageLoc3, &dStageLoc4);
    
    //double anglespeed = testDistance(&vRobot, &vStaging);
    //readUntilOK();
    puts("OK");
    fflush(stdout);
    int frameID = 0;
    int remFrameID = 0;
    //double angleSpeed = 0;

    bool onlyOne7 = false;

    //测试代码
    int tagetForInt0[2] = { 0, 2 };
    int tagetForInt1[2] = { 1, 0 };
    int tagetForInt2[2] = { 2, 0 };
    int tagetForInt3[2] = { 3, 0 };
    //vector<Robot>::iterator test = vRobot.begin();
    //(*test).tagetS = vStaging[tagetSFor0[0]];
    //(*test).flag_deliver = true;

    int numberOf7 = returnStage7(vStaging);
    if (numberOf7 == 1)
        onlyOne7 = true;

    int endNumber = 0;


    while (scanf("%d", &frameID) != EOF) {
        readInfo(&vRobot, &vStaging);  // 从第二行开始读，读取的信息用来调整机器人的命令
        classStaging(&vStaging, &vStaging_No1, &vStaging_No4, &vStaging_No5, &vStaging_No6, &vStaging_No7, &vStaging_No8, &vStaging_No9);
        //classStageByLoc(&vStaging, &dStageLoc1, &dStageLoc2, &dStageLoc3, &dStageLoc4);
        printf("%d\n", frameID);    //必须的
        // 输出对机器人的命令
        
        //实时更新 机器人的目标工作台状态
        //for (vector<Robot>::iterator it_r2 = vRobot.begin(); it_r2 != vRobot.end(); it_r2++)
        //{
        //    for (vector<Staging>::iterator it_s2 = vStaging.begin(); it_s2 != vStaging.end(); it_s2++)
        //    {
        //        if ((*it_r2).tagetS.staging_num == (*it_s2).staging_num)
        //        {
        //            (*it_r2).tagetS = *it_s2;
        //            //perror("匹配成功");
        //        }
        //            
        //    }
        //}
        //if (!vStaging_No7.empty())
        //{
        //    for (vector<Staging>::iterator it_s = vStaging_No7.begin(); it_s != vStaging_No7.end(); it_s++)
        //    {
        //        endNumber = (*it_s).staging_num;
        //        // 如果缺4
        //        if (it_s->raw_material_status == 96 || it_s->raw_material_status == 64 ||
        //            it_s->raw_material_status == 32 || it_s->raw_material_status == 0)
        //            lack_4 = true;
        //        else
        //            lack_4 = false;
        //        if (it_s->raw_material_status == 80 || it_s->raw_material_status == 64 ||
        //            it_s->raw_material_status == 16 || it_s->raw_material_status == 0)
        //            lack_5 = true;
        //        else
        //            lack_5 = false;
        //        if (it_s->raw_material_status == 48 || it_s->raw_material_status == 32 ||
        //            it_s->raw_material_status == 16 || it_s->raw_material_status == 0)
        //            lack_6 = true;
        //        else
        //            lack_6 = false;
        //    }
        //}
        //else
        //{
        //    all_lack = true;
        //}
   

        
        //大于50帧的时候，程序才开始，防止第一次把num工作台至false
        if (frameID > 50)
        {
            /*if (vStaging[0].flag_waitRobot)
                perror("凭什么不去第一个工作台");*/
            /*if (vStaging[1].flag_waitRobot)
                perror("凭什么不去第二个工作台");
            if (vStaging[2].flag_waitRobot)
                perror("凭什么不去第三个工作台");
            if (vStaging[3].flag_waitRobot)
                perror("凭什么不去第四个工作台");*/

            //less7 = false;

            /*if (vStaging[9].flag_waitRobot && vStaging[2].flag_waitRobot)
                perror("第九号和第2号工作台不能进入算法了");
            if (vStaging[9].flag_waitRobot)
                perror("九号工作台不能进入算法");
            if (vStaging[1].flag_waitRobot)
                perror("1号不能进入算法");
            if (vStaging[10].flag_waitRobot)
                perror("10号不能进入算法");

            if (vStaging[12].flag_waitRobot)
                perror("12号不能进入算法了");*/
            if(!onlyOne7)
            {
                //perror("????");
                remFrameID = 9000 - frameID;
                for (vector<Robot>::iterator it = vRobot.begin(); it != vRobot.end(); it++)
                {
                    withoutGoodsGreedyAlg(&*it, vStaging, (*it).tagetsNum);
                    vStaging[(*it).tagetsNum[0]].flag_waitRobot = true;
                    //if(vStaging[(*it).tagetsNum[1]].staging_id != 7 || !less7)
                    if(vStaging[(*it).tagetsNum[1]].staging_id != 9)
                        vStaging[(*it).tagetsNum[1]].flag_waitRobot = true;
                    /*if (vStaging[(*it).tagetsNum[1]].staging_id == 7)
                    {
                        if (vStaging[(*it).tagetsNum[0]].staging_id == 4)
                            vStaging[(*it).tagetsNum[1]].flag_wait4 = true;
                        if (vStaging[(*it).tagetsNum[0]].staging_id == 5)
                            vStaging[(*it).tagetsNum[1]].flag_wait5 = true;
                        if (vStaging[(*it).tagetsNum[0]].staging_id == 6)
                            vStaging[(*it).tagetsNum[1]].flag_wait6 = true;
                    }*/

                    if (!notEnoughTime(*it, vStaging, remFrameID))
                        workByInts(&*it, vStaging, (*it).tagetsNum);
                }
                for (vector<Robot>::iterator it_r1 = vRobot.begin(); it_r1 != vRobot.end(); it_r1++)
                {
                    (*it_r1).locToLoc(1.0);
                    completeJob(&*it_r1);
                    //(*it_r1).avoidCollision(vRobot);
                }
            }
            else
            {
                remFrameID = 9000 - frameID;
                // 靠近7先做决策
                /*int index = getMaxIndexByD(vRobot, *vStaging_No7.begin());
                withoutGoodsGreedyOnlyOne7(&vRobot[index], vStaging, vRobot[index].tagetsNum);
                vStaging[vRobot[index].tagetsNum[0]].flag_waitRobot = true;

                vStaging[vRobot[index].tagetsNum[1]].flag_waitRobot = true;

                if (vStaging[vRobot[index].tagetsNum[1]].staging_id == 7)
                {
                    if (vStaging[vRobot[index].tagetsNum[0]].staging_id == 4)
                        vStaging[vRobot[index].tagetsNum[1]].flag_wait4 = true;
                    if (vStaging[vRobot[index].tagetsNum[0]].staging_id == 5)
                        vStaging[vRobot[index].tagetsNum[1]].flag_wait5 = true;
                    if (vStaging[vRobot[index].tagetsNum[0]].staging_id == 6)
                        vStaging[vRobot[index].tagetsNum[1]].flag_wait6 = true;
                }
                if (!notEnoughTime(vRobot[index], vStaging, remFrameID))
                    workByInts(&vRobot[index], vStaging, vRobot[index].tagetsNum);

                int i = 0;
                for (vector<Robot>::iterator it = vRobot.begin(); it != vRobot.end(); it++, i++)
                {
                    if(i != index)
                    {
                        withoutGoodsGreedyOnlyOne7(&*it, vStaging, (*it).tagetsNum);

                        vStaging[(*it).tagetsNum[0]].flag_waitRobot = true;

                        vStaging[(*it).tagetsNum[1]].flag_waitRobot = true;

                        if (vStaging[(*it).tagetsNum[1]].staging_id == 7)
                        {
                            if (vStaging[(*it).tagetsNum[0]].staging_id == 4)
                                vStaging[(*it).tagetsNum[1]].flag_wait4 = true;
                            if (vStaging[(*it).tagetsNum[0]].staging_id == 5)
                                vStaging[(*it).tagetsNum[1]].flag_wait5 = true;
                            if (vStaging[(*it).tagetsNum[0]].staging_id == 6)
                                vStaging[(*it).tagetsNum[1]].flag_wait6 = true;
                        }
                        if (!notEnoughTime(*it, vStaging, remFrameID))
                            workByInts(&*it, vStaging, (*it).tagetsNum);
                    }
                }*/
                vector<Robot>::iterator it = vRobot.begin();
                if(frameID % 4 == 0)
                {
                    withoutGoodsGreedyOnlyOne7(&*it, vStaging, (*it).tagetsNum);

                    vStaging[(*it).tagetsNum[0]].flag_waitRobot = true;

                    vStaging[(*it).tagetsNum[1]].flag_waitRobot = true;

                    if (vStaging[(*it).tagetsNum[1]].staging_id == 7)
                    {
                        if (vStaging[(*it).tagetsNum[0]].staging_id == 4)
                            vStaging[(*it).tagetsNum[1]].flag_wait4 = true;
                        if (vStaging[(*it).tagetsNum[0]].staging_id == 5)
                            vStaging[(*it).tagetsNum[1]].flag_wait5 = true;
                        if (vStaging[(*it).tagetsNum[0]].staging_id == 6)
                            vStaging[(*it).tagetsNum[1]].flag_wait6 = true;
                    }
                    if (!notEnoughTime(*it, vStaging, remFrameID))
                        workByInts(&*it, vStaging, (*it).tagetsNum);

                }
                if(frameID % 4 == 1)
                {
                    it += 1;
                    withoutGoodsGreedyOnlyOne7(&*it, vStaging, (*it).tagetsNum);

                    vStaging[(*it).tagetsNum[0]].flag_waitRobot = true;

                    vStaging[(*it).tagetsNum[1]].flag_waitRobot = true;

                    if (vStaging[(*it).tagetsNum[1]].staging_id == 7)
                    {
                        if (vStaging[(*it).tagetsNum[0]].staging_id == 4)
                            vStaging[(*it).tagetsNum[1]].flag_wait4 = true;
                        if (vStaging[(*it).tagetsNum[0]].staging_id == 5)
                            vStaging[(*it).tagetsNum[1]].flag_wait5 = true;
                        if (vStaging[(*it).tagetsNum[0]].staging_id == 6)
                            vStaging[(*it).tagetsNum[1]].flag_wait6 = true;
                    }
                    if (!notEnoughTime(*it, vStaging, remFrameID))
                        workByInts(&*it, vStaging, (*it).tagetsNum);
                }
                if (frameID % 4 == 2)
                {
                    it += 2;
                    withoutGoodsGreedyOnlyOne7(&*it, vStaging, (*it).tagetsNum);

                    vStaging[(*it).tagetsNum[0]].flag_waitRobot = true;

                    vStaging[(*it).tagetsNum[1]].flag_waitRobot = true;

                    if (vStaging[(*it).tagetsNum[1]].staging_id == 7)
                    {
                        if (vStaging[(*it).tagetsNum[0]].staging_id == 4)
                            vStaging[(*it).tagetsNum[1]].flag_wait4 = true;
                        if (vStaging[(*it).tagetsNum[0]].staging_id == 5)
                            vStaging[(*it).tagetsNum[1]].flag_wait5 = true;
                        if (vStaging[(*it).tagetsNum[0]].staging_id == 6)
                            vStaging[(*it).tagetsNum[1]].flag_wait6 = true;
                    }
                    if (!notEnoughTime(*it, vStaging, remFrameID))
                        workByInts(&*it, vStaging, (*it).tagetsNum);
                }
                if (frameID % 4 == 3)
                {
                    it += 3;
                    withoutGoodsGreedyOnlyOne7(&*it, vStaging, (*it).tagetsNum);

                    vStaging[(*it).tagetsNum[0]].flag_waitRobot = true;

                    vStaging[(*it).tagetsNum[1]].flag_waitRobot = true;

                    if (vStaging[(*it).tagetsNum[1]].staging_id == 7)
                    {
                        if (vStaging[(*it).tagetsNum[0]].staging_id == 4)
                            vStaging[(*it).tagetsNum[1]].flag_wait4 = true;
                        if (vStaging[(*it).tagetsNum[0]].staging_id == 5)
                            vStaging[(*it).tagetsNum[1]].flag_wait5 = true;
                        if (vStaging[(*it).tagetsNum[0]].staging_id == 6)
                            vStaging[(*it).tagetsNum[1]].flag_wait6 = true;
                    }
                    if (!notEnoughTime(*it, vStaging, remFrameID))
                        workByInts(&*it, vStaging, (*it).tagetsNum);
                }
                for (vector<Robot>::iterator it_r1 = vRobot.begin(); it_r1 != vRobot.end(); it_r1++)
                {
                    (*it_r1).locToLoc(1.0);
                    completeJob(&*it_r1);
                    (*it_r1).avoidCollision(vRobot);
                }
            }
        }       

        //for (vector<Robot>::iterator it_r1 = vRobot.begin(); it_r1 != vRobot.end(); it_r1++)
        //{
        //    (*it_r1).locToLoc(1.0);
        //    completeJob(&*it_r1);
        //    (*it_r1).avoidCollision(vRobot);
        //    //如果少了
        //}
        





        printf("OK\n");
        fflush(stdout);
    }
        

    //输入流第一行被scanf用掉，后面fgets从第二行开始
    //while (scanf("%d", &frameID) != EOF) {
    //    readUntilOK();
    //    printf("%d\n", frameID);    //必须的
    //    int lineSpeed = 3;
    //    double angleSpeed = 1.5;
    //    for(int robotId = 0; robotId < 4; robotId++){
    //        printf("forward %d %d\n", robotId, lineSpeed);
    //        printf("rotate %d %f\n", robotId, angleSpeed);
    //        //printf("buy %d\n", robotId);
    //    }
    //    printf("OK\n", frameID);
    //    fflush(stdout);
    //}
    return 0;
}


//返回7工作台的数量
int returnStage7(vector<Staging> vs)
{
    int num = 0;
    for (vector<Staging>::iterator it = vs.begin(); it != vs.end(); it++)
    {
        if ((*it).staging_id == 7)
            num++;
    }
    return num;
}














// 先对789 工作台遍历
void ergodicStage(Robot* r, vector<Staging>* vStaging, bool minToMax)
{
    if(minToMax)
    {
        for (vector<Staging>::iterator it_s = vStaging->begin(); it_s != vStaging->end(); it_s++)
        {
            if ((*it_s).staging_id != 1 && (*it_s).staging_id != 2 && (*it_s).staging_id != 3 && !(*it_s).deliver)
            {
               
                (*it_s).callRobotTo789(r);              
                //(*it_s).callRobotTo456(&(*it_r2));
            }
        }
        for (vector<Staging>::iterator it_s = vStaging->begin(); it_s != vStaging->end(); it_s++)
        {
            if ((*it_s).staging_id != 1 && (*it_s).staging_id != 2 && (*it_s).staging_id != 3 && !(*it_s).deliver)
            {
                
                (*it_s).callRobotTo456(r);
                //(*it_s).callRobotTo456(&(*it_r2));
            }
        }
    }
    else
    {
        for (vector<Staging>::iterator it_s = vStaging->begin(); it_s != vStaging->end(); it_s++)
        {
            if ((*it_s).staging_id != 1 && (*it_s).staging_id != 2 && (*it_s).staging_id != 3)
            {

                (*it_s).callRobotTo789(r);
                //(*it_s).callRobotTo456(&(*it_r2));
            }
        }
        for (vector<Staging>::iterator it_s = vStaging->end() - 1; it_s != vStaging->begin(); it_s--)
        {
            if ((*it_s).staging_id != 1 && (*it_s).staging_id != 2 && (*it_s).staging_id != 3 && !(*it_s).deliver)
            {
                
                (*it_s).callRobotTo456(r);

                //(*it_s).callRobotTo456(&(*it_r2));
            }
        }
    }
}

//void workByInts(Robot* r, vector<Staging> vs, int* num)
//{
//    if (r->flag_deliver)
//    {
//        if (r->tagetS.staging_num == num[0])
//        {
//            //买成功了就换目标2
//            if (r->buyGoods(r->tagetS, r->tagetS.staging_num))
//            {
//                perror("被买的工作台可以重新参与贪心算法了");
//                r->tagetS.flag_waitRobot = false;
//                r->tagetS = vs[num[1]];
//            }
//            //perror("目一直是num1？");
//        }
//        else
//        {
//            //perror("目标变了吗");
//            if (r->sellGoods(r->tagetS, r->tagetS.staging_num))
//            {
//                r->tagetS.flag_waitRobot = false;
//                r->flag_deliver = false;
//            }
//        }
//    }
//
//}

//其他机器人抢先拿走生产好的物品，就至flag为false
void othersTakeThe(Robot* r, vector<Staging>& vs, int* num)
{
    //工作但是空手的时候
    if(r->flag_deliver && r->take_type == 0)
    {
        //目标工作台 的产品格被拿走了
        if (r->tagetS.product_status == 0)
        {
            r->flag_deliver = false;
            vs[num[1]].flag_waitRobot = false;
        }
    }
}



//输送任务被其他机器人抢先完成了
//这个函数是应对发物品的过程中被别的机器人抢先完成了
void completeJob(Robot* r)
{
    Staging s(-1, -1, r->x_loca, r->y_loca);
    //此时手上拿的东西，不缺了，deliver至false
    if (r->take_type == 1) 
    {
        //目标不缺1
        if (r->tagetS.staging_id == 4)
        {
            if (!(r->tagetS.raw_material_status == 0 || r->tagetS.raw_material_status == 4))
            {
                r->flag_deliver == false;
                r->tagetS = s;
                //perror("需要改变目标工作台了**");
            }
        }
        else if (r->tagetS.staging_id == 5)
        {
            //perror("目标是5");
            if (!(r->tagetS.raw_material_status == 0 || r->tagetS.raw_material_status == 8))
            {
                r->flag_deliver = false;
                r->tagetS = s;
                perror("需要改变目标工作台了");
            }               
        }
    }
    if (r->take_type == 2)
    {
        //目标不缺2
        if(r->tagetS.staging_id == 4)
        {
            if (!(r->tagetS.raw_material_status == 2 || r->tagetS.raw_material_status == 0))
                r->flag_deliver = false;
        }
        else if (r->tagetS.staging_id == 6)
        {
            if (!(r->tagetS.raw_material_status == 0 || r->tagetS.raw_material_status == 8))
                r->flag_deliver = false;
        }
    }
    if (r->take_type == 3)
    {
        //目标不缺3
        if (r->tagetS.staging_id == 5)
        {
            if (!(r->tagetS.raw_material_status == 0 || r->tagetS.raw_material_status == 2))
            {
                r->flag_deliver = false;
                r->flag_wait = true;
            }
        }
        else if (r->tagetS.staging_id == 6)
        {
            if (!(r->tagetS.raw_material_status == 0 || r->tagetS.raw_material_status == 4))
            {
                r->flag_deliver = false;
                r->flag_wait = true;
            }
 
        }
    }
    //if()
}



