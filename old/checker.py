output1 =[]
output2 =[]
ris = []
f1 = "submition1.c"
f2 = "main.c"
with open(f1) as f:
    output1 = f.readlines()

with open(f2) as f:
    output2 = f.readlines()

passed = True
x2 = 0
target = 0
for x1 in range(len(output1)):
    if output1[x1] == output2[x2]:
        ris.append(";")
    else:
        newLine = True
        for y in range(x2,len(output2)):
            if output1[x1] == output2[y]:
                target = y
                ris.append(";")
                newLine= False
                break
        if newLine:
            ris.append(str(output1[x1])+";")
        else:
            for y in range(x2,target+1):
                ris.append(";"+str(output2[y]))
print(ris)
