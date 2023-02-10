import matplotlib.pyplot as plt

with open("White_evol.txt","r") as f:
    lines = f.readlines()

seed = int(lines[0])
data = [[] for i in range(13)]

for line in lines[1:-1]:
    d = line.split('\n')[0].split(',')
    for i,val in enumerate(d):  
        data[i].append(float(val))


colors = ["black","lightgrey","lightgrey","grey","lightgrey","lightgrey","red","lightgrey","lightgrey","grey","lightgrey","lightgrey"]
widths = [5,1,1,2,1,1,5,1,1,2,1,1]
labels = ["Best so far","Best","Top 10%", "Top 20%", "Top 30%", "Top 40%","Top 50%","Top 60%","Top 70%","Top 80%","Top 90%","Worst"]

for i in range(12):
    if (i in {0,3,6,9}):
        plt.plot(data[0],data[i+1],color = colors[i], linewidth = widths[i],label = labels[i])
    else:
        plt.plot(data[0],data[i+1],color = colors[i], linewidth = widths[i])

plt.title(f"Genetic evolution of evaluation. seed: {seed}")
plt.xlabel("Generation")
plt.ylabel("Score")
plt.legend()
plt.show()
