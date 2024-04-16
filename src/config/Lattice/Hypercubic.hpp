#pragma once

class Hypercubic
{
    // 1D               2D                  3D                     ...
    //
    //                         1                   2  4                  
    //                         |                   | /
    //                         |                   |/
    //  0 -------- 1    3 -----|----- 0     0 -----|---- 5         ...
    //                         |                  /|
    //                         |                 / |
    //                         2                1  3
    
protected:
    int Dim = 1;
    int L = 10;
    long Vol = 10;
    int NNb = 2;

    int* x_ = nullptr;

public:
    void set(int _Dim, int _L);
    
    int getL();                                                // Get L 
    long getVol();                                             // Get NSite or Vol
    long getNBond();                                           // Get NBond
    int getNNb();                                              // Get NNb
    int getDim();                                              // Get Dim
    int getOpsDir(int _Dir);                                   // Get the opposite direction of _Dir

    int getComponent(long _Site, int _Dir);                    // Get component in direction _Dir of site _Site
    long getSite(int *_Component);                             // Get the site for component _Component
    long getSite(std::vector<int> &_Component);                // Get the site for component _Component [vector version]

    int getDir(long _Site, long _NNSite);                      // Get Direction
    long getNNSite(long _Site, int _Dir);                      // Get the nearest neighbor site in the direction _Dir
    long getNNBond(long _Site, int _Dir);                      // Get the nearest neighbor bond in the direction _Dir
    int getParity(long _Site);                                 // Get Parity
    long getDirSite(long _Site, int _Dir, int _Length);
    
};


//initialization
void Hypercubic::set(int _Dim, int _L)
{
    Dim = _Dim;
    L = _L;
    Vol = pow(L, Dim);
    NNb = 2 * Dim;

    x_ = new int[Dim];
}

int Hypercubic::getL()
{
    return L;
}

long Hypercubic::getVol()
{
    return Vol;
}

long Hypercubic::getNBond()
{
    return Dim * Vol;
}

int Hypercubic::getNNb()
{
    return NNb;
}

int Hypercubic::getDim()
{
    return Dim;
}

int Hypercubic::getOpsDir(int _Dir)
{
    return NNb - _Dir - 1;
}

//-----------------------------------------------------------
//-----------------------------------------------------------
int Hypercubic::getComponent(long _Site, int _Dir)      // Site --> (x_n, x_n-1, x_n-2, ...)
{
    for (int i = Dim - 1; i > _Dir; i--) _Site = _Site / L;
    return _Site % L;
}

long Hypercubic::getSite(int *_Component)               // (x_n, x_n-1, x_n-2, ...) --> Site
{
    long Vc;
    Vc = _Component[0];
    for (int i = 1; i < Dim; i++)
    {
        Vc = _Component[i] + Vc * L;
    }
    return Vc;
}

long Hypercubic::getSite(std::vector<int> &_Component)  // (x_n, x_n-1, x_n-2, ...) --> Site
{
    long Vc;
    Vc = _Component[0];
    for (int i = 1; i < Dim; i++)
    {
        Vc = _Component[i] + Vc * L;
    }
    return Vc;
}

//-----------------------------------------------------------
//-----------------------------------------------------------

long Hypercubic::getNNSite(long _Site, int _Dir)
{
    int i;
    for (i = 0; i < Dim; i++) x_[i] = getComponent(_Site, i);
    if(_Dir < Dim)
    {
        if((++x_[_Dir])>=L) x_[_Dir] -= L;
    }
    else
    {
        _Dir = getOpsDir(_Dir);
        if((--x_[_Dir])<0) x_[_Dir] += L;
    }
    return getSite(x_);
}

long Hypercubic::getNNBond(long _Site, int _Dir)
{
    if(_Dir < Dim) {return _Site + _Dir * Vol;}
    else {return getNNSite(_Site, _Dir) + getOpsDir(_Dir) * Vol;}
}

int Hypercubic::getDir(long _Site, long _NNSite)
{
    for (int Dir = 0; Dir < NNb; Dir++)
    {
        if(getNNSite(_Site, Dir)==_NNSite) return Dir;
    }
    return -1;
}

int Hypercubic::getParity(long _Site)
{
	long x = 0;
	for (int i = 0; i < Dim; i++)
	{
		x += getComponent(_Site, i);
	}
	if(x % 2) return -1;
	return 1;
}

long Hypercubic::getDirSite(long _Site, int _Dir, int _Length)
{
    int i;
    for (i = 0; i < Dim; i++) x_[i] = getComponent(_Site, i);
    if(_Dir < Dim)
    {
        x_[_Dir] += _Length; 
        if((x_[_Dir])>=L) x_[_Dir] -= L;
    }
    else
    {
        _Dir = getOpsDir(_Dir);
        x_[_Dir] -= _Length; 
        if((x_[_Dir])<0) x_[_Dir] += L;
    }
    return getSite(x_);

}


