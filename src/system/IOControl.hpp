#pragma once
#include <bits/stdc++.h>
#include "Utils/Tools_Fun.hpp"

class IOControl
{
    std::vector<void(*)(std::string File, std::string Info)> ExcuteFunc;

    int* Opers;
    std::string* Files;

    int NoPrtOrWrt;     // do nothing
    int Prt;            // print data in terminal
    int Wrt;            // rewrite data in file
    int HisWrt;         // append data in file
    int Abort;          // abort program

public:
    // the number of total types
    int ExportInfoNum = 6;

    int InputInfo = 0;	// 	check input parameter
    int ErrorInfo = 1;	//	error ocurr
    int WarniInfo = 2;	//	warning ocurr
    int TestiInfo = 3;	//	for test 
    int OuputInfo = 4;	//	output routine 
    int OuputCont = 5;	//	output current obs

    IOControl();
    ~IOControl();
    void initIOControl();
    void clearInfo();

    void exportInfo(int InfoType, std::string Info);
};

void IOControl::clearInfo()   // delete the last files
{
    void wrt(std::string File, std::string Info);
    for (int i = 0; i < ExportInfoNum; i++) remove(Files[i].c_str());
    return;
}

void IOControl::exportInfo(int InfoType, std::string Info)
{
    int x = Opers[InfoType];
    for (int i = 0; x != 0; i++, x = x / 2)
    {
      if (x % 2 == 1) ExcuteFunc[i](Files[InfoType], Info);
    }
}

#include "../config/IOControl_config.hpp"

IOControl::IOControl()
{
    void noPrtOrWrt(std::string File, std::string Info);
    void prt(std::string File, std::string Info);
    void wrt(std::string File, std::string Info);
    void hisWrt(std::string File, std::string Info);
    void abort(std::string File, std::string info);

    // File names and operations for each file
    Files = new std::string[ExportInfoNum];
    Opers = new int[ExportInfoNum];

    // Basic operations for IO control
    NoPrtOrWrt = (int)pow(2, 0);
           Prt = (int)pow(2, 1);
           Wrt = (int)pow(2, 2);
        HisWrt = (int)pow(2, 3);
         Abort = (int)pow(2, 4);

    ExcuteFunc.push_back(noPrtOrWrt);
    ExcuteFunc.push_back(prt);
    ExcuteFunc.push_back(wrt);
    ExcuteFunc.push_back(hisWrt);
    ExcuteFunc.push_back(abort);

    initIOControl();
    clearInfo();
}

IOControl::~IOControl()
{
    delete[] Opers; Opers = nullptr; 
    delete[] Files; Files = nullptr; 
}

//--------------------------------
//--- Functions for IO control
//--------------------------------
void noPrtOrWrt(std::string File, std::string Info)
{
    return;
}

void prt(std::string File, std::string Info)
{
    std::cout << Info;
    return;
}

void wrt(std::string File, std::string Info)
{
    std::ofstream file;
    file.open(File.c_str());
    file << Info;
    file.close();
    return;
}

void hisWrt(std::string File, std::string Info)
{
    std::ofstream file;
    file.open(File.c_str(), std::ios::app);
    file << Info;
    file.close();
    return;
}

void abort(std::string File, std::string info)
{
    std::string str = "";
    str += "Attention!\n    This program is aborted, see details in file\"" + File + "\"\n";
    std::cout << str << std::endl;
    std::ofstream file;
    file.open(File.c_str(), std::ios::app);
    file << info;
    file.close();
    exit(0);
}