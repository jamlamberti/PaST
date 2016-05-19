import os
from collections import defaultdict
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import subprocess
def call_past(model_file, scale_alloc=True):
    cmd = ['./bin/main', os.path.abspath(model_file)]
    if scale_alloc:
        cmd = ['LD_PRELOAD=/home/jlamberti/os_repos/Hoard/src/libhoard.so'] + cmd
    p = subprocess.Popen(['./bin/main', os.path.abspath(model_file)], stdout=subprocess.PIPE, stdin=subprocess.PIPE)
    out, err = p.communicate()
    return out.strip().split('\n')[-1].strip().split()[0]

model_file = 'models/test_model.mod'
new_model = 'models/exp.mod'
threads = [1, 2, 3, 4]
#steps = [5, 10, 25, 100, 500, 1000]
#traces = [5, 10, 25, 100, 250, 500, 1000, 5000, 10000]
steps = [250*cnt for cnt in range(1, 5)]
traces = [250*cnt for cnt in range(1, 5)]
iters = 5
model = []
with open(model_file, 'r') as f:
    model = f.readlines()
data = defaultdict(list)

for _ in range(iters):
    for trace in traces:
        for step in steps:
            for thread in threads:
                with open(new_model, 'w') as f:
                    f.write("\n".join(model))
                    f.write('numsteps = %d;\nnumtraces = %d;\nnumthreads = "%d";'%(step, trace, thread))
                t = int(call_past(new_model))
                print t
                data[(trace, step, thread)].append(t)

print data

# Compute averages
for k, v in data.items():
    data[k] = np.log10(np.median(v))

fig = plt.figure(figsize=(20,10))
ax = fig.add_subplot(111, projection='3d')
#ax.set_zscale('log')
colors = ['b', 'r', 'g', 'y']
for thread in threads:
    xs = []
    ys = []
    zs = []
    for step in steps:
        for trace in traces:
            xs.append(step)
            ys.append(trace)
            zs.append(data[(trace, step, thread)])
    ax.scatter(xs, ys, zs, c=colors[thread-1], label='%d threads'%thread)
ax.set_xlabel('# of Steps')
ax.set_ylabel('# of Traces')
ax.set_zlabel('Time log(ms)')
ax.legend(numpoints=1)
plt.savefig('out.png')
