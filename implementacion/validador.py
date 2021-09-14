import sys
import random

filePath = "Instances\instancia"+str(sys.argv[1])+".mcsb"

class Truck:
    'Base Truck instance'
    def __init__(self, id, cap):
        self.id = id
        self.cap = cap

    def displayId(self):
        print(self.id)

    def displayCap(self):
        print(self.cap)

    def displayAll(self):
        print(self.id,self.cap)

class Node:
    'Base Farm instance'

    def __init__(self, id, cap, q):
        self.id = id
        self.cap = cap
        self.q = q

    def displayId(self):
        print(self.id)

    def displayCap(self):
        print(self.cap)

    def displayq(self):
        print(self.q)

    def displayAll(self):
        print(self.id,self.cap,self.q)

class Instance:
    'Base read instance'
    trucks = []
    nodes = []

    def __init__(self, filePath):
        f = open(filePath)
        data = f.readlines()
        toread = ["Nodos:=","Q:=","P:=","qu :="]
        for line in data:
            if "Nodos:=" in line:
                self.n = int(line.split("Nodos:=")[1].strip().strip(";").replace("\t"," ").split(" ")[-1])
            if "K:=" in line:
                k = int(line.split("K:=")[1].strip().strip(";").replace("\t"," ").split(" ")[-1])
        for i, s in enumerate(data):
            if "Q:=" in s:
                truck = list(map(lambda st: str.replace(st,"\t"," ").strip(), data[i+1:i+1+k]))
                for i in truck:
                    if len(i.split(" ")) > 2:
                        self.trucks.append(Truck(int(i.split(" ")[0]),int(i.split(" ")[-1])))
            if "P:=" in s:
                self.qualities = [int(i) for i in list(map(lambda st: str.replace(st,"\t"," ").strip(), data[i+1:i+4]))]
            if "qu :=" in s:
                nodes = list(map(lambda st: str.replace(st,"\t"," ").strip(), data[i+2:i+2+self.n]))
                for i in nodes:
                    temp = i.split(" ")
                    temp = list(filter(('').__ne__,temp))
                    self.nodes.append(Node(int(temp[0]),int(temp[1]),int(temp[2])-1))

    def displayN(self):
        print(self.n)

    def displayTrucks(self):
        for i in self.trucks:
            i.displayAll()

    def displayQualities(self):
        print(self.qualities)

    def displayNodes(self):
        for i in self.nodes:
            i.displayAll()
    def retTrucks(self):
        return(self.trucks)

inst = Instance(filePath)
'''
print("Numero de Nodos: ")
inst.displayN()
print("Camiones: ")
inst.displayTrucks()
print("Calidades: ")
inst.displayQualities()
print("Nodos: ")
inst.displayNodes()
'''

q1 = []
q2 = []
q3 = []
visited = []

for i in inst.nodes:
    if i.q == 1:
        q1.append((i.id,i.cap))

random.seed(18)
k1 = random.sample(q1,random.randint(0,len(q1)))

tot1 = 0
for i,j in k1:
    visited.append(i)
    tot1 += j

camiones = []
k = len(inst.trucks)
for i in range(1,k):
    ruta = []
    for j in inst.nodes:
        if j.id not in visited and random.random() <0.15:
            visited.append(j.id)
            ruta.append((j.id,j.cap,j.q))
    camiones.append(ruta)

for i,j in k1:
    print(str(i),end =" ")
print(";", tot1,0)

entregado = [tot1,0,0]

for i in camiones:
    tot = 0
    maxq = 0
    for j,k,l in i:
        if l > maxq:
            maxq = l
        tot += k
        print(str(j),end =" ")
    entregado[maxq] += tot
    print(";", tot,maxq)
for i in range(3):
    print("Demanda de Leche de tipo",i,":",inst.qualities[i],"litros")

for i in range(3):
    print("Total recibido de calidad",i,":",entregado[i],"litros")


print("Beneficio total de leche recibida:",entregado[0]*0.03+entregado[1]*0.021+entregado[2]*0.009)
