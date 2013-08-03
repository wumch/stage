#coding:utf-8

import os
import sys
import traceback


class Tracer(object):
    """ Util for runtime back-trace.
    usage:
    >>> class Tester(object):
    >>>     def caller(self):
    >>>         self.callee()
    >>>     def callee(self):
    >>>         raise NotImplementedError()
    >>>
    >>> try:
    >>>     Tester().caller()
    >>> except NotImplementedError, e:
    >>>     print Tracer(max_depth=3).prety()
    /staging/tracer.py:182:callee(){raise NotImplementedError()} <= /staging/tra
    cer.py:178:caller(){self.callee()} <= /staging/tracer.py:186:<module>{Test()
    .caller()}
    >>>     sys.exc_clear()
    >>>     print Tracer(max_depth=10).trace()
    [['/staging/tracer.py', 94, 'trace'], ['/staging/tracer.py', 191, '<module>'
    ]]
    """

    class FrameInfo(object):

        def __init__(self, filepath=None, lineno=None,
                     clsname=None, funcname=None, code=None):
            self.filepath, self.lineno, self.clsname, self.funcname, \
                self.code = filepath, lineno, clsname, funcname, code

        def __repr__(self):
            return str(
                [self.filepath, self.lineno, self.clsname, self.funcname])

    def __init__(self, max_depth=1000, funcname=True, filepath=True,
                 lineno=True, abspath=False, singleline=True,
                 code=True, skip=1, extra_skip=0,
                 root_path='', omit_root=False):
        """
        @param max_depth:[=1000] max trace depth.
            @type max_depth int
        @param funcname:[=True] whether need function name or not.
            default True.
            @type funcname bool
        @param filepath:[=True] whether need code-file-path name or not.
            @type filepath bool
        @param lineno:[=True] whether need line-number or not.
            @type lineno bool
        @param abspath:[=False] whether give absolute-file-path
        or path related to package.
            @type abspath bool
        @param singleline:[=True] whether format into single line or multi.
            @type singleline bool
        @param code:[=True] whether need running code or not.
            @type code bool
        @param skip:[=1] how many frames to skip since `trace` called.
            @type skip int
        @param extra_skip:[=0] how many frames to skip since tracing point.
            @type extra_skip int
        @param root_path:[=''] the root path (to omit).
            @type root_path str or unicode
        @param omit_root:[=False] whether omit the `root_path` or not.
            @type omit_root bool
        """
        self.max_depth = max_depth
        self.funcname = funcname
        self.filepath = filepath
        self.lineno = lineno
        self.abspath = abspath
        self.singleline = singleline
        self.code = code
        self.skip = skip
        self.extra_skip = extra_skip
        self.root_path = root_path.encode('utf-8') \
            if isinstance(root_path, unicode) else root_path
        self.omit_root_path = omit_root
        self.root_path_len = len(self.root_path)

    def trace(self, depth=None):
        """ get the runtime tracing information as a list.
        @param depth:[=None] depth to trace. defaults according to `max_depth`.
        @rtype: list
        """
        return self._trace(depth=depth, sp=self.skip+self.extra_skip+1)

    def prety(self, depth=None):
        """ get the runtime tracing information as a prety formated string.
        @param depth:[=None] depth to trace. defaults according to `max_depth`.
        @rtype: str
        """
        return (' <= ' if self.singleline else os.linesep).join(
            map(self._pretys, self._trace(
                depth=depth, sp=self.skip+self.extra_skip+1)))

    def _pretys(self, frame_info):
        return (frame_info.filepath[self.root_path_len:]
                if self.omit_root_path and
                frame_info.filepath.startswith(self.root_path)
                else frame_info.filepath) \
            + ':' + str(frame_info.lineno) + ':' \
            + frame_info.funcname \
            + ("()" if frame_info.funcname != "<module>" else '') \
            + ('' if frame_info.code is None
                else ('{' + frame_info.code + '}'))

    def _trace(self, depth=None, sp=None):
        info = sys.exc_info()
        return self._trace_normal(depth=depth, sp=sp) if info[2] is None \
            else self._trace_exception(depth=depth, tb=info[2])

    def _trace_exception(self, depth=None, tb=None):
        depth = self._get_depth(depth=depth)
        cur = 0
        chain = []
        frame = tb.tb_frame.f_back
        while frame and cur < depth:
            chain.append(self._gen_info_from_frame(frame))
            frame = frame.f_back
            cur += 1
        return chain

    def _trace_normal(self, depth=None, sp=None):
        sp = sp or self.skip + self.extra_skip
        depth = self._get_depth(depth=depth)
        frame = sys._getframe(sp)
        cur = 0
        chain = []
        while frame is not None and cur < depth:
            chain.append(self._gen_info_from_frame(frame))
            frame = frame.f_back
            cur += 1
        return chain

    def _gen_info_from_frame(self, frame):
        fcode = frame.f_code
        if 'self' in frame.f_locals:
            funcname = frame.f_locals['self'].__class__.__name__ \
                + '.' + fcode.co_name
        elif 'cls' in frame.f_locals:
            funcname = frame.f_locals['cls'].__name__ + '::' + fcode.co_name
        else:
            funcname = fcode.co_name
        return self._gen_info(
            path=fcode.co_filename, lineno=frame.f_lineno,
            funcname=funcname)

    def _gen_info(self, path, lineno, funcname, code=None):
        finfo = self.FrameInfo()
        if self.filepath:
            finfo.filepath = os.path.abspath(path) if self.abspath else path
        if self.lineno:
            finfo.lineno = lineno
        if self.funcname:
            finfo.funcname = funcname
        if self.code:
            finfo.code = code
        return finfo

    def _get_depth(self, depth=None):
        if depth is None:
            return self.max_depth
        elif self.max_depth is None:
            return 10
        else:
            return min(self.max_depth, depth)


if __name__ == '__main__':

    class Test(object):

        def caller(self):
            self.callee()

        @classmethod
        def callee(cls):
            raise NotImplementedError()

    root_path = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    try:
        Test().caller()
    except NotImplementedError, e:
        settings = dict(root_path=root_path, abspath=True, file_path='', omit_root=True)
        print Tracer(max_depth=3, **settings).prety()
        sys.exc_clear()
        print Tracer(max_depth=10, **settings).trace()
