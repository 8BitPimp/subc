import os
import subprocess
import sys


is_linux = (sys.platform == "linux" or sys.platform == "linux2")


if is_linux:
  SUBC = '../build/subc'
  NASM = 'nasm'
  LINK = 'ld'
else:
  SUBC = '../build/Debug/subc.exe'
  NASM = '../tools/nasm/nasm.exe'
  LINK = '../tools/golink/GoLink.exe'


expected = {
    'test001' : 0,
    'test002' : 3,
    'test003' : 9,
    'test004' : 3524578,
}


tried = set()
passed = set()


def do_run(base, path):
    try:
      ret = subprocess.call([path])
      print '{0} -> {1}'.format(path, ret)
      if base in expected:
          if expected[base] == ret:
              passed.add(base)
          else:
              print '{0} did not return expected value'.format(path)
    except OSError:
      print 'failed to execute {0}'.format(path)


def do_link(base, path):
    if is_linux:
      out = './' + base
      if 0 == subprocess.call([LINK, '-e', 'Start', '-o', base, path]):
          do_run(base, out)
    else:
      out = base + '.exe'
      if 0 == subprocess.call([LINK, '/console', '/ni', path]):
          do_run(base, out)


def do_assemble(base, path):
    if is_linux:
      out = base + '.o'
      if 0 == subprocess.call([NASM, '-o', out, '-f', 'elf32', path]):
          do_link(base, out)
    else:
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
