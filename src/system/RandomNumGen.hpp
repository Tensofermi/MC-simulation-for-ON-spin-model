#pragma once
#include <bits/stdc++.h>
#include "Utils/Tools_Fun.hpp"
 
class RandomNumGen
{
    double tm32;
    int mult;
    int mod2, mul2;
    int len1, ifd1;
    int len2, ifd2;
    int* inxt1 = nullptr;
    int* inxt2 = nullptr;
    int* ir1 = nullptr; 
    int* ir2 = nullptr;
    int ipnt1, ipnf1;
    int ipnt2, ipnf2;
    int mxrn;
    int* irn = nullptr;
    
	int Seed;
    int nrannr;
    
public:
    RandomNumGen();
    ~RandomNumGen();
    RandomNumGen(int Seed);
    void initRandomNumGen(int Seed);                // initialize 
    int getRandomNum();                             // output -2^31 ~ 2^31-1
    int getRandomNum(int MaxNum);                   // output 0 ~ MaxNum-1
    long getRandomNum(long MaxNum);                 // output 0 ~ MaxNum-1
    double getRandomDouble();                       // output [0,1)
    double getRandomDouble(double a, double b);     // output [a,b)
    double getGaussian();                           // output Gaussian Distribution
};


RandomNumGen::RandomNumGen()
{
    // default seed is 1234567
    initRandomNumGen(1234567);    
}

RandomNumGen::RandomNumGen(int Seed)
{
    initRandomNumGen(Seed);
}

RandomNumGen::~RandomNumGen()
{
  delete[] inxt1; inxt1 = nullptr;
  delete[] inxt2; inxt2 = nullptr;
  delete[] ir1; ir1 = nullptr;
  delete[] ir2; ir2 = nullptr;
  delete[] irn; irn = nullptr;
}

void RandomNumGen::initRandomNumGen(int Seed)
{
    tm32 = 1.0e0 / pow(2.0e0, 32.0e0);
    mult = 32781;
    mod2 = 2796203; mul2 = 125;
    len1 = 9689; ifd1 = 471;
    len2 = 127; ifd2 = 30;
    mxrn = 10000;
    
    if(inxt1 == nullptr && inxt2 == nullptr && ir1 == nullptr && ir2 == nullptr && irn == nullptr)
        {
            inxt1 = new int[len1 + 1];
            inxt2 = new int[len2 + 1];
              ir1 = new int[len1 + 1];
              ir2 = new int[len2 + 1];
              irn = new int[mxrn + 1];
        }
    else
        {
            delete[] inxt1;
            delete[] inxt2;
            delete[] ir1;
            delete[] ir2;
            delete[] irn;
            inxt1 = new int[len1 + 1];
            inxt2 = new int[len2 + 1];
              ir1 = new int[len1 + 1];
              ir2 = new int[len2 + 1];
              irn = new int[mxrn + 1];
        }

    int i_r, k_r, k1_r;
    int iseed;
    nrannr = mxrn;
    iseed = abs(Seed) + 1;
    k_r = pow(3.0, 18.0) + 2 * iseed;
    k1_r = 1313131 * iseed;
    k1_r = k1_r - (k1_r / mod2) * mod2;
    for (i_r = 1; i_r <= len1; i_r++)
    {
        k_r = k_r * mult;
        k1_r = k1_r * mul2;
        k1_r = k1_r - (k1_r / mod2) * mod2;
        ir1[i_r] = k_r + k1_r * 8193;
    }
    for (i_r = 1; i_r <= len2; i_r++)
    {
        k_r = k_r * mult;
        k1_r = k1_r * mul2;
        k1_r = k1_r - (k1_r / mod2) * mod2;
        ir2[i_r] = k_r + k1_r * 4099;
    }
    for (i_r = 1; i_r <= len1; i_r++)
    {
        inxt1[i_r] = i_r + 1;
    }
    inxt1[len1] = 1;
    ipnt1 = 1;
    ipnf1 = ifd1 + 1;
    for (i_r = 1; i_r <= len2; i_r++)
    {
        inxt2[i_r] = i_r + 1;
    }
    inxt2[len2] = 1;
    ipnt2 = 1;
    ipnf2 = ifd2 + 1;
}

int RandomNumGen::getRandomNum()
{
    int i_r, l_r, k_r;
    nrannr = nrannr + 1;
    if (nrannr >= mxrn)
    {
        nrannr = 1;
        for (i_r = 1; i_r <= mxrn; i_r++)
        {
            l_r = ir1[ipnt1] ^ ir1[ipnf1];
            k_r = ir2[ipnt2] ^ ir2[ipnf2];
            irn[i_r] = k_r ^ l_r;
            ir1[ipnt1] = l_r;
            ipnt1 = inxt1[ipnt1];
            ipnf1 = inxt1[ipnf1];
            ir2[ipnt2] = k_r;
            ipnt2 = inxt2[ipnt2];
            ipnf2 = inxt2[ipnf2];
        }
    }
    return irn[nrannr];
}

int RandomNumGen::getRandomNum(int MaxNum)
{
    return (int)(MaxNum * getRandomDouble());
}

long RandomNumGen::getRandomNum(long MaxNum)
{
    return (long)(MaxNum * getRandomDouble());
}

double RandomNumGen::getRandomDouble()
{
    return getRandomNum() * tm32 + 0.5e0;
}

double RandomNumGen::getRandomDouble(double a, double b)
{
    return getRandomDouble() * (b - a) + a;
}

double RandomNumGen::getGaussian()
{
    return sqrt(-2.0 * log(1.0 - getRandomDouble())) * cos(2.0 * M_PI * getRandomDouble());
}