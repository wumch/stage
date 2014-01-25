#coding:utf-8

__mm_registry = {}


class __MultiMethod(object):

    def __init__(self, name):
        self.name = name
        self.typemap = {}
        self.mm_last_reg = None

    def __call__(self, *args):
        types = tuple(arg.__class__ for arg in args)
        function = self.typemap.get(types)
        if function is None:
            raise TypeError(
                '{%s(%s)} no match' %
                (self.name, ','.join(map(lambda t: t.__name__, types))))
        return function(*args)

    def register(self, types, function):
        if types in self.typemap:
            raise RuntimeError('{%s} duplicate registration' % self.name)
        self.typemap[types] = function
        self.mm_last_reg = function


def multimethod(*types):

    def register(function):
        global __mm_registry
        function = getattr(function, 'mm_last_reg', function)
        name = function.__name__
        mm = __mm_registry.get(name)
        if mm is None:
            mm = __mm_registry[name] = __MultiMethod(name)
        mm.register(types, function)
        return mm

    return register


if __name__ == '__main__':

    @multimethod(int, int)
    def test(a, b):
        print 'test(int,int): ', a + b

    @multimethod(float, float)
    def test(a, b):
        print 'test(float,float): ', a + b

    test(8, 9)
    test(8.0, 9.0)
    test(8.0, 9)
