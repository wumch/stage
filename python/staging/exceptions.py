#coding:utf-8

import sys


class PSException(Exception):
    """ Base class of exceptions provided by python-staging.
    """
    __err_code_generic = -1
    __err_code = __err_code_generic

    def __init__(self, code=None, message='', *args, **kwargs):
        super(PSException, self).__init__(self, *args, **kwargs)
        self.code = self.__class__.__err_code if code is None else code
        self.message = message


class NotImplementedException(PSException, NotImplementedError):
    """ Prety and self-inspected version of built-in `NotImplementedError`.
    """
    __err_code = 10086

    def __init__(self, this=None):
        caller_frame = sys._getframe(1)
        cls_name = 'global'
        if this is None:
            if 'self' in caller_frame.f_locals:
                cls_name = '<%s>.%s' % \
                    (caller_frame.f_locals['self'].__class__.__name__,
                     caller_frame.f_code.co_name)
            elif 'cls' in caller_frame.f_locals:
                cls_name = '%s::%s' % (caller_frame.f_locals['cls'].__name__,
                                       caller_frame.f_code.co_name)
        elif isinstance(this, type):
            cls_name = '%s::%s' % (this.__name__, caller_frame.f_code.co_name)
        elif isinstance(this, object):
            cls_name = '<%s>.%s' % (this.__class__.__name__,
                                    caller_frame.f_code.co_name)
        super(NotImplementedException, self).__init__(
            'method ' + cls_name + '.' + caller_frame.f_code.co_name
            + ' is not implemeted')
