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

        chains[0].apply(self.inData, self.outData)
        self.assertEqual(self.outData[0], GAIN ** N_CHAINS * self.inData[0])

        self.tearDown()
        chains[0].apply(self.inData)    # Apply in-place
        self.assertEqual(self.inData[0], GAIN ** N_CHAINS)

    def test_apply_process_in_place(self):
        pc = spe.ProcessingChain()
        g = spe.Gain()
        g.setup(N, GAIN, True)
        pc.add_process(g)
        pc.add_process(g)
        pc.apply(self.inData)
        self.assertEqual(self.inData.sum(), GAIN**2 * len(self.inData))

    def test_apply_process(self):
        g = spe.Gain()
        g.setup(N, GAIN, True)
        g.apply(self.inData, self.outData)
        self.assertEqual(self.inData.sum(), len(self.inData))
        self.assertEqual(self.outData.sum(), GAIN * len(self.outData))

    def test_json(self):
        conf = {"processes": [{"Gain": {"points_per_trace": 10, "enabled": True, "gain": 5}}]}
        pc = spe.ProcessingChain()
        pc.json_load(conf)
        pc.apply(self.inData, self.outData)
        self.assertEqual(self.outData.sum(), GAIN * len(self.outData))
        conf2 = pc.json_save()
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
