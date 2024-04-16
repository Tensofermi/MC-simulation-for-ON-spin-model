import os
import datetime
import parameter_qsub as p
from itertools import product

def get_unique_folder_name(input_path="my_folder"):
    # Extract the directory and base name from the input path.
    dir_path = os.path.dirname(input_path)
    base_name = os.path.basename(input_path)

    suffix = "_1"
    new_folder = os.path.join(dir_path, base_name)

    # While the folder (or its variants with suffix) exists, increment the suffix and check again.
    while os.path.exists(new_folder + suffix):
        suffix = "_" + str(int(suffix.lstrip("_")) + 1)  # Increment the number after underscore or start from 1.

    # Return the folder name with the original name or the appropriate suffix.
    return new_folder + suffix

def write_terms(f_w, para_name, para_val):
    tab = "\t" * 1
    slen = 20
    f_w.write(para_name.ljust(slen) + tab + str(para_val).ljust(slen) + tab + "\n")


########################################################################################
JOB_NAME = 'LRON'
ROOT_DIR = 'jobs'
compiler_op = 'g++ -std=c++11 -O2'     # g++, icpc, clang++, ifort, gfortran, python3 ...
########################################################################################
# Input File
arrays = [p.N, p.D, p.beta, p.L]
arrays.extend([p.Seed, p.N_Measure, p.N_Each, p.N_Therm, p.N_Total, p.NBlock, p.MaxNBin, p.NperBin])
########################################################################################

# jobs file name
now = datetime.datetime.now().strftime("%Y%m%d")
main_folder = ROOT_DIR + '/' + now + '_' + JOB_NAME
main_folder = get_unique_folder_name(main_folder)
print('jobs are in ' + main_folder)

combinations = list(product(*arrays))

# compile code
os.system("cd ../bin && " + compiler_op + " ../src/" + "main.cpp")
bin_dir = os.getcwd() + '/../bin/a.out'     # binary file address

# create work files
if not os.path.exists(ROOT_DIR):
    os.makedirs(ROOT_DIR)
os.system("mkdir " + main_folder)                           # create work file
os.chdir(main_folder)                                       # enter into work file

# loop combinations
num_jobs = 0
for combo in combinations:
########################################################################################
    _N = combo[0]
    _D = combo[1]
    _beta = combo[2]
    _L = combo[3]
    _seed = combo[-8]
########################################################################################
    job_name = "N_" + str(_N) +"_D_" + str(_D) + "_beta_" + str(_beta) + "_L_" + str(_L) + "_seed_" + str(_seed)

    os.system("mkdir " + job_name)                    # create each job file
    os.system("touch ./" + job_name + "/input.txt")   # create input file for each job

    f_w = open("./" + job_name + "/input.txt",'w')
    f_w.write("//----- Model_Parameters" + "\n")
    write_terms(f_w, "N",           combo[0])
    write_terms(f_w, "D",           combo[1])
    write_terms(f_w, "beta",        combo[2])
    write_terms(f_w, "L",           combo[3])
    f_w.write(""+"\n")    
    f_w.write("//----- Simulation_Parameters" + "\n")
    write_terms(f_w, "Seed",        combo[-8])
    write_terms(f_w, "N_Measure",   combo[-7])
    write_terms(f_w, "N_Each",      combo[-6])
    write_terms(f_w, "N_Therm",     combo[-5])
    write_terms(f_w, "N_Total",     combo[-4])
    write_terms(f_w, "NBlock",      combo[-3])
    write_terms(f_w, "MaxNBin",     combo[-2])
    write_terms(f_w, "NperBin",     combo[-1])
    f_w.write(""+"\n")                               
    f_w.close()

################################################################################################# 
################################################################################################# 
# generate jobs
    job_dir = os.getcwd()
    num_jobs = num_jobs + 1
    job_id = str(num_jobs)
    f = open("job_"+job_id,'w')
    f.write("#!/bin/bash"+'\n')
    f.write("#PBS -N "+ job_name +'\n')
    f.write("#PBS -j oe"+ '\n')
    f.write("#PBS -l walltime="+ p.job_time + '\n\n\n')

    f.write("cd  " + job_dir + "/" + job_name + "\n") 

    f.write(bin_dir + '\n')
    f.write( "echo " + job_name + " finished  at `date` " +" >> "+  "log.dat"+ '\n')
    f.write("\n")
    f.write("exit 0")
    f.close()

# ################################################################################################# 
# ################################################################################################# 
# create qsub.sh
f_w = open('qsub.sh', 'w')
f_w.write('#!/bin/bash' + '\n' + '\n')
f_w.write('Njob=' + str(num_jobs) + '\n')
f_w.write('sub=1' + '\n')
f_w.write('if [ $sub -eq 1 ]' + '\n')
f_w.write('then' + '\n')
f_w.write('for ((i=1;i<=$Njob;i++)); do' + '\n')
f_w.write('  {' + '\n')
f_w.write('      qsub job_$i' + '\n')
f_w.write('          sleep .5' + '\n')
f_w.write('      echo $i' + '\n')
f_w.write(' }' + '\n')
f_w.write('done' + '\n')
f_w.write('wait' + '\n')
f_w.write('fi' + '\n')
f_w.write('exit 0' + '\n')
f_w.close()

# chmod +x
os.system('chmod +x qsub.sh')

################################################################################################# 
################################################################################################# 
# fast.sh
os.chdir('../../')
f_w = open('fast.sh', 'w')
f_w.write('cd ' + main_folder + ' && ./qsub.sh')
f_w.close()

# chmod +x
os.system('chmod +x fast.sh')
