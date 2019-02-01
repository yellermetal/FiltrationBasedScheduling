#! /usr/bin/python2.7
'''
/*
*	HBS Run
*	Author: Ariel Livshits 2018
*/
'''

import re
import matplotlib.pyplot as plt
import numpy as np
import subprocess
import os

if not os.path.isdir("Results"):
	os.mkdir("Results")

Error = 1
args = []

switch_radix = 16
args.append(str(switch_radix))

delta = 20
args.append(str(delta))

time_window_width = 3000
args.append(str(time_window_width))

light_flows_num = 12
args.append(str(light_flows_num))

heavy_flows_num = 4
args.append(str(heavy_flows_num))

light_range = [1,16]
args.append(str(light_range[0]))
args.append(str(light_range[1]))

heavy_range = [16,100]
args.append(str(heavy_range[0]))
args.append(str(heavy_range[1]))

if (subprocess.call("make") == Error):
	raise Exception('make: Error 1')
subprocess.call(["./HBS"] + args)
subprocess.call(["make", "clean"])

ind = 0
flow_id = ind; ind = ind + 1
flow_size = ind; ind = ind + 1
flow_remainingSize = ind; ind = ind + 1
flow_arrival = ind; ind = ind + 1
flow_src = ind; ind = ind + 1
flow_dst = ind; ind = ind + 1
flow_FCT = ind; ind = ind + 1

flows = [{},{},{}]
num_of_flows = [0,0,0]
total_FCT = [0,0,0]
total_slowdown = [0,0,0]
logfile = [open("Results/Solstice.txt", "r"),  open("Results/Lumos.txt", "r"),  open("Results/Eclipse.txt", "r")]

for sched,scheduler_log in enumerate(logfile):
    for line in scheduler_log:
        num_of_flows[sched] = num_of_flows[sched] + 1
        data = [int(elem) for elem in re.findall(r'\d+', line)]
        flows[sched][data[flow_id]] = data
        total_FCT[sched] = total_FCT[sched] + data[flow_FCT]
        total_slowdown[sched] = total_slowdown[sched] + float(data[flow_FCT]) / data[flow_size]
    scheduler_log.close()

for sched, sched_name in enumerate(["Solstice", "Lumos", "Eclipse"]):
    avg_FCT = float(total_FCT[sched]) / num_of_flows[sched]
    avg_slowdown = float(total_slowdown[sched]) / num_of_flows[sched]

    print "Average FCT of " + sched_name , avg_FCT
    print "Average Slowdown of " + sched_name , avg_slowdown

arrival_times = [[],[],[]]
FCT = [[],[],[]]

for sched, sched_name in enumerate(["Solstice", "Lumos"]):#,"Eclipse"]):
    for flow in flows[sched].values():
        arrival_times[sched].append(flow[flow_arrival])
        FCT[sched].append( flow[flow_FCT])

FCT[0] = [elem/1.0e3 for elem in FCT[0]]      
FCT[1] = [elem/1.0e3 for elem in FCT[1]] 
#FCT[2] = [elem/1.0e3 for elem in FCT[2]]
arrival_times[0] = [elem/1.0e3 for elem in arrival_times[0]]      
arrival_times[1] = [elem/1.0e3 for elem in arrival_times[1]] 
#arrival_times[2] = [elem/1.0e3 for elem in arrival_times[2]]
        
plt.scatter(arrival_times[0],FCT[0], color='k', s=25, marker="o", alpha=0.5)
plt.scatter(arrival_times[1],FCT[1], color='b', s=25, marker="o", alpha=0.5)
#plt.scatter(arrival_times[2],FCT[2], color='r', s=25, marker="o", alpha=0.5)

#ax = plt.gca()
#ax.set_yscale('log')
#ax.set_xscale('log')
#plt.ylim((1e3, 3e4))
#plt.grid(which='both', axis='y', alpha=0.5)
plt.figure(1)
plt.xlabel('Arrival Times [msec]', fontsize='large')
plt.ylabel('Flow Completion Times [msec]', fontsize='large')
plt.legend(['Solstice','Lumos'])#,'Eclipse'])
plt.xticks(np.arange(0, 30, step=3))
plt.savefig('Scatter.jpg')


plt.figure(2)
plt.hist(FCT, bins=100)
plt.xlabel('Flow Completion Times [msec]', fontsize='large')
plt.legend(['Solstice','Lumos'])#,'Eclipse'])
plt.savefig('Bins.jpg')

plt.show()