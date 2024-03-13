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
        //do something---���г�ʼ��
        
    }
    return false;
}

bool readMap(vector<Staging>* vStaging, vector<Robot>* vRobot)
{
    char line[1024];
    int num = 0;    //Y����
    int robotId = 0;    //������ID
    int num_stage = 0;
    while (fgets(line, sizeof line, stdin)) {
        if (line[0] == 'O' && line[1] == 'K') {
            return true;
        }
        //do something---���г�ʼ��
        int i = 0;
        while (line[i] != '\n')
        {   
            // ������ͼ�еĻ�����
            if (line[i] == 'A')
            {
                // ��û����˱�ź�λ����Ϣ
                Robot* r = new Robot(robotId, 0.25 + 0.5 * i, 49.75 - 0.5 * num);
                vRobot->push_back(*r);
                robotId++;
                //cout << "������Y���꣺" << i * 0.25 << endl;
            }
            else if (line[i] != '.')
            {
                // ��ȡ����̨�����ͺ�λ��
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
    //int num = 0;    //��¼������������
    while (fgets(line, sizeof line, stdin)) {
        if (line[0] == 'O' && line[1] == 'K') {
            //����֮ǰ ���з���
            vector<string>::iterator it_code = input_code.begin() + 2;
            vector<Robot>::iterator it_r = r->begin();
            vector<Staging>::iterator it_s = s->begin();
            for (int i = 0; i < s->size(); i++, it_code++)
            {
                //��ȡ����̨��Ϣ
                (*it_s).codeToInfo(*it_code);
                it_s++;
            }
            for (int j = 0; j < r->size(); j++, it_code++)
            {
                //��ȡ��������Ϣ
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
    //��ȡ��ͼ��Ϣ��������������Ķ���
    readMap(&vStaging, &vRobot);
    //�Թ���̨���з���
    classStaging(&vStaging, &vStaging_No1, &vStaging_No4, &vStaging_No5, &vStaging_No6, &vStaging_No7, &vStaging_No8, &vStaging_No9);
    
    //���ݾ������Ϣ�ƶ���ʼ���ԣ��ƶ���ɣ����OK
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

    //���Դ���
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
        readInfo(&vRobot, &vStaging);  // �ӵڶ��п�ʼ������ȡ����Ϣ�������������˵�����
        classStaging(&vStaging, &vStaging_No1, &vStaging_No4, &vStaging_No5, &vStaging_No6, &vStaging_No7, &vStaging_No8, &vStaging_No9);
        //classStageByLoc(&vStaging, &dStageLoc1, &dStageLoc2, &dStageLoc3, &dStageLoc4);
        printf("%d\n", frameID);    //�����
        // ����Ի����˵�����
        
        //ʵʱ���� �����˵�Ŀ�깤��̨״̬
        //for (vector<Robot>::iterator it_r2 = vRobot.begin(); it_r2 != vRobot.end(); it_r2++)
        //{
        //    for (vector<Staging>::iterator it_s2 = vStaging.begin(); it_s2 != vStaging.end(); it_s2++)
        //    {
        //        if ((*it_r2).tagetS.staging_num == (*it_s2).staging_num)
        //        {
        //            (*it_r2).tagetS = *it_s2;
        //            //perror("ƥ��ɹ�");
        //        }
        //            
        //    }
        //}
        //if (!vStaging_No7.empty())
        //{
        //    for (vector<Staging>::iterator it_s = vStaging_No7.begin(); it_s != vStaging_No7.end(); it_s++)
        //    {
        //        endNumber = (*it_s).staging_num;
        //        // ���ȱ4
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
   

        
        //����50֡��ʱ�򣬳���ſ�ʼ����ֹ��һ�ΰ�num����̨��false
        if (frameID > 50)
        {
            /*if (vStaging[0].flag_waitRobot)
                perror("ƾʲô��ȥ��һ������̨");*/
            /*if (vStaging[1].flag_waitRobot)
                perror("ƾʲô��ȥ�ڶ�������̨");
            if (vStaging[2].flag_waitRobot)
                perror("ƾʲô��ȥ����������̨");
            if (vStaging[3].flag_waitRobot)
                perror("ƾʲô��ȥ���ĸ�����̨");*/

            //less7 = false;

            /*if (vStaging[9].flag_waitRobot && vStaging[2].flag_waitRobot)
                perror("�ھźź͵�2�Ź���̨���ܽ����㷨��");
            if (vStaging[9].flag_waitRobot)
                perror("�źŹ���̨���ܽ����㷨");
            if (vStaging[1].flag_waitRobot)
                perror("1�Ų��ܽ����㷨");
            if (vStaging[10].flag_waitRobot)
                perror("10�Ų��ܽ����㷨");

            if (vStaging[12].flag_waitRobot)
                perror("12�Ų��ܽ����㷨��");*/
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
                // ����7��������
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
        //    //�������
        //}
        





        printf("OK\n");
        fflush(stdout);
    }
        

    //��������һ�б�scanf�õ�������fgets�ӵڶ��п�ʼ
    //while (scanf("%d", &frameID) != EOF) {
    //    readUntilOK();
    //    printf("%d\n", frameID);    //�����
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


//����7����̨������
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














// �ȶ�789 ����̨����
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
//            //��ɹ��˾ͻ�Ŀ��2
//            if (r->buyGoods(r->tagetS, r->tagetS.staging_num))
//            {
//                perror("����Ĺ���̨�������²���̰���㷨��");
//                r->tagetS.flag_waitRobot = false;
//                r->tagetS = vs[num[1]];
//            }
//            //perror("Ŀһֱ��num1��");
//        }
//        else
//        {
//            //perror("Ŀ�������");
//            if (r->sellGoods(r->tagetS, r->tagetS.staging_num))
//            {
//                r->tagetS.flag_waitRobot = false;
//                r->flag_deliver = false;
//            }
//        }
//    }
//
//}

//�����������������������õ���Ʒ������flagΪfalse
void othersTakeThe(Robot* r, vector<Staging>& vs, int* num)
{
    //�������ǿ��ֵ�ʱ��
    if(r->flag_deliver && r->take_type == 0)
    {
        //Ŀ�깤��̨ �Ĳ�Ʒ��������
        if (r->tagetS.product_status == 0)
        {
            r->flag_deliver = false;
            vs[num[1]].flag_waitRobot = false;
        }
    }
}



//���������������������������
//���������Ӧ�Է���Ʒ�Ĺ����б���Ļ��������������
void completeJob(Robot* r)
{
    Staging s(-1, -1, r->x_loca, r->y_loca);
    //��ʱ�����õĶ�������ȱ�ˣ�deliver��false
    if (r->take_type == 1) 
    {
        //Ŀ�겻ȱ1
        if (r->tagetS.staging_id == 4)
        {
            if (!(r->tagetS.raw_material_status == 0 || r->tagetS.raw_material_status == 4))
            {
                r->flag_deliver == false;
                r->tagetS = s;
                //perror("��Ҫ�ı�Ŀ�깤��̨��**");
            }
        }
        else if (r->tagetS.staging_id == 5)
        {
            //perror("Ŀ����5");
            if (!(r->tagetS.raw_material_status == 0 || r->tagetS.raw_material_status == 8))
            {
                r->flag_deliver = false;
                r->tagetS = s;
                perror("��Ҫ�ı�Ŀ�깤��̨��");
            }               
        }
    }
    if (r->take_type == 2)
    {
        //Ŀ�겻ȱ2
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
        //Ŀ�겻ȱ3
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



