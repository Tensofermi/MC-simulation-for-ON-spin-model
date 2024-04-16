#pragma once

void Observable::initObservable()  // add addBasicObser() & addCombiObser()
{
	int Vol = pow(para.L, para.D);
	double Beta = para.beta;

    //  Observables               Name           				Description             		 A     +     B  *  Obs 
    addBasicObser(para.i_M        	 , "M"        		, "Total magnetic density"    				, 0     , 1.0/Vol      					);
	addBasicObser(para.i_absM        , "absM"        	, "Absolute magnetic density"    			, 0     , 1.0/Vol      					);
	addBasicObser(para.i_M2        	 , "M2"        		, "Square of magnetic density"       		, 0     , 1.0/Vol/Vol      				);
	addBasicObser(para.i_M4        	 , "M4"        		, "Fourth power of magnetic density" 		, 0     , 1.0/Vol/Vol/Vol/Vol   		);
	addBasicObser(para.i_E        	 , "E"        		, "Energy density"    			            , 0     , 1.0/Vol      					);
	addBasicObser(para.i_E2        	 , "E2"        		, "Square of Energy density"    			, 0     , 1.0/Vol/Vol      				);

	addCombiObser(para.i_Cv        	 , "Cv"        		, "Heat Capacity"    			            , 0     , Beta*Beta/Vol		        	);

    addBasicObser(para.i_NCluster    , "NCluster"    	, "Number of Clusters"    				    , 0     , 1.0/Vol 		      			);
	addBasicObser(para.i_S2          , "S2"        		, "Second moment of clusters"    			, 0     , 1.0/Vol/Vol       			);
	addBasicObser(para.i_S4        	 , "S4"        		, "Fourth moment of clusters"       		, 0     , 1.0/Vol/Vol/Vol/Vol   		);
	addBasicObser(para.i_SM4         , "SM4"        	, "Fourth power of magnetic density"        , 0     , 1.0/Vol/Vol/Vol/Vol   		);
	addBasicObser(para.i_C1        	 , "C1"        		, "Largest cluster size" 					, 0     , 1.0/Vol 						);
	addBasicObser(para.i_C2        	 , "C2"        		, "Second-Largest cluster size"    			, 0     , 1.0/Vol      					);

	addBasicObser(para.i_Mk2         , "Mk2"            , "Magnetic structure factor"    			, 0     , 1.0/Vol/Vol      				);

	addCombiObser(para.i_Qm        	 , "Qm"        		, "Binder ratio"    			            , 0     , 1.0		      				);
	addCombiObser(para.i_Qm_fk       , "Qm_fk"        	, "Binder ratio"  			  			    , 0     , 1.0		      				);
	addCombiObser(para.i_C12         , "C12"        	, "Binder ratio"    			            , 0     , 1.0		      				);
	addCombiObser(para.i_corr_L      , "Corr/L"        	, "Binder ratio"    			            , 0     , 1.0		      				);

	addBasicObser(para.i_M2_E        , "M2*E"           , "for K"    								, 0     , 1.0/Vol/Vol/Vol     			);
	addBasicObser(para.i_Mk2_E       , "Mk2*E"          , "for Kk"    								, 0     , 1.0/Vol/Vol/Vol 		      	);

	addCombiObser(para.i_K 		     , "K"        		, "M^2 covariance"    			            , 0     , -1.0		      				);
	addCombiObser(para.i_Kk 	     , "K_k"        	, "M_k^2 covariance"    			        , 0     , -1.0		      				);

	

}

void Observable::calCombiObser()  // add Result[]
{
	Result[para.i_Cv] = Ob[para.i_E2] - Ob[para.i_E] * Ob[para.i_E];

	Result[para.i_Qm] = Ob[para.i_M2] * Ob[para.i_M2] / Ob[para.i_M4];
	Result[para.i_Qm_fk] = Ob[para.i_S2] * Ob[para.i_S2] / Ob[para.i_SM4];
	Result[para.i_corr_L] = 1.0 / (2 * para.L * sin(M_PI / para.L)) * sqrt(Ob[para.i_M2] / Ob[para.i_Mk2] - 1.0);
	Result[para.i_C12] = Ob[para.i_C2] / Ob[para.i_C1];

	Result[para.i_K] = Ob[para.i_M2_E] / Ob[para.i_M2] - Ob[para.i_E];
	Result[para.i_Kk] = Ob[para.i_Mk2_E] / Ob[para.i_Mk2] - Ob[para.i_E];
}

// ---------------------------------------------
// This function prints the Monte Carlo results.
// ---------------------------------------------
std::string Observable::printOutput(bool Flag)
{
	std::string str = "";
	ZZ0 = NBin * NperBin + IBin;

	str += "==============================================================================\n";
	str += l_jf("N = " + toStr(para.Nspin), 22) + l_jf("D = " + toStr(para.D), 22) + "\n";
	str += l_jf("beta = " + toStr(para.beta), 22) + l_jf("L = " + toStr(para.L), 22) + "\n";
	str += l_jf("N_Therm = " + toStr(para.N_Therm), 22) + l_jf("N_Total = " + toStr(para.N_Total), 22) + l_jf("Seed = " + toStr(para.Seed), 22) + "\n";
	str += l_jf("N_Measure = " + toStr(para.N_Measure), 22) + l_jf("N_Each = " + toStr(para.N_Each), 22) + l_jf("NBlock = " + toStr(para.NBlock), 22) + "\n";
	str += "==============================================================================\n";

	if(Flag)
	{
		if(NBin >= NBlock)
		{
			ZZ = (NBin / NBlock) * NBlock * NperBin;
			//--------------------------------------------------//
			//------------ obtain average and error ------------//
			//--------------------------------------------------//
			calAveErr();	
			//--------------------------------------------------//
			//------------ obtain average and error ------------//
			//--------------------------------------------------//
			str += "Average: ZZ = " + toStr(ZZ0) + " , MCStep = " + toStr(MCStep0) + " , Status = " + dou2str(100.0 * MCStep / (double)para.N_Each / (double)para.N_Total, 2) + "%" + "\n";
			str += printAverage();
		}
		else
		{
			str += "    The samples are too few!!!\n";
		}
		str += "Simulation Time: " + ck.tick() + " , Total: " + ck.exportTime() + " , " + ck.exportNow() + "\n";
	}
	else
	{
		str += "Current: ZZ0 = " + toStr(ZZ0) + " , MCStep0 = " + toStr(MCStep0) + " , Status = " + dou2str(100.0 * MCStep / (double)para.N_Each / (double)para.N_Therm, 2) + "%" + "\n";
		str += printCurrent();
		str += "Therm Time: " + ck.tick() + " , Total: " + ck.exportTime() + " , " + ck.exportNow() + "\n";
	}
	
	str += "==============================================================================\n";

	return str;
}