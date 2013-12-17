#coding:utf-8

import os
from hashlib import md5 as _md5


def mkdir(path):
    """ recursive mkdir. """
    parent = os.path.dirname(path)
    if parent and not os.path.exists(parent):
        mkdir(parent)
    if path and not os.path.exists(path):
        os.mkdir(path)


def md5(data, encoding='utf-8'):
    """
    @type data: str or unicode
    @rtype: str
    """
    return _md5(
        data.encode(encoding)
        if isinstance(data, unicode) else data).hexdigest()
