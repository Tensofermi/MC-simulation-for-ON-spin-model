#pragma once
#include <bits/stdc++.h>
#include "Utils/Tools_Fun.hpp"
#include "Clock.hpp"
#include "IOControl.hpp"
#include "../config/Parameter_config.hpp"
#define sprintf_s sprintf

class DistributionBase
{
    Clock& ck;
    IOControl& io;

    std::string Des;
    int disType;    // 0: linear, 1: log
    unsigned long disCount, disNum, disMax, disMin;
    double a, b;    // range [a,b]
    std::vector<long double> disData; 
    bool saveOrNot = true;     // check if save or not

public:
    DistributionBase(std::string _Type, std::string _Des, double _a, double _b, unsigned long _size, Clock& _ck, IOControl& _io);
    void disAdd(double _obs);
    void disDel(double _obs);
    void disPush(unsigned long _index, double _obs);
    void disPull(unsigned long _index, double _obs);
    void saveDis();

    unsigned long obs2Bin(double _obs);
    double bin2Obs(unsigned long _bin);

    // logbin
    std::vector<int> mappingArray;
    int logNum = 2000;
    int interval = 1;
    int modsteps = 64;

};

DistributionBase::DistributionBase(std::string _Type, std::string _Des, double _a, double _b, unsigned long _size, Clock& _ck, IOControl& _io) 
: ck(_ck), io(_io)
{
    Des = _Des;
    disCount = 0;
    disNum = _size;
    disMax = 0;
    disMin = disNum;

    a = _a;
    b = _b;

    if (_Type == "linear")
    {
        disType = 0;
    }
    else if (_Type == "log")
    {
        disType = 1;
        mappingArray.resize(logNum);
        unsigned long val = 0; 
        for (int i = 0; i < logNum; i++)
        {
            mappingArray[i] = val;
            val += interval;
            if (i != 0 && i % modsteps == 0) interval = 2 * interval;
        }
    }
    else
    {
        io.exportInfo(io.ErrorInfo, "something wrong in distribution type");
    }

    disData.resize(disNum);
    for (unsigned long i = 0; i < disNum; i++)
        disData[i] = 0;
}

unsigned long DistributionBase::obs2Bin(double _obs)
{
    unsigned long Bin_;

    if (disType == 0)
    {
        // mapping [a,b] into [0,disNum]
        Bin_ = (long)((_obs - a) * disNum / (b - a));
        return Bin_;
    }
    else if (disType == 1)
    {
        int val, left, right;
        left = 0;
        right = logNum - 1;
        while (left <= right)
        {
            Bin_ = left + (right - left) / 2;
            if (_obs == mappingArray[Bin_]) return Bin_;
            else if (mappingArray[Bin_] < _obs) left = Bin_ + 1;
            else right = Bin_ - 1;
        }

    }

    return 0;
}

double DistributionBase::bin2Obs(unsigned long _bin)
{
    double Obs_;

    if (disType == 0)
    {
        // mapping [0,disNum] into [a,b]
        Obs_ = a + (b - a) * _bin / disNum;
        return Obs_;
    }
    else if (disType == 1)
    {
        // return log function
        return mappingArray[_bin];
    }

    return 0;
}

void DistributionBase::disAdd(double _obs)
{
    disCount++;
    unsigned long bindex = obs2Bin(_obs);

    if (bindex >= disNum || bindex == 0) return; // overflow
    disData[bindex] += 1;

    if(disMax < bindex) disMax = bindex;
    if(disMin > bindex) disMin = bindex;
}

void DistributionBase::disDel(double _obs)
{
    disCount--;
    unsigned long bindex = obs2Bin(_obs);

    if (bindex >= disNum || bindex == 0) return; // overflow
    disData[bindex] -= 1;

    if(disMax < bindex) disMax = bindex;
    if(disMin > bindex) disMin = bindex;
}

void DistributionBase::disPush(unsigned long _index, double _obs)
{
    disCount++;
    
    if (_index >= disNum || _index == 0) return; // overflow
    disData[_index] += _obs;

    if(disMax < _index) disMax = _index;
    if(disMin > _index) disMin = _index;
}

void DistributionBase::disPull(unsigned long _index, double _obs)
{
    disCount--;
    
    if (_index >= disNum || _index == 0) return; // overflow
    disData[_index] -= _obs;

    if(disMax < _index) disMax = _index;
    if(disMin > _index) disMin = _index;
}

void DistributionBase::saveDis()
{
    if (!saveOrNot) return;

    double obsVal, obsNum, obsDis;

    std::string disPrint = "# disCount = " + toStr(disCount) + "\n";
    for (unsigned long i = disMin; i <= disMax; i++)
    {
        obsVal = bin2Obs(i);
        obsNum = disData[i];    
        obsDis = obsNum/(double)disCount;
        disPrint += l_jf(toStr(obsVal), 20);   //  observable value
        disPrint += l_jf(toStr(obsNum), 20);   //  counting number
        disPrint += l_jf(toStr(obsDis), 20);   //  normalization
        disPrint += "\n";
    }

    // save
	std::ofstream file;
	file.open(Des + ".dis");
	file << disPrint;
	file.close();
}

//----------------------------------------------------
class Distribution
{
    Clock& ck;
    IOControl& io;
    Parameter& para;

    DistributionBase** Dis;
    int NDis;
    bool saveOrNot = true;     // check if save or not

    public:
    Distribution(Clock& _ck, IOControl& _io, Parameter& _para);
    void initDistr();
    void addDistr(int & _Name, std::string _Type, std::string _Des,  double _a, double _b, unsigned long _size);
    void obsAdd(int _NDis, double _obs);
    void obsDel(int _NDis, double _obs);
    void obsPush(int _NDis, unsigned long _index, double _obs);
    void obsPull(int _NDis, unsigned long _index, double _obs);
    void saveDistr();
};

Distribution::Distribution(Clock& _ck, IOControl& _io, Parameter& _para) : ck(_ck), io(_io), para(_para)
{
    NDis = 0;
    Dis = nullptr;
    initDistr();
}

#include "../config/Distribution_config.hpp"

void Distribution::addDistr(int & _Name, std::string _Type, std::string _Des,  double _a, double _b, unsigned long _size)
{
    _Name = NDis;
    
    DistributionBase** Dis0 = new DistributionBase* [NDis + 1];
    for (int i = 0; i < NDis; i++)
    {
        Dis0[i] = Dis[i];
    }
    Dis0[NDis] = new DistributionBase(_Type, _Des, _a, _b, _size, ck, io);
    delete[] Dis;
    Dis = Dis0;
    NDis++;
}


void Distribution::obsAdd(int _NDis, double _obs)
{
    Dis[_NDis]->disAdd(_obs);
}

void Distribution::obsDel(int _NDis, double _obs)
{
    Dis[_NDis]->disDel(_obs);
}

void Distribution::obsPush(int _NDis, unsigned long _index, double _obs)
{
    Dis[_NDis]->disPush(_index, _obs);
}

void Distribution::obsPull(int _NDis, unsigned long _index, double _obs)
{
    Dis[_NDis]->disPull(_index, _obs);
}

void Distribution::saveDistr()
{
    for (int i = 0; i < NDis; i++)
    {
        Dis[i]->saveDis();
    }
}