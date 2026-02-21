import csv
import matplotlib.pyplot as plt

#Define the axis of the graphs
tComponent = []
sComponent = []
vComponent = []
aComponent = []

with open ('equationData.csv', newline='') as csvfile:
    equationReader = csv.reader(csvfile, delimiter=',')
    next(equationReader) # Skip the header row
    for row in equationReader: # Get data from corresponding row
        tComponent.append(float(row[0]))
        sComponent.append(float(row[1]))
        vComponent.append(float(row[2]))
        aComponent.append(float(row[3]))

#Plot the points of each equation on the graph
plt.plot(tComponent, sComponent)
plt.plot(tComponent, vComponent)
plt.plot(tComponent, aComponent)
plt.legend(['S(t) - Position', 'V(t) - Velocity', 'A(t) - Acceleration'])
plt.xlabel('Time (s)')
plt.ylabel('Position (m), Velocity (m/s), Acceleration (m/s^2)')
plt.show()
