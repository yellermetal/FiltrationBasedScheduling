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
#import pylab as p
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
    
    def __init__(self, flows_path, queue_path, params):
        
        self.flows_path = flows_path
        self.queue_path = queue_path
        
        for name in scheduler_names:
            if name in flows_path:
                self.scheduler = name
       
        self.params = params
        self.flows = {}
        
        flows_log = open(flows_path)
        
        for line in flows_log:
            data = [int(elem) for elem in re.findall(r'\d+', line)]
            self.flows[data[flow_id]] = data
            
        flows_log.close()
              
        self.flow_completion_times = []
        for flow in self.flows.values():
            self.flow_completion_times.append(flow[flow_FCT] / TO_MSEC)
            
        self.FCTs = {}
        
        for fct in self.flow_completion_times:
            self.FCTs[fct] = self.FCTs.get(fct, 0) + 1
            
        self.queue_size = []
        queue_log = open(queue_path)
        
        for line in queue_log:
            data = [int(elem) for elem in re.findall(r'\d+', line)]
            self.queue_size.append(data[1] / TO_MSEC)
        queue_log.close()
        
        
        if self.params['useFilter']:
            self.lbl = 'Filtered ' + self.scheduler
            self.colr = 'b'
        elif self.params['adaptive'] and self.params['switch_radix'] < 128:
            self.lbl = 'Adaptive ' + self.scheduler
            self.colr = 'b'
        else:
            self.lbl = self.scheduler
            self.colr = 'r'
        
    def displayFlows(self, fig_num, title):
        plt.figure(fig_num)
        
        fcts = sorted(self.FCTs.keys())

        y,binEdges=np.histogram(fcts,bins=100)
        bincenters = 0.5*(binEdges[1:]+binEdges[:-1])
        plt.plot(bincenters,y, color=self.colr, label=self.lbl)
        plt.xlabel('Flow Completion Times [msec]', fontsize='large')
        plt.ylabel('Occurences', fontsize='large')
        plt.legend(loc='upper right', shadow=True)
        plt.title(title)
        plt.grid(True)
        
        plt.savefig('Figures' + "/" + title)
        
        
    def displayQueueSize(self, fig_num, title):
        plt.figure(fig_num)
        
        time = np.array(range(len(self.queue_size))) / TO_MSEC
        
        plt.plot(time , self.queue_size, color=self.colr, label=self.lbl)
        plt.ylabel('DCT in Queue [msec]', fontsize='large')
        plt.xlabel('Time [msec]', fontsize='large')
        plt.legend(loc='upper right', shadow=True)
        plt.title(title)
        plt.grid(True)
        
        plt.savefig('Figures' + "/" + title)

sims = []
SimsDesc = open('SimsDesc.txt')
for simDesc_line in SimsDesc:
    
    params = params_extractor(simDesc_line)
    directory = "Results-Sim" + str(params['sim_num'])
    logfiles = os.listdir(directory)
    
    direc = directory + "/"
    
    lumos_data = LogData(direc + "LumosFlows.txt", direc + "LumosQueueSize.txt", params)
    solstice_data = LogData(direc + "SolsticeFlows.txt", direc + "SolsticeQueueSize.txt", params)
            
    sims.append( {'params':params, 'Lumos': lumos_data, 'Solstice': solstice_data} )
    
SimsDesc.close()    
    
for num1, scheduler in enumerate(['Lumos', 'Solstice']):
    for num2, switch_radix in enumerate([16, 32, 64, 128, 256]):
        for sim in sims:
            if sim['params']['switch_radix'] == switch_radix:
                sim[scheduler].displayFlows(5*num1 + num2, scheduler + ' FCT Distribution, (Switch Radix ' + str(switch_radix) + ")")
                sim[scheduler].displayQueueSize(5*num1 + num2 + 10, scheduler + ' DCT in ConfigQueue, (Switch Radix ' + str(switch_radix) + ")")
             
