import matplotlib.pyplot as pypl
import numpy as np

sizelist = []
seqs = [[]]
pars = [[]]
boostKruskal = [[]]
boostPrim = [[]]

file1 = "log_varSize_edgePerVertex=20.txt"
with open(file1) as f1:
    #read sizes
    content = f1.readlines()
    lsizes = content[0]
    linesizes = lsizes.split()
    sizes = int(linesizes[-1])
    # read trials
    ltrials = content[1]
    linetrials = ltrials.split()
    trials = int(linetrials[-1])

    index = 2
    sizeindex = 0
    # read content
    for _ in range(sizes):

        line = content[index].split()
        sizelist.append(int(line[-1]))
        seqs.append([]);
        pars.append([]);
        boostKruskal.append([]);
        boostPrim.append([]);
        index += 1

        for _ in range(trials):
            line = content[index].split()

            seqs[sizeindex].append(float(line[-1]));
            pars[sizeindex].append(float(line[1]));
            boostKruskal[sizeindex].append(float(line[2]));
            boostPrim[sizeindex].append(float(line[3]));

            index += 1

        sizeindex += 1

seqMean = []
seqStd = []
parMean = []
parStd = []
boostKruskalMean = []
boostKruskalStd = []
boostPrimMean = []
boostPrimStd = []

for i in range(sizes):
       seqMean.append(np.mean(seqs[i])) 
       parMean.append(np.mean(pars[i])) 
       boostKruskalMean.append(np.mean(boostKruskal[i])) 
       boostPrimMean.append(np.mean(boostPrim[i])) 
       seqStd.append(np.std(seqs[i])) 
       parStd.append(np.std(pars[i])) 
       boostKruskalStd.append(np.std(boostKruskal[i])) 
       boostPrimStd.append(np.std(boostPrim[i])) 


pypl.errorbar(sizelist,seqMean,seqStd,label="Sequential")
pypl.errorbar(sizelist,parMean,parStd,label="Parallel")
pypl.errorbar(sizelist,boostKruskalMean,boostKruskalStd,label="BGL - Kruskal")
pypl.errorbar(sizelist,boostPrimMean,boostPrimStd,label="BGL - Prim")
pypl.legend()
pypl.xlabel("Vertices")
pypl.ylabel("Seconds")
pypl.title("Wall-Clock Time over Vertices, with 4 threads, m = 100 n edges")
pypl.xticks(sizelist)
pypl.xscale('log')
pypl.ylim(ymin=0)
#pypl.yscale('log')
#pypl.show()
pypl.savefig("Plots_varSize_edgePerVertex=20")
