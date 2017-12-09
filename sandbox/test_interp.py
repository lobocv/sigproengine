from pylib import sensor_network_py as SN
from pylib import empath5_py as p
import numpy as np

interp_pts = 3
N = 5
a = np.arange(N, dtype=np.float64) - N
a = a.reshape((N, 1))

interp_size = N*(interp_pts+1)
b = np.zeros(interp_size, dtype=a.dtype)
b = b.reshape((interp_size, 1))

c = p.InterpolateTrace(a, b, interp_pts, "linear")

print b


