#coding:utf-8

import sys


class PSException(Exception):
    """ Base class of exceptions provided by python-staging.
    """
    __err_code_generic = -1
    _err_code = __err_code_generic

    def __init__(self, message='', code=None, *args, **kwargs):
        """
        @param message: error message
            @type message str or unicode
        @param code: error code.
            @type code int
        """
        super(PSException, self).__init__(self, *args, **kwargs)
        self.message = message
        self.code = self._err_code if code is None else code


class NotImplementedException(PSException, NotImplementedError):
    """ Prety and self-inspected version of built-in `NotImplementedError`.
    """
    _err_code = 10086

    def __init__(self, this=None):
        caller_frame = sys._getframe(1)
        func = caller_frame.f_code.co_name
        if this is None:
            if 'self' in caller_frame.f_locals:
                func = '<%s>.%s' % \
                    (caller_frame.f_locals['self'].__class__.__name__,
                     caller_frame.f_code.co_name)
            elif 'cls' in caller_frame.f_locals:
                func = '%s::%s' % \
                       (caller_frame.f_locals['cls'].__name__,
                        caller_frame.f_code.co_name)
        elif isinstance(this, type):
            func = '%s::%s' % \
                   (this.__name__, caller_frame.f_code.co_name)
        elif isinstance(this, object):
            func = '<%s>.%s' % \
                   (this.__class__.__name__, caller_frame.f_code.co_name)
        super(NotImplementedException, self).__init__(
            func + ' is not implemeted')
