libs = []

try:
	from pylib import empath5_py as empath5
	libs.append(empath5)
except ImportError as e:
	print e

try:
	from pylib import radarview_py as radarview
	libs.append(radarview)
except ImportError as e:
	print e

import inspect

for lib in libs:
	print "Modules detected in %s:" % lib.__name__
	modcnt = 1
	for  attr in dir(lib):
		if not attr.startswith('__'):

			attrval = getattr(lib, attr)
			if inspect.isclass(attrval):
				vartype = "Class"
			elif inspect.ismethoddescriptor(attrval):
				vartype = "Function"
			else:
				vartype = type(attrval)

			print '\t %d. %s (%s)' % (modcnt, attr, vartype)
			modcnt+=1
