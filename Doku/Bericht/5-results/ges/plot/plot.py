import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

def avfilter(x, M): #M immer ungerade
    avfilter = np.ones(M)/M
    av_x = np.convolve(x, avfilter)
    return av_x[int((M-1)/2):np.size(av_x)-int((M-1)/2)]


data = pd.read_csv('scope_27.csv')


t = pd.DataFrame.to_numpy(data.second).reshape(-1,1)[:,0]
v_bat = pd.DataFrame.to_numpy(data.Volt1).reshape(-1,1)[:,0]
v_step = pd.DataFrame.to_numpy(data.Volt2).reshape(-1,1)[:,0]
v_ldo = pd.DataFrame.to_numpy(data.Volt3).reshape(-1,1)[:,0]
v_disp = pd.DataFrame.to_numpy(data.Volt4).reshape(-1,1)[:,0]




#Mitteln (ÜBERGABEPARAMETER = 1 entspricht keine mittelung)
v_bat = avfilter(v_bat, 1)
v_step = avfilter(v_step, 1)
v_ldo = avfilter(v_ldo, 1)
v_disp = avfilter(v_disp, 1)


#zuschneiden (l,r für links und recht abschneiden)
l = 0
r = 20
org_len = np.size(t)

t = t[l:org_len-r]
v_bat = v_bat[l:org_len-r]
v_step = v_step[l:org_len-r]
v_ldo = v_ldo[l:org_len-r]
v_disp = v_disp[l:org_len-r]


# use LaTeX fonts in the plot
plt.rcParams['font.family'] = 'STIXGeneral'
plt.rcParams.update({'font.size': 12})

#plot
plt.plot(t, v_bat, label='$V_{\mathbf{BAT}}$')
plt.plot(t, v_step, label='$V_{\mathbf{BOOT}}$')
plt.plot(t, v_ldo, label='$V_{\mathbf{LDO}}$')
plt.plot(t, v_disp, label='$V_{\mathbf{DISP}}$')
plt.grid(True)

plt.tight_layout()
plt.legend(bbox_to_anchor=(0.7, 0.5))

#plt.savefig('vboot.pdf')