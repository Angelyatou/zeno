import runpy
import tempfile
import threading
import atexit
import shutil
import os
import zen
from multiprocessing import Process

from .descriptor import parse_descriptor_line


zen.loadAutoloads()


g_proc = None
g_iopath = None
g_lock = threading.Lock()


def killProcess():
    global g_proc
    if g_proc is None:
        print('worker process is not running')
        return
    g_proc.terminate()
    g_proc = None
    print('worker process killed')


def _launch_mproc(func, *args):
    global g_proc
    if g_proc is not None:
        killProcess()
    if os.environ.get('ZEN_SPROC'):
        func(*args)
    else:
        g_proc = Process(target=func, args=tuple(args), daemon=True)
        g_proc.start()
        g_proc.join()
        if g_proc is not None:
            print('worker processed exited with', g_proc.exitcode)
        g_proc = None


@atexit.register
def cleanIOPath():
    global g_iopath
    if g_iopath is not None:
        shutil.rmtree(g_iopath, ignore_errors=True)
    g_iopath = None


def launchGraph(graph, nframes):
    global g_iopath
    cleanIOPath()
    g_iopath = tempfile.mkdtemp(prefix='zenvis-')
    print('iopath:', g_iopath)
    _launch_mproc(zen.runGraph, graph, nframes, g_iopath)


def getDescriptors():
    descs = zen.dumpDescriptors()
    descs = descs.splitlines()
    descs = [parse_descriptor_line(line) for line in descs if ':' in line]
    descs = {name: desc for name, desc in descs}
    print('loaded', len(descs), 'descriptors')
    return descs


__all__ = [
    'getDescriptors',
    'launchGraph',
    'killProcess',
]
