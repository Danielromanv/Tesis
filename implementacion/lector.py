filePath = "Instances\instancia1.mcsb"

class Truck:
    'Base Truck instance'
    def __init__(self, id, cap):
        self.id = id
        self.cap = cap

    def displayId(self):
        print(self.id)

    def displayCap(self):
        print(self.cap)

    def displayUsed(self):
        print(self.used)

    def displayAll(self):
        print(self.id,self.cap,self.used)

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
                    self.trucks.append(Truck(i.split(" ")[0],i.split(" ")[-1]))
            if "P:=" in s:
                self.qualities = [int(i) for i in list(map(lambda st: str.replace(st,"\t"," ").strip(), data[i+1:i+4]))]
            if "qu :=" in s:
                nodes = list(map(lambda st: str.replace(st,"\t"," ").strip(), data[i+2:i+2+self.n]))
                for i in nodes:
                    temp = i.split(" ")
                    temp = list(filter(('').__ne__,temp))
                    self.nodes.append(Node(temp[0],temp[1],temp[2]))

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
inst.displayN()
inst.displayTrucks()
inst.displayQualities()
inst.displayNodes()
