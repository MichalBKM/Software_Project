from setuptools import Extension, setup

module = Extension("symnmf", sources=["symnmfmodule.c", "symnmf.c"])

setup(name='symnmf',
     version='1.0',
     description='Python wrapper for symNMF extension',
     ext_modules=[module])