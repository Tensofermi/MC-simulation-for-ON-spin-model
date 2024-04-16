#pragma once

/* ------------------------------------------------------------------------------------*/
/* ---------------------------------- Spin Operation ----------------------------------*/
/* ------------------------------------------------------------------------------------*/

void Configuration::randSpin_()
{
    switch (Nspin)
    {
        case 1:
        {
            tempSpin[0] = 2 * rn.getRandomNum(2) - 1;
            break;
        }
        case 2:
        {    
            double theta = rn.getRandomDouble(0, 2 * M_PI);
            tempSpin[0] = cos(theta);
            tempSpin[1] = sin(theta);
            break;
        }
        case 3:
        {
            double z = 2 * rn.getRandomDouble() - 1;
            double theta = rn.getRandomDouble(0, 2 * M_PI);
            tempSpin[0] = cos(theta) * sqrt(1 - z * z);
            tempSpin[1] = sin(theta) * sqrt(1 - z * z);
            tempSpin[2] = z;
            break;
        }
        default:
            break;
    }
}

void Configuration::randSpin()
{
    double length = 0.0;

    for (int i_N = 0; i_N < Nspin; i_N++)
        tempSpin[i_N] = rn.getRandomDouble() - 0.5;  // [-0.5, 0.5)

    length = abs(tempSpin);
    if (length == 0) 
    {
        randSpin();
        return;
    }

    for (int i_N = 0; i_N < Nspin; i_N++)
        tempSpin[i_N] /= length;

}

void Configuration::flipSpin(long _site, std::vector<double> _axis)
{
    Site[_site] = Site[_site] - 2 * Site[_site] * _axis * _axis;
}
