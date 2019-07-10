# Tools usage


```
subc -S <input.c>
```

This will generate a coresponding `input.s` file.

```
nasm.exe -o <outfile.obj> <infile> -f win32
```

This state will generate and output object file which must be linked.
Note that GoLink expects object files to have the .obj extension.

```
GoLink.exe  /console /entry _start <a.obj> <b.obj> ...
```

The /entry option doesnt seem to work so the entry should be `Start`.
