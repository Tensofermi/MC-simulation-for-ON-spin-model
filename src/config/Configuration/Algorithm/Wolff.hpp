#pragma once

/* ------------------------------------------------------------------------------------*/
/* ------------------------------ Short-Range Interaction -----------------------------*/
/* ------------------------------------------------------------------------------------*/

void Configuration::Wolff()
{
    long i_Site, j_Site;
    double project;
    int rear, front;

    //--- Clear Queue and Memory
    for (unsigned int i = 0; i < Vol; i++)
    {
        Que[i] = 0;
        Mem[i] = 0;
    }

    //--- Randomly choose an axis to project
    randSpin();
    
    //--- BFS Search
    i_Site = rn.getRandomNum(Vol);
    Mem[i_Site] = 1;

    rear = 0; 
    front = 1;
    Que[front] = i_Site;    // start from 1 !

    while(front > rear)
    {
        rear++;
        i_Site = Que[rear];
        for (unsigned int i = 0; i < NNb; i++)
        {
            j_Site = Latt.getNNSite(i_Site, i);
            project = (tempSpin * Site[i_Site]) * (tempSpin * Site[j_Site]);
            P_sw = 1.0 - exp(-2 * Beta * project);
            if(Mem[j_Site] == 0 && rn.getRandomDouble() < P_sw)
            {
                front++; 
                Mem[j_Site] = 1;
                Que[front] = j_Site; 
            }
        }

    }

    //--- Flip Cluter Spins
    for (unsigned i = 1; i <= front; i++)    // start from 1 !
        flipSpin(Que[i], tempSpin);


}
