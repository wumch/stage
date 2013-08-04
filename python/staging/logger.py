#coding:utf-8

import sys
import logging
from datetime import datetime
from .tracer import Tracer
from .exceptions import PSException

_default_date_format = '%Y-%m-%d %H:%I:%S.%s'
_default_logger_name = 'staging'


# currently not used.
class LogHandler(logging.FileHandler):

    def __init__(self, filename, mode='a', encoding=None, delay=0,
                 date_format=_default_date_format):
        super(LogHandler, self).__init__(
            filename=filename, mode=mode, encoding=encoding, delay=delay)
        self.date_format = date_format

    def format(self, record):
        return datetime.now().strftime(self.date_format) + \
            (' %(levelname)s[%(process)d] %(filename)s:%(lineno)d \
%(funcName)s%(args)s: %(msg)s' % record.__dict__)


class Logger(logging.Logger):

    def __init__(self, name=_default_logger_name, level=logging.NOTSET,
                 tracer=None):
        super(Logger, self).__init__(name, level=level)
        self.tracer = tracer or Tracer(max_depth=10, extra_skip=3)

    def exception(self, excp, *args):
        """ log an exception with prety tracing information.
        @type excp: Exception or basestring
        """
        return self.trace_exception(excp) \
            if isinstance(excp, Exception) else self.error(excp, *args)

    def trace_exception(self, e):
        """ log an exception with prety tracing information.
        """
        if isinstance(e, PSException):
            self.trace_error(
                'caught %s(code=%d, message="%s")' %
                (e.__class__.__name__, e.code, e.message)
            )
        else:
            self.trace_error(
                'caught ' + e.__class__.__name__ +
                (str(e.args)[:-2] + ')' if e.args else '()'))

    def trace_error(self, msg, *args, **kwargs):
        """ log an error with prety tracing information.
        """
        super(Logger, self).error(
            msg + ". <= " + self.tracer.prety(), *args, **kwargs)

    def findCaller(self):
        f = sys._getframe(5 if any(sys.exc_info()) else 4)
        return (f.f_code.co_filename, f.f_lineno, f.f_code.co_name) \
            if f and f.f_code \
            else ("(unknown file)", 0, "(unknown function)")


def make_logger(logfile, name=_default_logger_name, level=logging.NOTSET,
                echo=False, dateformat=_default_date_format):
    """ make <Logger>.
    @rtype: Logger
    @attention: if `name` is specified as non-string, then will return a
    <logging.RootLogger> instead of <Logger>.
    """
    logging.setLoggerClass(Logger)
    logger = logging.getLogger(name=name) \
        if isinstance(name, basestring) else logging.getLogger()
    if echo:
        handler = logging.StreamHandler(stream=sys.stderr)
        fmt = '%(levelname)s[%(process)d] %(filename)s:%(lineno)d \
%(funcName)s%(args)s: %(msg)s'
    else:
        handler = logging.FileHandler(filename=logfile)
        fmt = '%(asctime)s %(levelname)s[%(process)d] %(filename)s:%(lineno)d \
%(funcName)s%(args)s: %(msg)s'
    handler.setFormatter(logging.Formatter(fmt=fmt, datefmt=dateformat))
    logger.addHandler(handler)
    logger.setLevel(level=level)
    return logger


def test_logger():

    import os
    import sys
    from .exceptions import NotImplementedException

    source_root = os.path.join(
        os.path.dirname(
            os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
    if source_root not in sys.path:
        sys.path.append(source_root)

    logfile = '/tmp/test_logger.log'
    logger = make_logger(logfile=logfile, echo=True)

    class Test(object):

        @classmethod
        def test(cls):
            try:
                raise NotImplementedException()
            except Exception, e:
                logger.trace_exception(e)

    def caller():
        Test().test()

    print '-' * 35, 'stdout/stderr:', '-' * 36

    caller()
    logger.error("just log an error")
    logger.trace_error("trace this as an error")

    print
    print '-' * 20, 'appended to "%s" right now:' % logfile, '-' * 20
    os.system("tail -n 3 %s" % logfile)
