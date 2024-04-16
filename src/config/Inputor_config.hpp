#pragma once

void Inputor::initInputor()
{
    setGroup("Model_Parameters");
    addInputor(para.Nspin         ,   "N"               , 		    1                 );
    addInputor(para.D             ,   "D"               , 		    2                 );
    addInputor(para.beta          ,   "beta"            , 		    0.4               );
    addInputor(para.L             ,   "L"               , 		    8                 );

}
