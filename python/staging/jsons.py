#coding:utf-8

import json


class JSONs(object):

    def __init__(self, encoding='utf-8', **kwargs):
        self.decoder = json.JSONDecoder(encoding=encoding, **kwargs)

    __instance = None
    @classmethod
    def instance(cls):
        """
        @rtype: JSONs
        """
        if cls.__instance is None:
            cls.__instance = cls()
        return cls.__instance

    def decode(self, jstr, idx=0, gap=0):
        if isinstance(gap, basestring):
            if isinstance(jstr, unicode) and not isinstance(gap, unicode):
                gap = unicode(gap, encoding=self.decoder.encoding)
            elif not isinstance(jstr, unicode) and isinstance(gap, unicode):
                gap = gap.encode(encoding=self.decoder.encoding)
            gap = len(gap)
        res = []
        end = len(jstr) - 1
        while idx < end:
            obj, idx = self.decoder.raw_decode(jstr, idx)
            idx += gap
            res.append(obj)
        return res
