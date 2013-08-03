#coding:utf-8

import sys
import logging
from datetime import datetime
from .tracer import Tracer
from .exceptions import PSException

_default_date_format = '%Y-%m-%d %H:%I:%S.%s'


class LogHandler(logging.FileHandler):

    def __init__(self, filename, mode='a', encoding=None, delay=0,
                 date_format=_default_date_format):
        super(LogHandler, self).__init__(
            filename=filename, mode=mode, encoding=encoding, delay=delay)
        self.date_format = date_format

    def format(self, record):
        return datetime.now().strftime(self.date_format) + \
            (' %(levelname)s %(filename)s:%(lineno)d \
%(className)s%(funcName)s%(args)s %(msg)s' % record.__dict__)


class Logger(logging.RootLogger):

    _instance = None

    @classmethod
    def instance(cls, logfile=None, also_print=False):
        """
        @param logfile:[=None] path of log file.
            @attention: it's required when first time instantiate.
        @param also_print:[=False] whether also print to stdout/stderr or not.
        @rtype: Logger
        """
        if cls._instance is None and logfile is not None:
            cls._instance = cls(logfile=logfile, also_print=also_print)
        return cls._instance

    def __init__(self, logfile, tracer=None, also_print=False):
        logging.RootLogger.__init__(self, logging.NOTSET)
        self.setLevel(logging.NOTSET)
        self.removeHandler(self.handlers)
        self.addHandler(LogHandler(filename=logfile))
        self.tracer = tracer or Tracer(max_depth=10, extra_skip=2)
        self.also_print = also_print

    def trace_exception(self, exception):
        """ log an exception with prety tracing information.
        """
        if isinstance(exception, PSException):
            self.error(
                str(exception.code) + exception.message + str(exception.args))
        else:
            self.error(str(exception.args))

    def error(self, msg, *args, **kwargs):
        if self.also_print:
            print >>sys.stderr, self, msg + ";trace: " + self.tracer.prety()
        logging.RootLogger.error(self, msg + ";trace: " + self.tracer.prety())

    @classmethod
    def set_instance(cls, logger_instance):
        """ set a Logger instance,
        so that you can Logger.instance without arguments.
        @type logger_instance Logger
        """
        cls._instance = logger_instance


def make_logger(logfile, dateformat=_default_date_format, **log_handler_kwargs):
    """
    @rtype: logging.RootLogger
    """
    logger = logging.getLogger()
    logger.setLevel(logging.NOTSET)
    logger.removeHandler(logger.handlers)
    logger.addHandler(LogHandler(
        filename=logfile, date_format=dateformat, **log_handler_kwargs))
    return logger


def test_logger():

    import os
    import sys

    source_root = os.path.join(
        os.path.dirname(
            os.path.dirname(os.path.dirname(os.path.realpath(__file__)))))
    if source_root not in sys.path:
        sys.path.append(source_root)

    logfile = '/tmp/test_logger.log'

    Logger.set_instance(Logger(logfile=logfile, also_print=True))

    def test():
        try:
            raise Exception("exception raised for test logger")
        except Exception, e:
            Logger.instance().trace_exception(e)

    test()
    print '-' * 20, 'content of %s' % logfile, '-' * 20
    os.system("cat %s" % logfile)
