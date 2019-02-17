#! /usr/bin/python2.7
'''
/*
*	HBS Analysis
*	Author: Ariel Livshits 2018
*/
'''

import re
import matplotlib.pyplot as plt
import numpy as np
import os



ind = 0
flow_id = ind; ind = ind + 1
flow_size = ind; ind = ind + 1
flow_remainingSize = ind; ind = ind + 1
flow_arrival = ind; ind = ind + 1
flow_src = ind; ind = ind + 1
flow_dst = ind; ind = ind + 1
flow_FCT = ind; ind = ind + 1

TO_MSEC = 1.0e3
gamma = 0.8

def moving_average(x,y):
    data = []
    for i in range(len(x)):
        data.append((x[i], y[i]))
    data.sort(key=lambda pair: pair[0])
    
    moving_avg = [0]
	
    fcts = [pair[1] for pair in data]
    
    for n in range(len(fcts)):
        moving_avg.append( moving_avg[n]*gamma + fcts[n])
    
    return moving_avg[1:]

# Sim-#1: switch_radix: 16, reconfig_penalty: 20, time_window_width: 3000, 
# light_flows_num: 12, heavy_flows_num: 4, light_range: [1,16], 
# heavy_range: [16,100], adaptive: 0, useFilter: 0, threshold: 0
def params_extractor(simDesc_line):
    params = {}
    vals = [int(elem) for elem in re.findall(r'\d+', simDesc_line)]
    
    ind = 0
    params['sim_num'] = vals[ind]; ind = ind + 1
    params['switch_radix'] = vals[ind]; ind = ind + 1
    params['reconfig_penalty'] = vals[ind]; ind = ind + 1
    params['time_window_width'] = vals[ind]; ind = ind + 1
    params['light_flows_num'] = vals[ind]; ind = ind + 1
    params['heavy_flows_num'] = vals[ind]; ind = ind + 1
    params['light_range'] = [vals[ind],vals[ind+1]]; ind = ind + 2
    params['heavy_range'] = [vals[ind],vals[ind+1]]; ind = ind + 2
    params['adaptive'] = vals[ind]; ind = ind + 1
    params['useFilter'] = vals[ind]; ind = ind + 1
    params['threshold'] = vals[ind]
    
    return params

scheduler_names = ["Solstice", "Lumos"]

class LogData:
    
    def __init__(self, logfile_path, params):
        
        self.logfile_name = logfile_path
        for name in scheduler_names:
            if name in logfile_path:
                self.scheduler = name
       
        self.params = params
        self.flows = {}
        self.num_of_flows = 0
        self.avg_FCT = 0
        self.avg_slowdown = 0
        
        scheduler_log = open(logfile_path)
        
        for line in scheduler_log:
            self.num_of_flows = self.num_of_flows + 1
            data = [int(elem) for elem in re.findall(r'\d+', line)]
            self.flows[data[flow_id]] = data
            self.avg_FCT = self.avg_FCT + data[flow_FCT]
            self.avg_slowdown = self.avg_slowdown + float(data[flow_FCT]) / data[flow_size]
            
        scheduler_log.close()
        
        self.avg_FCT = float(self.avg_FCT) / self.num_of_flows
        self.avg_slowdown = float(self.avg_slowdown) / self.num_of_flows
              
        self.arrival_times = []
        self.flow_completion_times = []
        for flow in self.flows.values():
            self.arrival_times.append(flow[flow_arrival]  / TO_MSEC)
            self.flow_completion_times.append(flow[flow_FCT] / TO_MSEC)


        self.moving_avg = moving_average(self.arrival_times, self.flow_completion_times)
        
    def displayAdaptive(self, fig_num, title):
        plt.figure(fig_num)
        
        if self.params['adaptive']:
            lbl = 'Adaptive ' + self.scheduler
            style = '--'
            colr = 'b'
        else:
            lbl = self.scheduler
            style = '-'
            colr = 'r'
        
        plt.plot(sorted(self.arrival_times), self.moving_avg, ls=style, color=colr, label=lbl)
        plt.xlabel('Arrival Times [msec]', fontsize='large')
        plt.ylabel('Mean Flow Completion Times [msec]', fontsize='large')
        plt.legend()
        plt.title(title)
        plt.grid(True)
        
    def displayFiltered(self, fig_num, title):
        plt.figure(fig_num)
        
        if self.params['useFilter']:
            lbl = 'Filtered ' + self.scheduler
            style = '--'
            colr = 'b'
        else:
            lbl = self.scheduler
            style = '-'
            colr = 'r'
        
        plt.plot(sorted(self.arrival_times), self.moving_avg, ls=style, color=colr, label=lbl)
        plt.xlabel('Arrival Times [msec]', fontsize='large')
        plt.ylabel('Mean Flow Completion Times [msec]', fontsize='large')
        plt.legend()
        plt.title(title)
        plt.grid(True)

sims = []
SimsDesc = open('SimsDesc.txt')
for simDesc_line in SimsDesc:
    
    params = params_extractor(simDesc_line)
    directory = "Results-Sim" + str(params['sim_num'])
    logfiles = os.listdir(directory)
    
    for logfile in logfiles:
        if 'Lumos' in logfile:
            lumos_data = LogData(directory + "/" + logfile, params)
        elif 'Solstice' in logfile:
            solstice_data = LogData(directory + "/" + logfile, params)
            
    sims.append( {'params':params, 'Lumos': lumos_data, 'Solstice': solstice_data} )
    
SimsDesc.close()    
    
for num1, scheduler in enumerate(['Lumos', 'Solstice']):
    for num2, switch_radix in enumerate([16, 32, 64]):
        for sim in sims:
            if sim['params']['switch_radix'] == switch_radix:
                sim[scheduler].displayAdaptive(3*num1 + num2, 'Switch Radix: ' + str(switch_radix))
                
    for num2, switch_radix in enumerate([128, 256]):
        for sim in sims:
            if sim['params']['switch_radix'] == switch_radix:
                sim[scheduler].displayFiltered(2*num1 + num2 + 6, 'Switch Radix: ' + str(switch_radix))
                
plt.show()               
