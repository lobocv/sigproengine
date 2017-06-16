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
        self.outData = np.zeros_like(self.inData)

    def tearDown(self):
        self.inData[:] = 1
        self.outData[:] = 1

    def test_recursive_processing_chain(self):
        print( "STARTING TEST %s" % "test_recursive_processing_chain")


        pc_100 = spe.ProcessingChain()
        pc_110 = spe.ProcessingChain()
        pc_120 = spe.ProcessingChain()
        pc_121 = spe.ProcessingChain()

        g_1 = spe.Gain()
        g_1.setup( 2.0, True)
        g_2 = spe.Gain()
        g_2.setup( 2.0, True)

        pc_100.add_process(g_1)         # Gain by x2
        pc_100.add_process(pc_110)      # Fork output of step 1 into new PC (Gain x3) --> : 2 x 3     = 6
        pc_100.add_process(pc_120)      # Fork output of step 1 into new PC (Gain x4) --> : 2 x 4     = 8
                                        #    |
                                        #    L--> This chain has another PC (Gain x 5)--> : 2 x 4 x 5 = 40
        pc_100.add_process(g_2)         # Gain by x2 to output of step 1              --> : 2 x 2     = 4


        g_3 = spe.Gain()
        g_3.setup( 3.0, True)
        pc_110.add_process(g_3)

        g_5 = spe.Gain()
        g_5.setup( 5.0, True)
        pc_121.add_process(g_5)

        g_4 = spe.Gain()
        g_4.setup( 4.0, True)
        pc_120.add_process(g_4)
        pc_120.add_process(pc_121)

        outData = self.outData
        outData2 = self.outData.copy()
        outData3 = self.outData.copy()
        outData4 = self.outData.copy()

        # Define output buffers for each processing chain
        output_structure = [outData,                # First chain
                                [outData2],         # Second level Chain
                                [outData3,          # Second level Chain
                                        [outData4]  # Third level Chain
                                 ]

                            ]

        pc_100.apply(self.inData, output_structure)

        first_chain_output = outData[0]
        second_chain_output = outData2[0]
        third_chain_output = outData3[0]
        fourth_chain_output = outData4[0]

        print (self.inData[0], first_chain_output, second_chain_output, third_chain_output, fourth_chain_output)
        self.assertEqual(first_chain_output,  2 * 2)
        self.assertEqual(second_chain_output, 2 * 3)
        self.assertEqual(third_chain_output,  2 * 4)
        self.assertEqual(fourth_chain_output,  2 * 4 * 5)

    def test_apply_process_in_place(self):
        pc = spe.ProcessingChain()
        g = spe.Gain()
        g.setup(GAIN, True)
        pc.add_process(g)
        pc.add_process(g)
        pc.apply(self.inData)
        self.assertEqual(self.inData.sum(), GAIN**2 * len(self.inData))

    def test_apply_process(self):
        g = spe.Gain()
        g.setup( GAIN, True)
        g.apply(self.inData, self.outData)
        self.assertEqual(self.inData.sum(), len(self.inData))
        self.assertEqual(self.outData.sum(), GAIN * len(self.outData))

    def test_disable_process(self):
        pc = spe.ProcessingChain()
        g = spe.Gain()
        g.setup(GAIN, True)
        g2 = spe.Gain()
        g2.setup(GAIN, False)
        pc.add_process(g)
        pc.add_process(g2)
        pc.apply(self.inData)
        self.assertEqual(self.inData.sum(), GAIN**1 * len(self.inData))

    def test_json(self):
        conf = {"processes": [{"name": "Gain", "enabled": True, "gain": 5}
                              ]
                }
        pc = spe.ProcessingChain()
        pc.json_load(conf)
        pc.apply(self.inData, [self.outData])
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
# # gain.setup( 1)
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
