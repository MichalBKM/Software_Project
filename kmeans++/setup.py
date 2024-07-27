from setuptools import Extension, setup

module = Extension("mykmeanssp", sources=['kmeansmodule.c'])
setup(name='mykmeanssp',
     version='1.0',
     description='Python wrapper for kmeans_pp.c extension',
     ext_modules=[module])