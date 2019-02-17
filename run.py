#! /usr/bin/python2.7
'''
/*
*	HBS Run
*	Author: Ariel Livshits 2018
*/
'''

import subprocess
import shutil
import os


Error = 1
args = {}

# Delete old files
old_results = os.listdir(".")
for directory in old_results:
	if "Results" in directory:
		shutil.rmtree(directory)
		

#call makefile
if (subprocess.call("make") == Error):
	raise Exception('make: Error 1')
	
sims_desc = open("SimsDesc.txt", "w")
	
def runSim(sim_num, args):
	if not os.path.isdir("Results"):
		os.mkdir("Results")
		
	params = [str(args['switch_radix']), str(args['reconfig_penalty']), str(args['time_window_width']), str(args['light_flows_num']), \
			 str(args['heavy_flows_num']), str(args['light_range'][0]),str(args['light_range'][1]), str(args['heavy_range'][0]), \
			 str(args['heavy_range'][1]), str(args['adaptive']), str(args['useFilter']), str(args['threshold']) ]

	print "Begin Sim #", sim_num
	subprocess.call(["./HBS"] + params)
	print "Sim #", sim_num, " finished successfully"
	
	os.rename("Results", "Results-Sim" + str(sim_num))
	print "Saved results to Results-Sim", sim_num
	
	print_vals = tuple([str(sim_num)] + params)
	
	sim_desc = "Sim-#%s: switch_radix: %s, reconfig_penalty: %s, time_window_width: %s, light_flows_num: %s, heavy_flows_num: %s, light_range: [%s,%s], heavy_range: [%s,%s], adaptive: %s, useFilter: %s, threshold: %s\n" % print_vals
	
	sims_desc.write(sim_desc)
	
# default args:
args['switch_radix'] = 16
args['reconfig_penalty'] = 20
args['time_window_width'] = 3000
args['light_flows_num'] = 12
args['heavy_flows_num'] = 4
args['light_range'] = [1,16]
args['heavy_range'] = [16,100]
args['adaptive'] = 0
args['useFilter'] = 0
args['threshold'] = 20*8

sims_to_run = range(1,11)
sim = 1

# --------------------------------------------- Non-Adaptive Batch	--------------------------------------------- #
	
# --------------------------------- Sim 1 --------------------------------- #


args['switch_radix'] = 16
args['adaptive'] = 0

if sim in sims_to_run:
	runSim(sim,args) 
sim = sim + 1

# --------------------------------- Sim 2 --------------------------------- #

args['switch_radix'] = 32
args['adaptive'] = 0

if sim in sims_to_run:
	runSim(sim,args) 
sim = sim + 1

# --------------------------------- Sim 3 --------------------------------- #

args['switch_radix'] = 64
args['adaptive'] = 0

if sim in sims_to_run:
	runSim(sim,args) 
sim = sim + 1

# --------------------------------------------- Adaptive Batch	------------------------------------------------ #

# --------------------------------- Sim 4 --------------------------------- #

args['switch_radix'] = 16
args['adaptive'] = 1

if sim in sims_to_run:
	runSim(sim,args) 
sim = sim + 1

# --------------------------------- Sim 5 --------------------------------- #

args['switch_radix'] = 32
args['adaptive'] = 1

if sim in sims_to_run:
	runSim(sim,args) 
sim = sim + 1
# --------------------------------- Sim 6 --------------------------------- #

args['switch_radix'] = 64
args['adaptive'] = 1

if sim in sims_to_run:
	runSim(sim,args) 
sim = sim + 1

# ------------------------------------------- Non-Filtered Batch ---------------------------------------------- #

# --------------------------------- Sim 7 --------------------------------- #

args['switch_radix'] = 128
args['adaptive'] = 1
args['useFilter'] = 0

if sim in sims_to_run:
	runSim(sim,args) 
sim = sim + 1

# --------------------------------- Sim 8 --------------------------------- #

args['switch_radix'] = 256
args['adaptive'] = 1
args['useFilter'] = 0

if sim in sims_to_run:
	runSim(sim,args) 
sim = sim + 1

# --------------------------------------------- Filtered Batch	------------------------------------------------ #

# --------------------------------- Sim 9 --------------------------------- #

args['switch_radix'] = 128
args['adaptive'] = 1
args['useFilter'] = 1

if sim in sims_to_run:
	runSim(sim,args) 
sim = sim + 1

# --------------------------------- Sim 10 --------------------------------- #

args['switch_radix'] = 256
args['adaptive'] = 1
args['useFilter'] = 1

if sim in sims_to_run:
	runSim(sim,args) 
sim = sim + 1



# clean environment
subprocess.call(["make", "clean"])



