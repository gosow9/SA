import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

def avfilter(x, M): #M immer ungerade
    avfilter = np.ones(M)/M
    av_x = np.convolve(x, avfilter)
    return av_x[int((M-1)/2):np.size(av_x)-int((M-1)/2)]

#plt.style.use('ggplot')

data = pd.read_csv('dlog3.csv')

s = pd.DataFrame.to_numpy(data.s).reshape(-1,1)[:,0]
v = pd.DataFrame.to_numpy(data.v).reshape(-1,1)[:,0]
i = pd.DataFrame.to_numpy(data.i).reshape(-1,1)[:,0] 
#volt2 = pd.DataFrame.to_numpy(data.Volt2).reshape(-1,1)[:,0]
#volt3 = pd.DataFrame.to_numpy(data.Volt3).reshape(-1,1)[:,0]
#volt4 = pd.DataFrame.to_numpy(data.Volt4).reshape(-1,1)[:,0]

#Mitteln (ÜBERGABEPARAMETER = 1 entspricht keine mittelung)
v = avfilter(v, 9)
i = avfilter(i, 9)



#zuschneiden (l,r für links und recht abschneiden)
l = 0 
r = 0
org_len = np.size(s)

s = s[l:org_len-r]
v = v[l:org_len-r]
i = i[l:org_len-r]


# use LaTeX fonts in the plot
plt.rcParams['font.family'] = 'STIXGeneral'
plt.rcParams.update({'font.size': 12})

#plot
#plt.plot(s, v)
plt.plot(s, i)
plt.plot(s, v)

plt.tight_layout()

#fig.savefig('entladen.pdf')