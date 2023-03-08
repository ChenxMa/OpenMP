import matplotlib.pyplot as plt
import os
import pandas as pd
import numpy as np

# threads1 = []
# bodies1 = []
# perfmance1 = []
# path1 = "D:\File\VSProjects\OpenMP\OpenMP\demo_seq.txt"
# with open(path1, "r") as p:
#     for row in p:
#         row = row.rstrip('\n')
#         data = row.split(',')
#         thr = float(data[0].split(':')[1])
#         bodi = float(data[1].split(':')[1])
#         perf = float(data[2].split(':')[1])
#         threads1.append(thr)
#         bodies1.append(bodi)
#         perfmance1.append(perf)
#
# threads2 = []
# bodies2 = []
# perfmance2 = []
# path2 = "D:\File\VSProjects\OpenMP\OpenMP\half_seq.txt"
# with open(path2, "r") as p:
#     for row in p:
#         row = row.rstrip('\n')
#         data = row.split(',')
#         thr = float(data[0].split(':')[1])
#         bodi = float(data[1].split(':')[1])
#         perf = float(data[2].split(':')[1])
#         threads2.append(thr)
#         bodies2.append(bodi)
#         perfmance2.append(perf)
#
# plt.scatter(bodies1, perfmance1, c='b', label='demo')
# plt.scatter(bodies2, perfmance2, c='g', label='half')
# plt.xlabel('Bodies')
# plt.ylabel('Performance')
# plt.title("Sequential performance in both program")
# plt.legend()
# plt.show()


threads3 = []
bodies3 = []
perfmance3 = []

path3 = "D:\File\VSProjects\OpenMP\OpenMP\demo_para_new.txt"
with open(path3, "r") as p:
    for row in p:
        row = row.rstrip('\n')
        data = row.split(',')
        thr = float(data[0].split(':')[1])
        bodi = float(data[1].split(':')[1])
        perf = float(data[2].split(':')[1])
        threads3.append(thr)
        bodies3.append(bodi)
        perfmance3.append(perf)
threads3 = [threads3[i:i+23] for i in range(0, len(threads3),23)]
bodies3 = [bodies3[i:i+23] for i in range(0, len(bodies3),23)]
perfmance3 = [perfmance3[i:i+23] for i in range(0, len(perfmance3),23)]
avg1 =[[] for i in range(4)]
avg2 =[[] for i in range(4)]
avg3 =[[] for i in range(4)]
for i in range(0,len(threads3)):
    for j in range(0, 20, 5):
        avg1[i].append(sum(threads3[i][j:j+5])/5)
        avg2[i].append(sum(bodies3[i][j:j + 5]) / 5)
        avg3[i].append(sum(perfmance3[i][j:j + 5]) / 5)
    for j in range(20, 23):
        avg1[i].append(threads3[i][j])
        avg2[i].append(bodies3[i][j])
        avg3[i].append(perfmance3[i][j])
p1 = plt.plot(avg2[0], avg3[0], color= 'red', label= 'Threads:1', marker= '.')
p2 = plt.plot(avg2[1], avg3[1], color= 'deeppink', label= 'Threads:2', marker= '.')
p3 = plt.plot(avg2[2], avg3[2], color= 'green', label= 'Threads:4', marker= '.')
p4 = plt.plot(avg2[3], avg3[3], color= 'blue', label= 'Threads:8', marker= '.')

plt.legend()
plt.xlabel('Bodies')
plt.ylabel('Performance')
plt.title('Parallel performance in demo_para_new.cpp')
plt.show()
