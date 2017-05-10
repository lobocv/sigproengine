from bin import sigproengine as spe
import numpy as np

# spe.func()

N=10
t = np.ones(N, dtype=np.float)

chain = spe.ProcessingChain()
gain = spe.Gain()
gain.setup(N, 5)
chain.add_process(gain)

js = chain.json_serialize()
print ("JSON: %s" % js)



print(t)
chain.apply(t)
print(t)

