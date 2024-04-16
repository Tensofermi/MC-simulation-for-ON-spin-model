#pragma once
#include <bits/stdc++.h>
#include "Utils/Tools_Fun.hpp"
#include "IOControl.hpp"
#include "../config/Parameter_config.hpp"
#define sprintf_s sprintf

class InputorBase
{
public:
    InputorBase() {}
    virtual int readInputor(std::string Line) { return 0; }
    virtual std::string exportInputor() { return ""; }
};

//----------------------------------------------------
template <typename T>
class InputorTerm : public InputorBase
{
    T* Var;
    std::string Key;
    int Flag;
    std::string Des;

public:
    InputorTerm(T& variable, std::string key, T initVar, std::string des);
    int readInputor(std::string Line);      // read parameters and check if valid
    std::string exportInputor();            // output what is read by program
};

template <typename T>
InputorTerm<T>::InputorTerm(T& variable, std::string key, T initVar, std::string des)
{
    Var = &variable;
   *Var = initVar;
    Key = key;
    Des = des;
}

template <typename T>
int InputorTerm<T>::readInputor(std::string Line)
{
    std::stringstream stream;
    std::string key;
    stream << Line;
    stream >> key;
    if (key == Key)
    {
        stream >> *Var;    // send var to program varible !!
        Flag++;
        return 1;
    }
    return 0;
}

template <typename T>
std::string InputorTerm<T>::exportInputor()
{
    std::stringstream stream;
    std::string flag, datastr;
    char a[128];
    std::string printstr;
    stream << *Var;
    stream >> datastr;
    switch (Flag) {
    case 0:
        flag = "Dflt";
        break;
    case 1:
        flag = "Read";
        break;
    default:
        flag = "Last";
    }
    sprintf_s(a, "%-12.12s%-20.20s%-6.6s%-40.40s", Key.c_str(), datastr.c_str(), flag.c_str(), Des.c_str());
    printstr = a;
    return printstr;
}

//----------------------------------------------------
class InputorGroup
{
    InputorBase** Inp;
    int NInp;
    std::string Key;
    std::string Des;

    IOControl& io;

public:
    InputorGroup(IOControl& _io, std::string key, std::string des);
    template <typename T1, typename T2>
    void addInputor(T1& variable, std::string key, T2 initVar, std::string des);
    std::string exportInputor();
    void readInputor(std::string Line);
    std::string getKeyWord() { return Key; }
};

void InputorGroup::readInputor(std::string Line)
{
    int num = 0;
    for (int i = 0; i < NInp; i++)
    {
        num += Inp[i]->readInputor(Line);
    }
    if (num == 0)
    {
        std::string str = "Warning from Inputor::readInputor():\n";
        str += "    No Terms match the std::string!!!\n";
        io.exportInfo(io.WarniInfo, str);
    }
    else if (num > 1)
    {
        std::string str = "Warning from Inputor::readInputor():\n";
        str += "    Multiple Terms match the std::string!!!\n";
        io.exportInfo(io.WarniInfo, str);
    }
}

InputorGroup::InputorGroup(IOControl& _io, std::string key, std::string des) : io(_io)
{
    NInp = 0;
    Inp = nullptr;
    Key = key;
    Des = des;
}

template <typename T1, typename T2>
void InputorGroup::addInputor(T1& variable, std::string key, T2 initVar, std::string des)
{
    InputorBase** Inp0 = new InputorBase * [NInp + 1];
    for (int i = 0; i < NInp; i++)
    {
        Inp0[i] = Inp[i];
    }
    Inp0[NInp] = new InputorTerm<T1>(variable, key, initVar, des);
    delete[] Inp;
    Inp = Inp0;
    NInp++;
}

std::string InputorGroup::exportInputor()
{
    std::string str = "//----- " + Key + "  \n";
    for (int i = 0; i < NInp; i++)
    {
        str += Inp[i]->exportInputor() + "\n";
    }
    str = str + "\n";
    return str;
}

//----------------------------------------------------
class Inputor
{
    std::string InFile;  // name of input file

    InputorGroup** InpGroup;
    int NGroup;

    IOControl& io;
    Parameter& para;

  public:
    Inputor(IOControl& _io, Parameter& _para, std::string name = "input.txt");   // default name is "input.txt"
    void initInputor();
    void initSystemInputor();
    void readInputor();             // Key function !!
    std::string exportInputor();

    void setGroup(std::string key, std::string des = "");
    
    template <typename T1, typename T2>
    void addInputor(T1& variable, std::string key, T2 initVar, std::string des = "");
    
};

Inputor::Inputor(IOControl& _io, Parameter& _para, std::string input_name) : io(_io), para(_para)
{
    InFile = input_name;
    InpGroup = nullptr;
    NGroup = 0;
    initInputor();
    initSystemInputor();
    readInputor();
    io.exportInfo(io.OuputInfo, exportInputor());
}

#include "../config/Inputor_config.hpp"

void Inputor::setGroup(std::string key, std::string des)
{
    InputorGroup** InpGroup0 = new InputorGroup* [NGroup + 1];
    for (int i = 0; i < NGroup; i++)
    {
        InpGroup0[i] = InpGroup[i];
    }
    InpGroup0[NGroup] = new InputorGroup(io, key, des);
    delete[] InpGroup;
    InpGroup = InpGroup0;
    NGroup++;
}

template <typename T1, typename T2>
void Inputor::addInputor(T1& variable, std::string key, T2 initVar, std::string des)
{
    InpGroup[NGroup - 1]->addInputor(variable, key, initVar, des);
}

std::string Inputor::exportInputor()
{
    std::string str = "";
    for (int i = 0; i < NGroup; i++)
    {
        str += InpGroup[i]->exportInputor();
    }
    return str;
}

void Inputor::readInputor()
{
    std::ifstream IFStream;
    std::string Line;
    IFStream.open(InFile.c_str());

    if (!IFStream)
    {
        std::string str = "Warning from Inputor::readInputor():\n";
        str += "    No input file named \"" + InFile + "\"! Read data from initial data.\n";
        io.exportInfo(io.WarniInfo, str);
        return;
    }

    int LineNum = 0;
    std::stringstream SStream;
    std::string key;
    std::string temp;
    int iGroup = -1;
    int i, num;

    while (true)
    {
        if (!getline(IFStream, Line)) break;          // the end of the file
        LineNum++;        
        if (Line == "\r") continue;
        if (Line == "\n") continue;
        if (Line == "") continue;
        if (Line.substr(0, 5) == "//---")             // read this type string
        {   
            SStream.clear();
            SStream.str(Line);
            key = "";
            SStream >> temp >> key;
            num = 0;
            for (i = 0; i < NGroup; i++)
            {
                if (key == InpGroup[i]->getKeyWord())
                {
                    iGroup = i;
                    num++;
                }
            }
            if (num == 0)
            {
                std::string str = "Warning from Inputor::readInputor():\n";
                if (iGroup != -1)
                {
                    str += "    Invalid Inputor Group \"" + key + "\"! Use the last one \"" + InpGroup[iGroup]->getKeyWord() + "\"\n";
                }
                else
                {
                    str += "    Invalid Inputor Group \"" + key + "\"! Use the first one \"" + InpGroup[0]->getKeyWord() + "\"\n";
                }
                io.exportInfo(io.WarniInfo, str);
            }
            else if (num > 1)
            {
                std::string str = "Warning from Inputor::readInputor():\n";
                str += "    Multiple Inputor Groups have the same std::string \"" + key + "\"\n";
                io.exportInfo(io.WarniInfo, str);
            }
        }
        else
        {
            if (iGroup == -1)
            {
                std::string str = "Warning from Inputor::readInputor():\n";
                str += "    No valid Inputor Group! Use the first one \"" + InpGroup[0]->getKeyWord() + "\"\n";
                io.exportInfo(io.WarniInfo, str);
                iGroup = 0;
            }

            // read each term
            InpGroup[iGroup]->readInputor(Line);
        }
    }
}

void Inputor::initSystemInputor()
{
    // This is basic Monte Carlo input information, so we leave it in the system file.

    setGroup("Simulation_Parameters");
    addInputor(para.Seed      ,    "Seed"       , 		 1234       );
    addInputor(para.N_Measure ,    "N_Measure"  ,        1          );
    addInputor(para.N_Each    ,    "N_Each"     ,        100000     );
    addInputor(para.N_Therm   ,    "N_Therm"    ,        100        );
    addInputor(para.N_Total   ,    "N_Total"    ,        10000      );
    addInputor(para.NBlock    ,    "NBlock"     ,        1000       );
    addInputor(para.MaxNBin   ,    "MaxNBin"    ,        1000000    );
    addInputor(para.NperBin   ,    "NperBin"    ,        1          );
    
}

