# pragma once

// ------------------------------------------------------------------------
// This function passes the config data into the observable for collection.
// ------------------------------------------------------------------------
void Configuration::measure()
{
//--- Spin Observables
	
	for (unsigned int i = 0; i < Nspin; i++) 
		tempSpin[i] = 0;
	
	obs.Ob[para.i_E] = 0;
	for (unsigned int i = 0; i < Vol; i++)
	{
		tempSpin = tempSpin + Site[i];
		for (unsigned int j = 0; j < NNb / 2; j++)	// avoid double counting
		{
			obs.Ob[para.i_E] += Site[i] * Site[Latt.getNNSite(i,j)];
		}
	}
	obs.Ob[para.i_M] = abs(tempSpin);
	obs.Ob[para.i_absM] = obs.Ob[para.i_M];
	obs.Ob[para.i_M2] = obs.Ob[para.i_M] * obs.Ob[para.i_M];
	obs.Ob[para.i_M4] = obs.Ob[para.i_M2] * obs.Ob[para.i_M2];
	obs.Ob[para.i_E] = -obs.Ob[para.i_E];	// add minus
	obs.Ob[para.i_E2] = obs.Ob[para.i_E] * obs.Ob[para.i_E];

//--- Cluster Observables
	obs.Ob[para.i_NCluster] = NCluster;
	obs.Ob[para.i_S2] = S2;
	obs.Ob[para.i_S4] = S4;
	obs.Ob[para.i_SM4] = 3 * S2 * S2 - 2 * S4;
	obs.Ob[para.i_C1] = C1;
	obs.Ob[para.i_C2] = C2;

//--- k-space Observables
	std::vector<double> mk_c, mk_s;
	mk_c.resize(Nspin);
	mk_s.resize(Nspin);
	
	for (int i = 0; i < Nspin; i++)
	{
		mk_c[i] = 0;
		mk_s[i] = 0;
	}
	
	for (int i = 0; i < Vol; i++)
	{
		mk_c = mk_c + k_cos[i] * Site[i];
		mk_s = mk_s + k_sin[i] * Site[i];
	}
	
	double Mk2_temp = 0;
	for (int i = 0; i < Nspin; i++)
		Mk2_temp += mk_c[i] * mk_c[i] + mk_s[i] * mk_s[i];

	obs.Ob[para.i_Mk2] = Mk2_temp;

	obs.Ob[para.i_M2_E] = obs.Ob[para.i_M2] * obs.Ob[para.i_E];
	obs.Ob[para.i_Mk2_E] = obs.Ob[para.i_Mk2] * obs.Ob[para.i_E];
}