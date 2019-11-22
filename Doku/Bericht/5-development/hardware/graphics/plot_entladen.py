import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

def avfilter(x, M): #M immer ungerade
    avfilter = np.ones(M)/M
    av_x = np.convolve(x, avfilter)
    return av_x[int((M-1)/2):np.size(av_x)-int((M-1)/2)]

#plt.style.use('ggplot')

data = pd.read_csv('entladeverhalten1.txt')

t = pd.DataFrame.to_numpy(data.t).reshape(-1,1)[:,0]
t_rel = pd.DataFrame.to_numpy(data.t_rel).reshape(-1,1)[:,0]
V_sys = pd.DataFrame.to_numpy(data.V_sys).reshape(-1,1)[:,0]
V_bat = pd.DataFrame.to_numpy(data.V_bat).reshape(-1,1)[:,0]
V_in = pd.DataFrame.to_numpy(data.V_in).reshape(-1,1)[:,0]
lux = pd.DataFrame.to_numpy(data.lux).reshape(-1,1)[:,0]

#Mitteln
V_sys = avfilter(V_sys, 1)
V_bat = avfilter(V_bat, 1)
V_in = avfilter(V_in, 1)
lux = avfilter(lux, 1)


#zuschneiden
l = 0
r = 0
org_len = np.size(t_rel)

t = t[l:org_len-r]
t_rel = t_rel[l:org_len-r]
V_sys = V_sys[l:org_len-r]
V_bat = V_bat[l:org_len-r]
V_in = V_in[l:org_len-r]
lux = lux[l:org_len-r]

# use LaTeX fonts in the plot
plt.rcParams['font.family'] = 'STIXGeneral'

#plot V_sys, V_bat, Vin
fig, ax1 = plt.subplots()
ax1.set_xlabel(r'Time (s)')
ax1.set_ylabel(r'Voltage (V)')
ax1.set_ylim([0, 4])
ax1.plot(t_rel, V_sys, label=r'$V_{sys}$', color='tab:red')
ax1.plot(t_rel, V_bat, label='$V_{bat}$')
ax1.plot(t_rel, V_in, label='$V_{in}$')
ax1.set_xlim([t_rel[0], t_rel[len(t_rel)-1]])


plt.grid(True)

#plot lux
ax2 = ax1.twinx()
ax2.set_ylabel(r'Illuminance (lux)', color='tab:green')
ax2.set_ylim([0, 1000])
ax2.tick_params(axis='y', labelcolor='tab:green')
ax2.plot(t_rel, lux, label=r'$E_v$', color='tab:green')

fig.legend(bbox_to_anchor=(0.3, 0.4))

plt.tight_layout()

fig.savefig('entladen.pdf')