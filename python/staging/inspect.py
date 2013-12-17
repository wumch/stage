#coding:utf-8

from pyramid.settings import asbool, aslist


def asint(val):
    """parse/convert anything to int. """
    if isinstance(val, (str, unicode)):
        v = val.strip()
        return int(v) if v.isdigit() else 0
    elif isinstance(val, (int, float)):
        return int(val)
    elif isinstance(val, complex):
        return int(val.real)
    elif hasattr(val, '__int__'):
        return int(val)
    else:
        return 0


__coef = (7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2, )
__iter = tuple(range(len(__coef)))
__value = ('1', '0', 'X', '9', '8', '7', '6', '5', '4', '3', '2', )


def isIDCard2G(cardno):
    """
    @param cardno: the number of ID-Card, an string contains 18 characters.
        @type cardno str or unicode
    @return: bool
    """
    global __coef, __value, __iter
    return len(cardno) == 18 and cardno[17] == \
        __value[sum((ord(cardno[i]) & 207) * __coef[i] for i in __iter) % 11]
