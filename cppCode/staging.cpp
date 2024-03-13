#include <iostream>
using namespace std;

#include <math.h>
#include <string>
#include <vector>
#include <stack>
#include <deque>

#include "robot.h"
#include "staging.h"

bool Staging::is_lack1()
{
    if (this->staging_id == 4 || this->staging_id == 5 || this->staging_id == 9)
    {
        if ((this->raw_material_status & 2) == 0)
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Staging::is_lack2()
{
    if (this->staging_id == 4 || this->staging_id == 6 || this->staging_id == 9)
    {
        if ((this->raw_material_status & 4) == 0)
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Staging::is_lack3()
{
    if (this->staging_id == 5 || this->staging_id == 6 || this->staging_id == 9)
    {
        if ((this->raw_material_status & 8) == 0)
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Staging::is_lack4()
{
    if (this->staging_id == 7 || this->staging_id == 9)
    {
        if ((this->raw_material_status & 16) == 0)
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Staging::is_lack5()
{
    if (this->staging_id == 7 || this->staging_id == 9)
    {
        if ((this->raw_material_status & 32) == 0)
            return true;
        else
            return false;
    }
    return
        false;
}

bool Staging::is_lack6()
{
    if (this->staging_id == 7 || this->staging_id == 9)
    {
        if ((this->raw_material_status & 64) == 0)
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Staging::is_lack7()
{
    if (this->staging_id == 8 || this->staging_id == 9)
    {
        if ((this->raw_material_status & 128) == 0)
            return true;
        else
            return false;
    }
    return false;
}

bool Staging::is_lackAll()
{
    if (this->raw_material_status == 0)
        return true;
    else
        return false;
}


void Staging::callRobotTo456(Robot* pr)
{
    
    // ���������û����Ʒ �������ڸɻ� ����
    if (pr->take_type == 0 || pr->flag_deliver)
        return;

    // ����������Ʒ����û�¸�
    // �ֹ���̨4�� ȱ����
    if(pr->take_type == 1 || pr->take_type == 2 || pr->take_type == 3)  
    {
        if (this->staging_id == 4 && this->raw_material_status != 6)
        {
            // ���ȱ1
            if (pr->take_type == 1 && this->is_lack1())
            //if (pr->take_type == 1 && (this->raw_material_status == 0 || this->raw_material_status == 4))
            {
                pr->tagetS = *this;
                pr->flag_deliver = true;
                return;
            }
            // ���ȱ2
            else if (pr->take_type == 2 && (this->raw_material_status == 2 || this->raw_material_status == 0))
            {
                pr->tagetS = *this;
                pr->flag_deliver = true;

                return;
            }
        }
        //����̨5ȱ����
        else if (this->staging_id == 5 && this->raw_material_status != 10)
        {
            //���ȱ1
            if (pr->take_type == 1 && (this->raw_material_status == 0 || this->raw_material_status == 8))
            {
                pr->tagetS = *this;
                pr->flag_deliver = true;

                return;
            }
            else if (pr->take_type == 3 && (this->raw_material_status == 0 || this->raw_material_status == 2))
            {
                pr->tagetS = *this;
                pr->flag_deliver = true;

                return;
            }
        }
        //����̨6
        else if (this->staging_id == 6 && this->raw_material_status != 12)
        {
            if (pr->take_type == 2 && (this->raw_material_status == 0 || this->raw_material_status == 8))
            {
                pr->tagetS = *this;
                pr->flag_deliver = true;

                return;
            }
            else if (pr->take_type == 3 && (this->raw_material_status == 0 || this->raw_material_status == 4))
            {
                pr->tagetS = *this;
                pr->flag_deliver = true;

                return;
            }
        }
    }
}


void Staging::callRobotTo789(Robot* pr)
{
    if (pr->take_type == 0 || pr->flag_deliver)
        return;


    if (this->staging_id == 7 && this->raw_material_status != 112)
    {
        // ȱ4
        if (pr->take_type == 4 && (this->raw_material_status == 96 || 
            this->raw_material_status == 64 || this->raw_material_status == 32 || this->raw_material_status == 0))
        {
            pr->tagetS = *this;
            pr->flag_deliver = true;
            return;
        }
        // ȱ5
        if (pr->take_type == 5 && (this->raw_material_status == 80 || 
            this->raw_material_status == 64 || this->raw_material_status == 16 || this->raw_material_status == 0))
        {
            pr->tagetS = *this;
            pr->flag_deliver = true;
            return;
        }
        // ȱ6
        if (pr->take_type == 6 && (this->raw_material_status == 48 || 
            this->raw_material_status == 32 || this->raw_material_status == 16 || this->raw_material_status == 0))
        {
            pr->tagetS = *this;
            pr->flag_deliver = true;
            return;
        }
    }
    else if (this->staging_id == 8 && this->raw_material_status == 0)
    {
        if (pr->take_type == 7)
        {
            pr->tagetS = *this;
            pr->flag_deliver = true;
            return;
        }
    }
    else if (this->staging_id == 9)
    {
        if (pr->take_type == 7 || pr->take_type == 4 || pr->take_type == 5 || pr->take_type == 6)
        {
            pr->tagetS = *this;
            pr->flag_deliver = true;
            return;
        }
    }
}

