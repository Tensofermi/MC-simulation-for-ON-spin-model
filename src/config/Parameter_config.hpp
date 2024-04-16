#pragma once

struct Parameter
{
    // Simulation Parameters
    int Seed;
    unsigned long N_Measure;
    unsigned long N_Each;
    unsigned long N_Therm;
    unsigned long N_Total;
    unsigned long NBlock;
    unsigned long MaxNBin;
    unsigned long NperBin;

    // Model Parameters
    int Nspin, D, L;
    double beta;

    // Observable label
    int i_M, i_absM, i_M2, i_M4, i_E, i_E2;
    int i_NCluster, i_S2, i_S4, i_SM4, i_C1, i_C2;
    int i_Qm, i_Qm_fk, i_Cv, i_C12;
    int i_Mk2, i_corr_L;
    int i_M2_E, i_Mk2_E, i_K, i_Kk;

    // Distribution label

};