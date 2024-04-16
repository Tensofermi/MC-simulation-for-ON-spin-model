import math
import numpy as np


#=== Model_Parameters
N = [3]

D = [3]

Kc = 0.693003
beta = [Kc - 0.1, Kc - 0.04, Kc - 0.02, Kc, Kc + 0.02, Kc + 0.04, Kc + 0.1]

L = [8, 16, 20, 32]

#=== Simulation_Parameters
Seed = [12345]

N_Measure = [1]

N_Each = [1000]

N_Therm = [5]

N_Total = [50]

NBlock = [1000]

MaxNBin = [1000000]

NperBin = [1]
