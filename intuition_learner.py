# import libraries
from keras.models import Sequential
from keras.layers import Dense, Conv2D, Flatten, Input
from keras.optimizers import Adam
import matplotlib.pyplot as plt
import numpy as np
import math
from sklearn.model_selection import train_test_split

# get the labels
with open('values.txt') as f:
    lines = f.read().splitlines()

values = np.array([min(max(-3500, float(val)), 3500) for val in lines[:30000]])

print(len(values))
# get the inputs
with open('grids.txt') as f:
    lines = f.read().splitlines()

vals = np.array([val.split(" ")[:-1] for val in lines[:30000]])
grids = np.array([np.array([float(v)/2 for v in line]).reshape((8, 8)) for line in vals])

grids = grids[..., np.newaxis]
print(grids[0])

model = Sequential()
#model.add(Conv2D(filters=4, kernel_size=2, padding='same', activation='relu'))
model.add(Conv2D(filters=4, kernel_size=3, padding='same', activation='relu'))
model.add(Flatten())
#model.add(Dense(128, activation='relu'))
model.add(Dense(1024, activation='relu'))
model.add(Dense(1024, activation='relu'))
model.add(Dense(1024, activation='relu'))
#model.add(Dense(64, activation='relu'))
#model.add(Dense(25))
#model.add(Dense(25))
#model.add(Dense(25))
#model.add(Dense(32, activation='relu'))
#model.add(Dense(16, activation='relu'))
#model.add(Dense(8, activation='relu'))
#model.add(Dense(4, activation='relu'))
#model.add(Dense(2, activation='relu'))
model.add(Dense(1))

model.build(input_shape=np.shape(grids))
model.summary()

# Gradient descent algorithm
adam = Adam(0.005)

X_train, X_test, y_train, y_test = train_test_split(grids, values, test_size=0.10)

model.compile(loss='mse', optimizer=adam, metrics=["accuracy","mse","cosine_similarity"])
history = model.fit(X_train, y_train, epochs=15, validation_split = 0.2, shuffle= True)
plt.plot(history.history['loss'])
plt.plot(history.history['val_loss'])
plt.legend(['loss','validation loss'])
plt.show()


predict = np.array(X_test)
predictions = model.predict(predict)

y_predict = [math.floor(prediction[0]) for prediction in predictions]

#plt.plot(100*(np.array(y) - np.array(y_predict))/np.array(y), "bo")
#ax = plt.gca()
#ax.set_ylim([-100, 100])
#plt.show()

r = [(math.floor(predictions[i][0]), math.floor(y_test[i])) for i in range(len(y_test))]
for i in range(100):
    print(r[i])