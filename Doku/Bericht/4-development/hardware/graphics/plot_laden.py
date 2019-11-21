import matplotlib.pyplot as plt
import matplotlib.dates as md
import numpy as np
import pandas as pd
from datetime import datetime


def avfilter(x, M): #M immer ungerade
    avfilter = np.ones(M)/M
    av_x = np.convolve(x, avfilter)
    return av_x[int((M-1)/2):np.size(av_x)-int((M-1)/2)]

#plt.style.use('ggplot')

data = pd.read_csv('ladeverhalten.txt')

t = pd.DataFrame.to_numpy(data.t).reshape(-1,1)[:,0]
t_rel = pd.DataFrame.to_numpy(data.t_rel).reshape(-1,1)[:,0]
V_sys = pd.DataFrame.to_numpy(data.V_sys).reshape(-1,1)[:,0]
V_bat = pd.DataFrame.to_numpy(data.V_bat).reshape(-1,1)[:,0]
V_in = pd.DataFrame.to_numpy(data.V_in).reshape(-1,1)[:,0]
lux = pd.DataFrame.to_numpy(data.lux).reshape(-1,1)[:,0]

#Mitteln
V_sys = avfilter(V_sys, 33)
V_bat = avfilter(V_bat, 33)
V_in = avfilter(V_in, 33)
lux = avfilter(lux, 33)


#zuschneiden
l = 20
r = 20
org_len = np.size(t_rel)

t = t[l:org_len-r]
t_rel = t_rel[l:org_len-r]
V_sys = V_sys[l:org_len-r]
V_bat = V_bat[l:org_len-r]
V_in = V_in[l:org_len-r]
lux = lux[l:org_len-r]

#grössen reduzieren (für tikz-file)
#index = np.arange(0, len(t), 2)
#
#t = np.delete(t, index)
#t_rel = np.delete(t_rel, index)
#V_sys = np.delete(V_sys, index)
#V_bat = np.delete(V_bat, index)
#V_in = np.delete(V_in, index)
#lux = np.delete(lux, index)

#t zu date_time_obj
t_obj = [datetime.strptime(t[i], '%Y-%m-%d %H:%M:%S.%f') for i in range(np.size(t))]

# use LaTeX fonts in the plot
plt.rcParams['font.family'] = 'STIXGeneral'

#plot V_sys, V_bat, Vin
fig, ax1 = plt.subplots()
ax1.set_xlabel('Time')
ax1.set_ylabel('Voltage (V)')
ax1.set_ylim([0, 4])
ax1.plot(t_obj, V_sys, label=r'$V_{sys}$', color='tab:red')
ax1.plot(t_obj, V_bat, label=r'$V_{bat}$')
ax1.plot(t_obj, V_in, label='$V_{in}$')

#plot lux
ax2 = ax1.twinx()
ax2.set_ylabel('Illuminance (lux)', color='tab:green')
ax2.set_ylim([0, 1000])
ax2.tick_params(axis='y', labelcolor='tab:green')
ax2.plot(t_obj, lux, label=r'$E_v$', color='tab:green')

#zeitachse
ax1.xaxis.set_major_locator(md.HourLocator(interval=2))
ax1.xaxis.set_major_formatter(md.DateFormatter('%H:%S'))
ax1.grid(True)
plt.setp(ax1.xaxis.get_majorticklabels(), rotation=60)
ax1.set_xlim([t_obj[0], t_obj[len(t_obj)-1]])

fig.legend(bbox_to_anchor=(0.27, 0.42))

plt.tight_layout()

fig.savefig('laden.pdf')