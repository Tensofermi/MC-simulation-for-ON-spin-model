#pragma once
#include <bits/stdc++.h>
#include "../Parameter_config.hpp"
#include "../../system/Clock.hpp"
#include "../../system/IOControl.hpp"
#include "../../system/RandomNumGen.hpp"
#include "../../system/Observable.hpp"
#include "../../system/Distribution.hpp"

#include "../Lattice/LatticeList.hpp"

class Configuration
{
    Clock& ck;
    IOControl& io;
    RandomNumGen& rn;
    Parameter& para;
    Observable& obs;
    Distribution& dis;

public:
    //--- Configuration
    std::vector<std::vector<double>> Site;
    Hypercubic Latt;            
    int Dim, Nspin, L, NNb;
    long Vol;
    double Beta;

    //--- Basic parameter for algorithms
    std::vector<int> Mem, Que;
    double P_metro, P_sw;

    //--- For spin flip and k-space
    std::vector<double> tempSpin;
    std::vector<double> k_vec, k_cos, k_sin;

    //--- Basic Observables
    std::vector<int> x_max, x_min, x_now;
    long NCluster;                  // Number of clusters
    double C1, C2, S2, S4;          // Cluster size defined by particle number

public:
    Configuration(Clock& _ck, IOControl& _io, RandomNumGen& rn, Parameter& _para, Observable& _obs, Distribution& _dis);
    void initialConf();
    void initialAlgo();
    void initialMeas();
    void initialObsr();

    void updateCnf();
    bool measureOrNot();
    void measure();
    void writeCnf();

    void printConfig(int _index);
    void squarePrint();

    void checkCnf();

    // Basic Operation
    void randSpin();
    void randSpin_();
    void flipSpin(long _site, std::vector<double> _axis);
    
    // Update Algorithm
    void Metropolis();
    void Wolff();
    void SwendsenWang();


};

Configuration::Configuration(Clock& _ck, IOControl& _io, RandomNumGen& _rn, Parameter& _para, Observable& _obs, Distribution& _dis) 
: ck(_ck), io(_io), rn(_rn), para(_para), obs(_obs), dis(_dis)
{
    initialConf();  // for site spin 
    initialAlgo();  // for algorithms
    initialMeas();  // for k-space and cluster measurement
    initialObsr();  // for basic observables
}

void Configuration::initialConf()
{
    //--- Initialize Basic Parameters
    Beta = para.beta;
    Dim = para.D;
    L = para.L;
    Nspin = para.Nspin;

    //--- Initialize Lattice
    Latt.set(Dim, L);
    Vol = Latt.getVol();
    NNb = Latt.getNNb();
    Site.resize(Vol, std::vector<double>(Nspin));
    
    //--- Initialize Temporary Spin
    tempSpin.resize(Nspin);

    //--- Initialize Spin Configuration
    for (unsigned int i = 0; i < Vol; i++)
    {
        randSpin();
        Site[i] = tempSpin;
    }

}

void Configuration::initialAlgo()
{
    //--- Initialize Queue and Memory
    Que.resize(Vol);
    Mem.resize(Vol);
    for (int i = 0; i < Vol; i++)
    {
        Que[i] = 0;
        Mem[i] = 0;
    }

    //--- Initialize Probabilities
    P_metro = 0.0;
    P_sw = 0.0;

}

void Configuration::initialMeas()
{
    //--- k-space Measurement
    k_vec.resize(Dim);
    for (int i = 0; i < Dim; i++) 
        k_vec[i] = 0;

    k_vec[0] = 2 * M_PI / L;    // kx = 2 Pi / L

    k_cos.resize(Vol);
    k_sin.resize(Vol);

    std::vector<double> coor;
    coor.resize(Dim);
    for (int i = 0; i < Vol; i++)
    {
        for (int j = 0; j < Dim; j++)
            coor[j] = Latt.getComponent(i, j);

        k_cos[i] = cos(k_vec * coor);
        k_sin[i] = sin(k_vec * coor);
    }
    coor.clear();

    //--- Geometric Measurement
    x_max.resize(Dim);
    x_min.resize(Dim);
    x_now.resize(Dim);

    for (int i = 0; i < Dim; i++)
    {
        x_max[i] = 0;
        x_min[i] = 0;
        x_now[i] = 0;
    }

}

void Configuration::initialObsr()
{
    //--- Initialize Cluster Observables
    NCluster = 0;
    C1 = 0;
    C2 = 0;
    S2 = 0;
    S4 = 0;

}


#include "UpdataScheme.hpp"
#include "PrintConfiguration.hpp"
#include "../Measurement_config.hpp"

bool Configuration::measureOrNot()
{
    return true;
}

void Configuration::writeCnf()
{
    
}

void Configuration::checkCnf()
{

}
