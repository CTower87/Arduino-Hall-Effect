import pandas as pd
import matplotlib.pyplot as plt
import scipy.optimize as optimize
import numpy as np
def func(x, a, b):
    val = (a*x) + b
    return val

#create dataframe
data = pd.DataFrame()
data['g'] = [0.0104,0.0226,0.0341,0.0467]
data['R'] = [1,1.3,1.5,2]

#Run linear regression
popt, pcov = optimize.curve_fit(func, data['g'], data['R'])

#calculate r^2
data['resid'] = data['R'] - func(data['g'], *popt)
data['resid^2'] = data['resid'] **2
ss_tot = np.sum((data['R']-np.mean(data['R']))**2)
ss_res = sum(data['resid^2'])
r_squared = (1 - (ss_res/ss_tot))

#plot data and regression line
xvlaues = np.linspace(0, .05, 500)

plt.plot(xvlaues, func(xvlaues, *popt), "r--", label = str(r_squared) + ' ' + str(popt))
plt.scatter(data['g'], data['R'])
plt.legend()
plt.show()



