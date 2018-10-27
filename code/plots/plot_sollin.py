import matplotlib.pyplot as pypl

seqs = []
pars = []
boost = []

file1 = "sollin.txt"
with open(file1) as f1:
    content = f1.readlines()
    for l in content:
        line = l.split()
        seqs.append(float(line[0]));
        pars.append(float(line[1]));
        boost.append(float(line[2]));



pypl.plot(throughputSet,latencySet,label="Set-only")
pypl.plot(throughputGet,latencyGet,label="Get-only")
pypl.legend()
pypl.xlabel("msec")
pypl.ylabel("Ops/sec")
pypl.title("Interactive Law, Latency Over Throughput")
pypl.ylim(ymin=0)
pypl.xlim(xmin=0)
#pypl.show()
pypl.savefig("Interactive_1m_2s")
