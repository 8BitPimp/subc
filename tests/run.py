import os
import subprocess


SUBC = '../build/Debug/subc.exe'
NASM = '../tools/nasm/nasm.exe'
LINK = '../tools/golink/GoLink.exe'


expected = {
    'test001' : 0,
    'test002' : 3,
    'test003' : 9,
}



tried = set()
passed = set()


def do_run(base, path):
    ret = subprocess.call([path])
    print '{0} -> {1}'.format(path, ret)

    if base in expected:
        if expected[base] == ret:
            passed.add(base)
        else:
            print 'does not match expected value'


def do_link(base, path):
    out = base + '.exe'
    if 0 == subprocess.call([LINK, '/console', '/ni', path]):
        do_run(base, out)


def do_assemble(base, path):
    out = base + '.obj'
    if 0 == subprocess.call([NASM, '-o', out, '-f', 'win32', path]):
        do_link(base, out)


def do_compile(base, path):
    tried.add(base)
    out = base + '.s'
    if 0 == subprocess.call([SUBC, '-S', path]) == 0:
        do_assemble(base, out)


def main():
    # compile all files in here
    for f in os.listdir('.'):
        root, ext = os.path.splitext(f)
        if ext == '.c':
            do_compile(root, f)

    print '{0} of {1} passed'.format(len(passed), len(tried))

    if len(passed) != len(tried):
        print 'failed:'
        for x in tried:
            if x not in passed:
                print '   {0}'.format(x)
        exit(1)
    else:
        exit(0)

if __name__ == '__main__':
    main()
