#pragma once
#include <bits/stdc++.h>
#include "Utils/Tools_Fun.hpp"
#include "Clock.hpp"
#include "IOControl.hpp"
#include "../config/Parameter_config.hpp"
#define sprintf_s sprintf

class ObservableBase
{
public:
	unsigned long NBlock;
	unsigned long MaxNBin;
	unsigned long NperBin;
	
	std::string Name;
	std::string Des;
	double* BlkList;
	double Ave;
	double Err;
	double Cor;
	double A;
	double B;

public:
	ObservableBase();
	ObservableBase(std::string _Name, std::string _Des, double _A, double _B, unsigned long _NBlock, unsigned long _MaxNbin, unsigned long _NperBin);
	void calErr();
	std::string printAverage(int Index);
	double getBlock(int i);
	double getAve();
};

ObservableBase::ObservableBase()
{
    Name = "UNNAMED";
    Des = "This Observable has not been initialized!";

    BlkList = new double[NBlock];
    for (unsigned long i = 0; i < NBlock; i++) BlkList[i] = 0;
    Ave = 0;
    Err = 1e100;
    Cor = 0;
    A = 0;
    B = 1;
}

ObservableBase::ObservableBase(std::string _Name, std::string _Des, double _A, double _B, unsigned long _NBlock, unsigned long _MaxNbin, unsigned long _NperBin)
{
	NBlock = _NBlock;
	MaxNBin = _MaxNbin;
	NperBin = _NperBin;

	Name = _Name;
	Des = _Des;
	A = _A;
	B = _B;
	BlkList = new double[NBlock];
	for (unsigned long i = 0; i < NBlock; i++) BlkList[i] = 0;
    Ave = 0;
    Err = 1e100;
    Cor = 0;
}

void ObservableBase::calErr()
{
    unsigned long i;
    double err, cor, ave;
    double devn, devp;
    ave = Ave;
    
    err = 0; cor = 0;
    devn = 0; devp = 0;
    for (i = 0; i < NBlock; i++)
    {
        devp = devn;
        devn = BlkList[i] - ave;
        err += devn * devn;
        cor += devp * devn;
    }
	if(err < 1e-99)
	{
		Cor = 1e8;
	}
	else
	{
		Cor = cor / err;
	}
    Err = sqrt(err / NBlock / (NBlock - 1));
}

std::string ObservableBase::printAverage(int Index)
{
	char a[150];
	std::string str;
	sprintf_s(a,"%-4d %-12.12s = % -16.8e % -3.8e % -3.3e  %-40.40s\n", Index, ("<"+Name+">").c_str(), A+B*Ave, fabs(B)*Err, Cor, Des.c_str());
    str = a;
	//--------------------------------------------------//
	//-------------------- save obs --------------------//
	//--------------------------------------------------//
	// std::string obs_save;
	// obs_save = r_jf(dou2str(A+B*Ave,9), 12) + "    " + r_jf(dou2str(fabs(B)*Err,9), 12) + "    " + r_jf(dou2str(Cor,9), 12) + "\n";
	// ofstream file;
	// // file.open((Name + ".dat").c_str());
	// file.open((Name + ".dat").c_str(), std::ios::app);
	// file << obs_save;
	// file.close();
	//--------------------------------------------------//
	//-------------------- save obs --------------------//
	//--------------------------------------------------//
    return str;
}

double ObservableBase::getBlock(int i)
{
	return BlkList[i];
}

double ObservableBase::getAve()
{
	return Ave;
}

//----------------------------------------------------
class BasicObservable :public ObservableBase
{
public:
	double  Value;
	double* BinList;

public:
	BasicObservable();
	BasicObservable(std::string _Name, std::string _Des, double _A, double _B, unsigned long _NBlock, unsigned long _MaxNbin, unsigned long _NperBin);
	void measure(double value);
	void collectData(unsigned long _NBin);
	void compressBin();
	std::string printCurrent(int Index);
	void calBlock(unsigned long _NBin, unsigned long _NperBin);
	void calAve();
	void reset();
};

void BasicObservable::calAve()
{
	unsigned long i;
	Ave = 0;
    for (i = 0; i < NBlock; i++) Ave += BlkList[i];
    Ave = Ave / NBlock;
}

void BasicObservable::reset()
{
	unsigned long i;
	Value = 0;
	for (i = 0; i < NBlock; i++) BlkList[i] = 0;
	for (i = 0; i < MaxNBin; i++) BinList[i] = 0;
	Ave = 0;
	Err = 1e100;
	Cor = 0;
	
}

BasicObservable::BasicObservable(std::string _Name, std::string _Des, double _A, double _B, unsigned long _NBlock, unsigned long _MaxNbin, unsigned long _NperBin)
:ObservableBase(_Name, _Des, _A, _B, _NBlock, _MaxNbin, _NperBin)
{
	unsigned long i;
	Value = 0;
	BinList = new double[MaxNBin];
	for (i = 0; i < MaxNBin; i++)
	{
		BinList[i] = 0;
	}
}

void BasicObservable::measure(double value)
{
	Value = value;
}

void BasicObservable::collectData(unsigned long _NBin)
{
	BinList[_NBin] += Value;
}

void BasicObservable::calBlock(unsigned long _NBin, unsigned long _NperBin)
{
	unsigned long n = _NBin / NBlock;
	unsigned long i, j = 0, k = 0;
	for (i = 0; i < NBlock; i++)
	{
		BlkList[i] = 0;
		for (j = 0; j < n; j++)
		{
			BlkList[i] += BinList[k];
			k++;
		}
		BlkList[i] = BlkList[i] / n / _NperBin;
	}
}

std::string BasicObservable::printCurrent(int Index)
{
	char a[150];
    std::string str;
    sprintf_s(a,"%-4d %-12.12s = % -16.8e  %-40.40s\n",Index,Name.c_str(),A+B*Value,Des.c_str());
    str = a;
    return  str;
}

void BasicObservable::compressBin()
{
	unsigned long i;
	unsigned long maxnb2 = MaxNBin / 2;
	for (i = 0; i < maxnb2; i++)
	{
		BinList[i] = BinList[2 * i] + BinList[2 * i + 1];
	}
	for (i = maxnb2; i < MaxNBin; i++)
	{
		BinList[i] = 0;
	}
}

//----------------------------------------------------
class CombiObservable :public ObservableBase
{
public:
	CombiObservable(std::string _Name, std::string _Des, double _A, double _B, unsigned long _NBlock, unsigned long _MaxNbin, unsigned long _NperBin);
	void calBlock(int _BlockIndex, double _Value);
	void reset();
	void calAve(double _Value);
};

CombiObservable::CombiObservable(std::string _Name, std::string _Des, double _A, double _B, unsigned long _NBlock, unsigned long _MaxNbin, unsigned long _NperBin)
:ObservableBase(_Name, _Des, _A, _B, _NBlock, _MaxNbin, _NperBin)
{
    
}

void CombiObservable::calAve(double _Value)
{
	Ave = _Value;
}

void CombiObservable::calBlock(int _BlockIndex, double _Value)
{
	BlkList[_BlockIndex] = _Value;
}

void CombiObservable::reset()
{
	unsigned long i;
	for (i = 0; i < NBlock; i++) BlkList[i] = 0;
	Ave = 0;
	Err = 1e100;
	Cor = 0;

}


//----------------------------------------------------
class Observable
{
	Clock& ck;
    IOControl& io;
    Parameter& para;

public:
	unsigned int NBasic;
	unsigned int NCombi;
	BasicObservable** BasicObser;
	CombiObservable** CombiObser;
	std::vector<double> Ob;
	std::vector<double> Result;

	double MCStep = 0;
	double MCStep0 = 0;
	double MCStep1 = 0;
	double ZZ = 0;
	double ZZ0 = 0;

	unsigned long NBin = 0;
	unsigned long IBin = 0;

	unsigned long NBlock = 1000;
	unsigned long MaxNBin = 100000;
	unsigned long NperBin = 1;
	
public:
	Observable(Clock& _ck, IOControl& _io, Parameter& _para);
	void addBasicObser(int &_Index, std::string _Name, std::string _Des, double _A, double _B);
	void addCombiObser(int &_Index, std::string _Name, std::string _Des, double _A, double _B);
	void initObservable();
	void calCombiObser();
	void collectData();
	void calBlock();
	void calAveErr();
	void reset();
	std::string printOutput(bool Flag);
	std::string printCurrent();
	std::string printAverage();
};

void Observable::reset()
{
	unsigned int Index;
	for (Index = 0; Index < NBasic; Index++) BasicObser[Index]->reset();
	for (Index = 0; Index < NCombi; Index++) CombiObser[Index]->reset();
	NperBin = 1;
	NBin = 0;
	IBin = 0;
	MCStep = 0;
}


Observable::Observable(Clock& _ck, IOControl& _io, Parameter& _para) : ck(_ck), io(_io), para(_para)
{
	NBasic = 0;
	NCombi = 0;
	BasicObser = nullptr;
	CombiObser = nullptr;
	NBlock = para.NBlock;
	MaxNBin = para.MaxNBin;
	NperBin = para.NperBin;

	initObservable();
}

#include "../config/Observable_config.hpp"

void Observable::calAveErr()
{
	unsigned int i;
	// get Ave
	calBlock();
	// get Err
	for (i = 0; i < NBasic; i++) BasicObser[i]->calErr();
	for (i = 0; i < NCombi; i++) CombiObser[i]->calErr();
}

void Observable::addBasicObser(int &_Index, std::string _Name, std::string _Des, double _A, double _B)
{
	BasicObservable** BasicObser0 = new BasicObservable*[NBasic+1];
	_Index = NBasic;
	for (unsigned int i = 0; i < NBasic; i++) BasicObser0[i] = BasicObser[i];
    BasicObser0[NBasic] = new BasicObservable(_Name, _Des, _A, _B, NBlock, MaxNBin, NperBin);
	delete [] BasicObser;
	BasicObser = BasicObser0;
	NBasic++;
	Ob.clear();
	Ob.resize(NBasic);
}

void Observable::addCombiObser(int &_Index, std::string _Name, std::string _Des, double _A, double _B)
{
	CombiObservable** CombiObser0 = new CombiObservable*[NCombi+1];
	_Index = NCombi;
	for (unsigned int i = 0; i < NCombi; i++) CombiObser0[i] = CombiObser[i];
    CombiObser0[NCombi] = new CombiObservable(_Name, _Des, _A, _B, NBlock, MaxNBin, NperBin);
	delete [] CombiObser;
	CombiObser = CombiObser0;
	NCombi++;
	Result.clear();
	Result.resize(NCombi);
}

std::string Observable::printCurrent()
{
	unsigned int i;
	std::string str = "";
	for (i = 0; i < NBasic; i++)
	{
		str += BasicObser[i]->printCurrent(i);
	}
	return str;
}

std::string Observable::printAverage()
{
	unsigned int i;
	std::string str = "";
	calAveErr();
	for (i = 0; i < NBasic; i++)
	{
		str += BasicObser[i]->printAverage(i);
	}
	for (i = 0; i < NCombi; i++)
	{
		str += CombiObser[i]->printAverage(i+NBasic);
	}
	str += "NperBin = " + toStr(NperBin) + " , NperBlk = " + toStr(NperBin * NBin / NBlock) + " , NBin = " + toStr(NBin) + "\n";
	return str;
}

void Observable::calBlock()
{
	unsigned long i, index;
	
	//--- get BasicObser Ave and BlkList
	for (index = 0; index < NBasic; index++)
	{
		BasicObser[index]->calBlock(NBin, NperBin);
		BasicObser[index]->calAve();
	}

	//--- get CombiObser Ave
	for (index = 0; index < NBasic; index++) Ob[index] = BasicObser[index]->getAve();
	calCombiObser();
	for (index = 0; index < NCombi; index++) CombiObser[index]->calAve(Result[index]);

	//--- get CombiObser BlkList
	for (i = 0; i < NBlock; i++)
	{
		for (index = 0; index < NBasic; index++) Ob[index] = BasicObser[index]->getBlock(i);
		calCombiObser();
		for (index = 0; index < NCombi; index++) CombiObser[index]->calBlock(i,Result[index]);		
	}
}

// ---------------------------------------------------------
// This function adds the current obs value to the bin list.
// ---------------------------------------------------------
void Observable::collectData()
{
	unsigned int i;
	for (i = 0; i < NBasic; i++)
	{
		BasicObser[i]->measure(Ob[i]);	    // send Ob[i] to BasicObser[i]
		BasicObser[i]->collectData(NBin);	// add the Value to BinList[Nbin]
	}
	IBin++;		// The index for one Bin
	if(IBin == NperBin)
	{
		IBin = 0;
		NBin++;
		if (NBin == MaxNBin)	// overflow
		{
			for (i = 0; i < NBasic; i++)
			{
				BasicObser[i]->compressBin();
			}
			NBin = MaxNBin / 2;
			NperBin = 2 * NperBin;
		}
		if(NBin%NBlock == 0) MCStep1 = MCStep;
	}
	MCStep0 = MCStep;
}
