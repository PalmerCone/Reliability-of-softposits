import os
import sys
import random
import time

nTrials = int(sys.argv[1])
seed = int (sys.argv[2])
random.seed(seed)
start = 0
print ("\n\n\nCreating submission scripts for:")
print ("\t", nTrials, " fault injection trials in 8 scripts")
print ("\t", seed, " RNG seed")
print ("\n\n\n")

if len(sys.argv) >=4:
    start = int(sys.argv[3])
for i in range(0, start):
    t = random.randint(0,50000000)
    print ("Skippig trial %d with seed %d" %(i, t))


# TODO: set this variable to where "test_HPCCG" exists
exe_path = "/home/pcone/HPC/vy"
flipit_dir = os.environ["FLIPIT_PATH"]
i = start
while i < nTrials:

    #write the pbs submission file
    outfile = open("fault" + str(i) + ".pbs", "w")
    outfile.write("#!/bin/bash\n"
                "#PBS -l select=1:ncpus=1:mem=8gb\n"
                "#PBS -l walltime=09:00:00\n"
                #"#PBS -j oe\n"
        "#PBS -A jr5\n"
                "#PBS -N ECE_6740_HW3\n"
                "module load gcc\n"
                "module load anaconda3/5.1.0-gcc\n"
                "module list\n")
    j = 0
    while j < (nTrials-start)/16 + 1:
    	if i+j < nTrials:
            outfile.write("\nmkdir $PBS_O_WORKDIR/test_" + str(i+j)\
                	+ "\ncd $PBS_O_WORKDIR/test_" +str(i+j)\
                        + "\n" + exe_path + "/a.out "+str(i+j) + " &> test_" +str(i+j) + ".txt\n")

	
    	j += 1
    
    #submit the file
    outfile.close()
    if i <= nTrials:
        print ("Launching job...")#, i
        os.system("qsub fault" + str(i) + ".pbs")

    i += j
    

