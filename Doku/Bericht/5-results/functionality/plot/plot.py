import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

def avfilter(x, M): #M immer ungerade
    avfilter = np.ones(M)/M
    av_x = np.convolve(x, avfilter)
    return av_x[int((M-1)/2):np.size(av_x)-int((M-1)/2)]


data = pd.read_csv('scope_27.csv')


t_r = pd.DataFrame.to_numpy(data.second).reshape(-1,1)[:,0]
v_bat = pd.DataFrame.to_numpy(data.Volt1).reshape(-1,1)[:,0]
v_step = pd.DataFrame.to_numpy(data.Volt2).reshape(-1,1)[:,0]
v_ldo = pd.DataFrame.to_numpy(data.Volt3).reshape(-1,1)[:,0]
v_disp = pd.DataFrame.to_numpy(data.Volt4).reshape(-1,1)[:,0]



#Mitteln (ÜBERGABEPARAMETER = 1 entspricht keine mittelung)
v_bat = avfilter(v_bat, 3)
v_step = avfilter(v_step, 3)
v_ldo = avfilter(v_ldo, 3)
v_disp = avfilter(v_disp, 3)


#zuschneiden (l,r für links und recht abschneiden)
l = 2500
r = 1500
org_len = np.size(t_r)

t_r = t_r[l:org_len-r]
v_bat = v_bat[l:org_len-r]
v_step = v_step[l:org_len-r]
v_ldo = v_ldo[l:org_len-r]
v_disp = v_disp[l:org_len-r]

#zeitaches erstellen
T = t_r[1]-t_r[0]
t = np.arange(0, np.size(t_r))*T

# use LaTeX fonts in the plot
plt.rcParams['font.family'] = 'STIXGeneral'
plt.rcParams.update({'font.size': 12})

#plot
plt.plot(t, v_bat, label='$V_{\mathbf{BAT}}$')
plt.plot(t, v_step, label='$V_{\mathbf{BOOT}}$')
plt.plot(t, v_ldo, label='$V_{\mathbf{LDO}}$', color='tab:red')
#plt.plot(t, v_disp, label='$V_{\mathbf{DISP}}$', color='tab:red')
plt.xlim([0, 12])
plt.ylim([0, 4])
plt.xlabel('Time (s)')
plt.ylabel('Voltage (V)')

plt.grid(True)

plt.tight_layout()
plt.legend(bbox_to_anchor=(0.75, 0.55))

plt.savefig('v.pdf')