#coding:utf-8

import datetime
import time

default_date_format = '%Y-%m-%d'
default_time_format = '%H:%I:%S'
default_datetime_format = default_date_format + ' ' + default_time_format
durations = (
    (1,         '秒前', ),
    (60,        '分钟前', ),
    (3600,      '小时前', ),
    (86400,     '天前', ),
    # (7 * 86400, '周前', ),
    # (30 * 86400, '个月前', ),
    # (365 * 86400, '年前', ),
)
durations_max_seconds = 60 * durations[-1][0]


def cur_timestamp():
    """ get current Unix timestamp.
    @return: current Unix timestamp.
    @rtype: int
    """
    return int(time.time())


def strftime(timestamp, fmt=default_datetime_format):
    """ gen datetime string from `timestamp`, format in `fmt`.
    @param fmt:[="%Y-%m-%d %H:%I:%S"] format string.
    @rtype: str or unicode
    """
    return datetime.datetime.fromtimestamp(timestamp).strftime(format=fmt)


def now(fmt=default_datetime_format):
    """ get current datetime, default in "YYYY-MM-DD HH:MM:SS" format.
    @param fmt:[="%Y-%m-%d %H:%I:%S"] format string.
        @type fmt: str or unicode
    @return: now time, default in YYYY-MM-DD HH:MM:SS format.
    @rtype: str or unicode
    """
    return datetime.datetime.now().strftime(format=fmt)


def today(fmt=default_date_format):
    """ get current datetime, default in "YYYY-MM-DD HH:MM:SS" format.
    @param fmt:[="%Y-%m-%d"] format string.
        @type fmt: str or unicode
    @return: now time, default in YYYY-MM-DD HH:MM:SS format.
    @rtype: str or unicode
    """
    return datetime.date.today().strftime(format=fmt)


def ago(since, durations=durations, formats=False):
    """ gen human readable "how long ago since `since`".
    @param since: the timestamp since.
        @type since: int
    @rtype: str
    """
    past = cur_timestamp() - since
    if past > durations_max_seconds:
        return strftime(since, fmt=default_date_format)
    last = durations[0]
    for unit in durations:
        if past < unit[0]:
            break
        last = unit
    units = int(past // last[0])
    return last[1] % units \
        if formats else str(units) + last[1]
