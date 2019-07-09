import os
import subprocess


SUBC = '../build/Debug/subc.exe'
NASM = '../tools/nasm/nasm.exe'
LINK = '../tools/golink/GoLink.exe'


def run(path):
    ret = subprocess.call([path])
    print '{0} -> {1}'.format(path, ret)


def link(base, path):
    out = base + '.exe'
    if 0 == subprocess.call([LINK, '/console', '/ni', path]):
        run(out)


def assemble(base, path):
    out = base + '.obj'
    if 0 == subprocess.call([NASM, '-o', out, '-f', 'win32', path]):
        link(base, out)


def compile(base, path):
    out = base + '.s'
    if 0 == subprocess.call([SUBC, '-S', path]) == 0:
        assemble(base, out)


def main():
    for f in os.listdir('.'):
        root, ext = os.path.splitext(f)
        if ext == '.c':
            compile(root, f)


if __name__ == '__main__':
    main()
