import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

def avfilter(x, M): #M immer ungerade
    avfilter = np.ones(M)/M
    av_x = np.convolve(x, avfilter)
    return av_x[int((M-1)/2):np.size(av_x)-int((M-1)/2)]


data1 = pd.read_csv('scope_14.csv')
data2 = pd.read_csv('scope_15.csv')
data3 = pd.read_csv('scope_16.csv')
data4 = pd.read_csv('scope_17.csv')

t = pd.DataFrame.to_numpy(data1.second).reshape(-1,1)[:,0]
volt1 = pd.DataFrame.to_numpy(data1.Volt).reshape(-1,1)[:,0]

volt2 = pd.DataFrame.to_numpy(data2.Volt).reshape(-1,1)[:,0]

volt3 = pd.DataFrame.to_numpy(data3.Volt).reshape(-1,1)[:,0]

volt4 = pd.DataFrame.to_numpy(data4.Volt).reshape(-1,1)[:,0]



#Mitteln (ÜBERGABEPARAMETER = 1 entspricht keine mittelung)
volt1 = avfilter(volt1, 15)
volt2 = avfilter(volt2, 1)
volt3 = avfilter(volt3, 1)
volt4 = avfilter(volt4, 1)


#zuschneiden (l,r für links und recht abschneiden)
l = 0
r = 20
org_len = np.size(t)

t = t[l:org_len-r]
volt1 = volt1[l:org_len-r]
volt2 = volt1[l:org_len-r]
volt3 = volt1[l:org_len-r]
volt4 = volt1[l:org_len-r]


# use LaTeX fonts in the plot
plt.rcParams['font.family'] = 'STIXGeneral'
plt.rcParams.update({'font.size': 12})

#plot

plt.plot(t, volt1, label='$V_{\mathbf{boot}}$', color='tab:blue')
plt.xlim([-1,4])
plt.ylim([0,4])
plt.grid(True)

plt.tight_layout()
plt.legend(bbox_to_anchor=(0.93, 0.5))

plt.savefig('vboot.pdf')