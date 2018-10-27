import matplotlib.pyplot as pypl
import numpy as np

sizelist = []
seqs = [[]]
pars = [[]]
boost = [[]]

file1 = "sollin.txt"
with open(file1) as f1:
    #read sizes
    content = f1.readlines()
    lsizes = content[0]
    linesizes = lsizes.split()
    sizes = int(linesizes[0])
    # read trials
    ltrials = content[1]
    linetrials = ltrials.split()
    trials = int(linetrials[0])

    index = 2
    sizeindex = 0
    # read content
    for _ in range(sizes):

        line = content[index].split()
        sizelist.append(int(line[0]))
        seqs.append([]);
        pars.append([]);
        boost.append([]);
        index += 1

        for _ in range(trials):
            line = content[index].split()

            seqs[sizeindex].append(float(line[0]));
            pars[sizeindex].append(float(line[1]));
            boost[sizeindex].append(float(line[2]));

            index += 1

        sizeindex += 1

seqMean = []
seqStd = []
parMean = []
parStd = []
boostMean = []
boostStd = []

for i in range(sizes):
       seqMean.append(np.mean(seqs[i])) 
       parMean.append(np.mean(pars[i])) 
       boostMean.append(np.mean(boost[i])) 
       seqStd.append(np.std(seqs[i])) 
       parStd.append(np.std(pars[i])) 
       boostStd.append(np.std(boost[i])) 


pypl.errorbar(sizelist,seqMean,seqStd,label="Sequential")
pypl.errorbar(sizelist,parMean,parStd,label="Parallel")
pypl.errorbar(sizelist,boostMean,boostStd,label="BGL")
pypl.legend()
pypl.xlabel("Vertices")
pypl.ylabel("Seconds")
pypl.title("Wall-Clock Time over Vertices, with 4 threads, m = 100 n edges")
pypl.xticks(sizelist)
pypl.xscale('log')
#pypl.ylim(ymin=0)
pypl.yscale('log')
#pypl.show()
pypl.savefig("Sollin")
