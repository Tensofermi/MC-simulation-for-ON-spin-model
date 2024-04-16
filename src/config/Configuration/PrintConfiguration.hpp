# pragma once

void Configuration::printConfig(int _index)
{
    // squarePrint();

}

void Configuration::squarePrint()
{
    if(Dim!=2) return;           // only for two dimensions
    if(Nspin!=1) return;       // only for Ising model

    std::string str;
    int coor[2];

    str += "//---\n";
    for (int i = 0; i < para.L; i++)
    {   
        for (int j = 0; j < para.L; j++)
        {
            coor[0] = i;
            coor[1] = j;

            str += r_jf(Site[Latt.getSite(coor)][0], 3)  + "    ";
        }
        str += "\n";
    }
    str += "//---\n";
    
    io.exportInfo(io.TestiInfo, str);
}