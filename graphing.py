import csv
import matplotlib.pyplot as plt
import numpy as np

xpoint = []
ypoint = []

with open ('out/build/Clang++/equationData.csv', newline='') as csvfile:
    equationReader = csv.reader(csvfile, delimiter=',')
    next(equationReader) # Skip the header row
    for row in equationReader:
        xpoint.append(float(row[1]))
        ypoint.append(float(row[0]))


plt.plot(xpoint, ypoint)
plt.show()
