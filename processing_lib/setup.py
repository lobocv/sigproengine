__author__ = 'calvin'
from setuptools import setup
import os



PACKAGE_VERSION = '0.1.0'

PACKAGE_NAME = 'sigproengine'


# Create the relative module names
modules = ['procedures']


setup(name=PACKAGE_NAME,
      version=PACKAGE_VERSION,
      author='Calvin Lobo',
      description='signal processing procedures library.',
      platforms=['linux', 'windows'],
      url='www.sensoft.ca',
      author_email=['clobo@sensoft.ca'],
      package_dir={'': 'pylib'},  # Map the root directory to pylib
      packages=modules,  # List of modules to put in the root directory
      requires=[]
      )