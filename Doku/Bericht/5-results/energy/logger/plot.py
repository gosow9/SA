import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

def avfilter(x, M): #M immer ungerade
    avfilter = np.ones(M)/M
    av_x = np.convolve(x, avfilter)
    return av_x[int((M-1)/2):np.size(av_x)-int((M-1)/2)]

#plt.style.use('ggplot')

data = pd.read_csv('dlog2.csv')

s = pd.DataFrame.to_numpy(data.s).reshape(-1,1)[:,0]
v = pd.DataFrame.to_numpy(data.v).reshape(-1,1)[:,0]
i = pd.DataFrame.to_numpy(data.i).reshape(-1,1)[:,0] 

#Mitteln (ÜBERGABEPARAMETER = 1 entspricht keine mittelung)
v = avfilter(v, 1)
i = avfilter(i, 17)


#zuschneiden (l,r für links und recht abschneiden)
l = 3100
r = 18800
org_len = np.size(s)

s = s[l:org_len-r]
v = v[l:org_len-r]
i = i[l:org_len-r]

#Zeitvetor erstellen
T = 1.00352/1000
t = np.arange(0, np.size(s))*T


# use LaTeX fonts in the plot
plt.rcParams['font.family'] = 'STIXGeneral'
plt.rcParams.update({'font.size': 12})


#plots von i und u
#plot i
fig1, ax1 = plt.subplots()
ax1.set_xlabel(r'Time (s)')
ax1.set_ylabel(r'Current (mA)', color='tab:red')
ax1.set_xlim([0,8])
ax1.set_ylim([0, 400])
ax1.plot(t, i*1000, label=r'$i(t)$', color='tab:red')

plt.grid(True)

#plot v
ax2 = ax1.twinx()
ax2.set_ylabel(r'Voltage (V)', color='tab:blue')
ax2.set_ylim([0, 4])
ax2.plot(t, v, label=r'$u(t)$', color='tab:blue')

fig1.legend(bbox_to_anchor=(0.5, 0.7))
fig1.tight_layout()
fig1.show()

fig1.savefig('ui.pdf')

#plot von p
p = v*i
E = np.trapz(p, dx=T)

print('Consumed energy: '+ str(E) + ' Ws')

fig2= plt.figure()
plt.plot(t, p, label=r'P(t)', color='tab:orange')
plt.xlabel(r'Time (s)')
plt.ylabel(r'Power (W)')
plt.xlim([0,8])
plt.ylim([0, 1.4])

plt.grid(True)

fig2.savefig('p.pdf')