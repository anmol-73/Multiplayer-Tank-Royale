import numpy as np
p0 = 0.05
p1 = 0.5
p2 = 0.45
map = []
for i in range(480*270):
    p = np.random.uniform(0,1,1)
    if(p<p0):
        map.append(0)
    elif(p0<=p and p<p1+p0):
        map.append(1)
    elif(p>=p1+p0):
        map.append(2)
print(map)