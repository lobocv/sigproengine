import unittest

from bin import sigproengine as spe
import numpy as np
import json as JSON

N=10
GAIN = 5

class SigProEngineTest(unittest.TestCase):

    def __init__(self, *args):
        super(SigProEngineTest, self).__init__(*args)
        self.inData = np.ones(N, dtype=np.float)
        self.outData = self.inData.copy()
        self.pc = spe.ProcessingChain()

    def tearDown(self):
        self.inData[:] = 1
        self.outData[:] = 1

    def test_recursive_processing_chain(self):
        g = spe.Gain()
        g.setup(N, GAIN, True)

        chains = []
        N_CHAINS = 3
        for i in range(N_CHAINS):
            pc = spe.ProcessingChain()
            pc.add_process(g)
            chains.append(pc)
            if i > 0:
                chains[i-1].add_process(pc)

        # print(chains[0].json_save())
        # chains[0].apply(self.inData)
        chains[0].apply(self.inData, self.outData)
        print (self.inData)
        print (self.outData)
        # self.assertEqual(self.outData[0], GAIN ** N_CHAINS * self.outData[0])

    def test_apply_process_in_place(self):
        g = spe.Gain()
        g.setup(N, GAIN, True)
        g.apply(self.inData)
        self.assertEqual(self.inData.sum(), GAIN * len(self.inData))

    def test_apply_process(self):
        g = spe.Gain()
        g.setup(N, GAIN, True)
        g.apply(self.inData, self.outData)
        self.assertEqual(self.inData.sum(), len(self.inData))
        self.assertEqual(self.outData.sum(), GAIN * len(self.outData))

    def test_json(self):
        conf = {"processes": [{"Gain": {"points_per_trace": 10, "enabled": True, "gain": 5}}]}
        self.pc.json_load(conf)
        self.pc.apply(self.inData, self.outData)
        self.assertEqual(self.outData.sum(), GAIN * len(self.outData))
        conf2 = self.pc.json_save()
        self.assertEqual(conf, conf2)

if __name__ == '__main__':

    unittest.main()





# print(inData)
# print(outData)



# #
# chain = spe.ProcessingChain()
# # gain = spe.Gain()
# # gain.setup(N, 1)
# # chain.add_process(gain)
#
# # print('SAVING JSON\n\n')
# # js = chain.json_save()
# # print("Saved JSON:\n %s" % js)
#
# # print (JSON.loads(js))
#
# print('LOADING JSON\n\n')
# # chain.json_load(js)
# chain.json_load(JSON.dumps(
#             {
#              'processes': [{'Gain': {'points_per_trace': N, 'gain': GAIN,
#                                      'enabled': 1
#                                      }},
#                            # {'Dewow': {'ppt': 5, 'freq': 100}},
#                           ],
#
#              }
# ))
#
# # chain.apply(inData, outData)
# chain.apply(inData)
# print(inData)
# print(outData)
# #
