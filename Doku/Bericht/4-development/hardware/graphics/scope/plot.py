import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

def avfilter(x, M): #M immer ungerade
    avfilter = np.ones(M)/M
    av_x = np.convolve(x, avfilter)
    return av_x[int((M-1)/2):np.size(av_x)-int((M-1)/2)]

#plt.style.use('ggplot')

data = pd.read_csv('scope_20.csv')

t = pd.DataFrame.to_numpy(data.second).reshape(-1,1)[:,0]
volt1 = pd.DataFrame.to_numpy(data.Volt1).reshape(-1,1)[:,0]
#volt2 = pd.DataFrame.to_numpy(data.Volt2).reshape(-1,1)[:,0]
#volt3 = pd.DataFrame.to_numpy(data.Volt3).reshape(-1,1)[:,0]
#volt4 = pd.DataFrame.to_numpy(data.Volt4).reshape(-1,1)[:,0]
#volt2 = pd.DataFrame.to_numpy(data.Volt2).reshape(-1,1)[:,0]
#volt3 = pd.DataFrame.to_numpy(data.Volt3).reshape(-1,1)[:,0]
#volt4 = pd.DataFrame.to_numpy(data.Volt4).reshape(-1,1)[:,0]

#Mitteln (ÜBERGABEPARAMETER = 1 entspricht keine mittelung)
volt1 = avfilter(volt1, 1)
#volt2 = avfilter(volt2, 11)
#volt3 = avfilter(volt3, 11)
#volt4 = avfilter(volt4, 11)


#zuschneiden (l,r für links und recht abschneiden)
l = 0 
r = 0
org_len = np.size(t)

t = t[l:org_len-r]
volt1 = volt1[l:org_len-r]
#volt2 = volt2[l:org_len-r]
#volt3 = volt3[l:org_len-r]
#volt4 = volt4[l:org_len-r]

# use LaTeX fonts in the plot
plt.rcParams['font.family'] = 'STIXGeneral'
plt.rcParams.update({'font.size': 12})

#plot
plt.plot(t, volt1)
#plt.plot(t, volt2)
#plt.plot(t, volt3)
#plt.plot(t, volt4)
#plt.plot(t, volt2)
#plt.plot(t, volt3)
#plt.plot(t, volt4)


plt.tight_layout()

#fig.savefig('entladen.pdf')