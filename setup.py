from distutils.core import setup, Extension
setup(name='non_lock', ext_modules=[Extension('non_lock', sources=['main.c'])])
