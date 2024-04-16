#pragma once

void IOControl::initIOControl()
{
	// File names
	Files[InputInfo] = "input.info";	// 	check input parameter
	Files[ErrorInfo] = "error.info";	//	error ocurr
	Files[WarniInfo] = "warni.info";	//	warning ocurr
	Files[TestiInfo] = "testi.info";	//	for test 
	Files[OuputInfo] = "ouput.info";	//	output routine 
	Files[OuputCont] = "ouput.txt";		//	output current obs

	// Operations for each file
	Opers[InputInfo] = HisWrt;
	Opers[ErrorInfo] = Prt + Wrt + Abort;
	Opers[WarniInfo] = Prt + HisWrt;
	Opers[TestiInfo] = HisWrt;
	Opers[OuputInfo] = HisWrt;
	Opers[OuputCont] = Wrt;
}