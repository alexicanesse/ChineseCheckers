import matplotlib.pyplot as plt

with open("White_evol.txt","r") as f:
    lines = f.readlines()


general_data = lines.pop(0).split('\n')[0].split(',')
seed = int(general_data[0])
p_mutation = float(general_data[1])
sigma_mutation = float(general_data[2])
pop_size = int(general_data[3])
ab_depth = int(general_data[4])
num_generation = int(general_data[5])
round_length = int(general_data[6])

data = [[] for i in range(13)]

for line in lines:
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

plt.suptitle("Genetic evolution of evaluation.")
plt.title(f" seed: {seed} , p_mutation: {p_mutation}, sigma_mutation: {sigma_mutation}, pop_size: {pop_size}, ab_depth: {ab_depth}, num_generation: {num_generation}, round_length: {round_length}")
plt.xlabel("Generation")
plt.ylabel("Score")
plt.legend()
plt.show()
