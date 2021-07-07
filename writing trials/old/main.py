for fileNumber in range(1,7):
    output = []
    with open("./-h/input/open/input."+str(fileNumber)) as f:
        ctx = f.readlines()
        data = ctx[0].split(" ")
        numberNode = data[0]
        topK = int(data[1])
        i = 1
        listaTopK = []
        listaTopKIndex = []
        numbergraph = 0
        while i<len(ctx):
            line = ctx[i]
            i+=1
            if line=="AggiungiGrafo\n":                
                # print("new graph")
                keynode = 0
                graphIndex = []
                graphCosts = []
                while ctx[i][0].isnumeric():
                    support = ctx[i][0:-1].split(",")
                    graphIndex.append([int(x) for x in range(0,len(support)) if support[x]!='0' and keynode!=x])  
                    graphCosts.append([int(support[x]) for x in range(0,len(support)) if support[x]!='0' and keynode!=x])               
                    # print("Node",keynode,graphIndex[keynode],graphCosts[keynode])
                    i+=1  
                    keynode+=1
                nonVisited = []
                dist = []
                prev = []
                Q = []
                for v in range(0,len(graphIndex)):
                    nonVisited.append(0)
                    dist.append(9999999999999999999)
                    prev.append(-1)
                    Q.append(v)
                dist[0]=0
                thisIteration=-1
                while len(Q)!=0:
                    thisIteration+=1
                    # print(Q)
                    minimum = dist[Q[0]]
                    u = 0
                    for x in Q:
                        # print(dist[x],minimum)
                        if dist[x]<minimum:
                            minimum = dist[x]
                            u = x
                    if (u==0 and thisIteration>0):
                        u=Q[0]
                    Q.remove(u)
                    for x in range(0,len(graphIndex[u])):
                        if graphIndex[u][x] in Q:
                            alt = dist[u] + graphCosts[u][x]
                            # print(u,"->",graphIndex[u][x],"cost",alt)
                            if alt < dist[graphIndex[u][x]]:
                                nonVisited[graphIndex[u][x]] += 1
                                dist[graphIndex[u][x]] = alt
                                prev[graphIndex[u][x]] = u
                summ = dist[0]  
                for x in range(0,len(nonVisited)):
                    if nonVisited[x] > 0:
                        summ+=dist[x]
                # summ = sum(dist)
                lengthT = len(listaTopK)
                if (len(listaTopK)<topK):
                    listaTopK.append([numbergraph,summ])
                else:
                    maximum = 0
                    index = 0
                    for x in range(0,len(listaTopK)):
                        if maximum<=listaTopK[x][1]:
                            maximum = listaTopK[x][1]
                            index = x
                    if (summ < maximum):
                        listaTopK[index]= [numbergraph,summ]
                # print(numbergraph,"->",summ)
                numbergraph+=1
                # exit()
            elif line=="TopK\n":
                # print("evaluation")
                toPrint = []
                for x in listaTopK:
                    toPrint.append(x[0])
                toPrint.sort()
                output.append(str(toPrint).replace("[","").replace("]","").replace(",","") +"\n")
                pass
            else:
                pass
    print("Input",fileNumber)
    print(output)
    # with open("output_"+str(fileNumber))as f:
    #     ctx = f.readlines()
    #     passed = True
    #     for x in range(len(ctx)):
    #         if ctx[x] != output[x]:
    #             passed = False
    #             break
    #     if passed:
    #         print("File",fileNumber,"passed")


