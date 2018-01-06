#pythonCWrapTest
This is an example how to wrap C for Python3.x.
I did not confirm the performance with Python2.x

#Install
```bash
$ git clone https://github.com/montblanc18/pythonCWrapTest.git .
$ python setup.py build
$ python setup.py install
$ pip list -l | grep SpamMethod
SpamMethod (1.0)
```

#Usage
```bash
$ python
Python 3.6.1 (default, Aug 27 2017, 16:38:38) 
[GCC 4.2.1 Compatible Clang 3.9.1 (tags/RELEASE_391/final)] on darwin
Type "help", "copyright", "credits" or "license" for more information.
```
```python
>>> import spam
>>> spam.system("ls")
README.md	build		setup.py	spammodule.c
```