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
    if i.q == 0:
        q1.append((i.id,i.cap,0))
    if i.q == 1:
        q2.append((i.id,i.cap,1))
    if i.q == 2:
        q3.append((i.id,i.cap,2))

random.seed(18)
random.shuffle(q1)
random.shuffle(q2)
random.shuffle(q3)

maxq1 = sum(x[1] for x in q1)
maxq2 = sum(x[1] for x in q2)
maxq3 = sum(x[1] for x in q3)
print("Total leche de calidad 0 en predios:",maxq1)
print("Total leche de calidad 1 en predios:",maxq2)
print("Total leche de calidad 2 en predios:",maxq3)

camiones = []
tot1 = 0
for i in range(len(inst.trucks)):
    if tot1 < inst.qualities[0] and tot1 != maxq1:
        t1 = 0
        ruta = []
        for i,j,k in q1:
            if t1 + j <= inst.trucks[0].cap and i not in visited:
                visited.append(i)
                ruta.append((i,j,k))
                t1 += j
        tot1 += t1
        if len(ruta) > 0:
            camiones.append(ruta)
tot2 = 0
for i in range(len(camiones),len(inst.trucks)):
    if tot2 < inst.qualities[1] and tot2 != maxq2:
        t2 = 0
        ruta = []
        q12 = q1+q2
        for i,j,k in q12:
            if t2 + j <= inst.trucks[0].cap and i not in visited:
                visited.append(i)
                ruta.append((i,j,k))
                t2 += j
                c1=0
                c2=0
                for x,y,z in ruta:
                    if z == 0:
                        c1 += y
                    if z == 1:
                        c2 += y
                    print(x,end =" ")
                print("; 0:",c1," 1:",c2,"pérdida: ",c1*0.021-c1*0.03)
        tot2 += t2
        if len(ruta) > 0:
            camiones.append(ruta)

for i in range(len(camiones),len(inst.trucks)):
    ruta = []
    for j,k,l in q1+q2+q3:
        if j not in visited and sum(x[1] for x in ruta) <= inst.trucks[i].cap:
            visited.append(j)
            ruta.append((j,k,l))
            c1=0
            c2=0
            c3=0
            for x,y,z in ruta:
                if z == 0:
                    c1 += y
                if z == 1:
                    c2 += y
                if z == 2:
                    c3 += y
                print(x,end =" ")
            print("; 0:",c1," 1:",c2," 2:",c3,"pérdida: ",c1*0.009-c1*0.03 + c2*0.009-c2*0.021 )
    camiones.append(ruta)


entregado = [0,0,0]
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
