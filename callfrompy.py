from bin import sigproengine as spe
import numpy as np
import json as JSON

# spe.func()

N=10
GAIN = 5

t = np.ones(N, dtype=np.float)
print(t)

chain = spe.ProcessingChain()
# gain = spe.Gain()
# gain.setup(N, 1)
# chain.add_process(gain)

# print('SAVING JSON\n\n')
# js = chain.json_save()
# print("Saved JSON:\n %s" % js)

# print (JSON.loads(js))

print('LOADING JSON\n\n')
# chain.json_load(js)
chain.json_load(JSON.dumps(
            {
             'processes': [{'Gain': {'points_per_trace': N, 'gain': GAIN,
                                     'enabled': 0
                                     }},
                           # {'Dewow': {'ppt': 5, 'freq': 100}},
                          ],

             }
))

chain.apply(t)
print(t)
#
