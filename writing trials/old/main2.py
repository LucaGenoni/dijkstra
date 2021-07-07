# for fileNumber in range(0,7):
fileNumber=1
if True:
    output = []
    with open("input_"+str(fileNumber)) as f:
        ctx = f.readlines()
        topK = int(ctx[0].split(" ")[1])
        listaTopK = []
        listaTopKIndex = []
        numbergraph = 0
        i_line = 1
        while i_line<len(ctx):
            line = ctx[i_line]
            i_line+=1
            if line=="AggiungiGrafo\n":
                keynode = 0
                graphIndex = []
                graphCosts = []
                while ctx[i_line][0].isnumeric():
                    support = ctx[i_line][0:-1].split(",")
                    graphIndex.append([int(x) for x in range(0,len(support)) if support[x]!='0' and keynode!=x] )  
                    graphCosts.append([int(support[x]) for x in range(0,len(support)) if support[x]!='0' and keynode!=x])               
                    # print("Node",keynode,graphIndex[keynode],graphCosts[keynode])
                    i_line+=1  
                    keynode+=1
                # print(graphIndex)
                # print(graphCosts)
                # for v in range(len(graphIndex)):
                #     print(v,":",end=" ")
                #     for arcs in range(len(graphIndex[v])):
                #         print(f"{graphIndex[v][arcs]} ({graphCosts[v][arcs]})",end=", ")
                #     print()
                nonVisited = []
                dist = []
                # prev = []
                Q = []
                for v in range(0,len(graphIndex)):
                    nonVisited.append(0)
                    dist.append(999999999999999999999999)
                    # prev.append(-1)
                    Q.append(v)
                dist[0]=0
                while len(Q)!=0:
                    # print(Q)
                    minimum = dist[Q[0]]
                    u=Q[0]
                    for x in Q:
                        # print(dist[x],minimum)
                        if dist[x]<minimum:
                            minimum = dist[x]
                            u = x
                        
                    Q.remove(u)
                    # print("<",end="")
                    for x in range(0,len(graphIndex[u])):
                        if graphIndex[u][x] in Q:
                            alt = dist[u] + graphCosts[u][x]
                            # print(u,"->",graphIndex[u][x],"cost",alt,end=", ")
                            if alt < dist[graphIndex[u][x]]:
                                nonVisited[graphIndex[u][x]] += 1
                                dist[graphIndex[u][x]] = alt
                                # prev[graphIndex[u][x]] = u
                    # print(">",end="")
                summ = dist[0]
                print()
                print(dist[0], end=" ")
                for x in range(1,len(nonVisited)):
                    if nonVisited[x] > 0:
                        print("+",dist[x],end="")
                        summ+=dist[x]
                print()
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
                print(numbergraph,"->",summ)
                # if (numbergraph==1):
                #     exit()
                # else:
                numbergraph+=1
                print("#"*100," "*10,numbergraph)
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
    with open("output_"+str(fileNumber))as f:
        ctx = f.readlines()
        passed = True
        for x in range(len(ctx)):
            if ctx[x] != output[x]:
                passed = False
                break
        if passed:
            print("File",fileNumber,"passed")
        else:
            print("File",fileNumber,"FAIL")
            print(output)



