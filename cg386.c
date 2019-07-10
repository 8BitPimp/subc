/*
 *	NMH's Simple C Compiler, 2011,2012
 *	386 target description
 */

#include "defs.h"
#include "data.h"
#include "decl.h"
#include "cgen.h"


void cgdata(void) { gen("section .data"); }
void cgtext(void) { gen("section .text"); }

// file prelude
void cgprelude(void) { gen("[BITS 32]"); }
// file postlude
void cgpostlude(void) {}

void cgpublic(char *s) { ngen("global %s", s, 0); }

// literal
void cglit(int v) { vgen("mov eax, %d", v); }
// clear register
void cgclear(void) { gen("xor eax, eax"); }
// load global byte
void cgldgb(char *s) { sgen("%s al, %s", "mov", s); }
// load global word
void cgldgw(char *s) { sgen("%s eax, %s", "mov", s); }
// load local byte
void cgldlb(int n) { ngen("%s al, [ebp + %d]", "mov", n); }
// load local word
void cgldlw(int n) { ngen("%s eax, [ebp + %d]", "mov", n); }

// load static?
void cgldsb(int n) { lgen("%s al, %c%d", "mov", n); }
void cgldsw(int n) { lgen("%s eax, %c%d", "mov", n); }

// load local address
void cgldla(int n) { ngen("%s eax, [ebp + %d]", "lea", n); }

// load static address ?
void cgldsa(int n) { lgen("%s eax, $%c%d", "mov", n); }
// load global address ?
void cgldga(char *s) { sgen("%s eax, $%s", "mov", s); }

// indirect byte
void cgindb(void) {
  gen("mov edx, eax");
  cgclear();
  gen("mov al, [edx]");
}

// indirect dword
void cgindw(void) { gen("mov eax, [eax]"); }

// get argument count (abi)
void cgargc(void) {
  // +8  __argc
  // +4  return address
  //  0  old esp
  // -4  local 1
  // ...
  gen("mov eax, [ebp + 8]");
}

void cgldlab(int id) { lgen("%s eax, $%c%d", "mov", id); }

void cgpush(void) { gen("push eax"); }
void cgpushlit(int n) { ngen("%s %d", "push", n); }
void cgpop2(void) { gen("pop ecx"); }
void cgswap(void) { gen("xchg ecx, eax"); }

void cgand(void) { gen("and eax, ecx"); }
void cgxor(void) { gen("xor eax, ecx"); }
void cgior(void) { gen("or eax, ecx"); }
void cgadd(void) { gen("add eax, ecx"); }
void cgmul(void) { gen("imul eax, ecx"); }
void cgsub(void) { gen("sub eax, ecx"); }

void cgdiv(void) {
  gen("cdq");  // EDX:EAX = sign-extend of EAX
  gen("idiv ecx");
}
void cgmod(void) {
  cgdiv();
  gen("mov eax, edx");
}
void cgshl(void) { gen("shl eax, cl"); }
void cgshr(void) { gen("sar eax, cl"); }
void cgcmp(char *inst) {
  int lab;
  lab = label();
  gen("xor edx, edx");
  cgpop2();
  gen("cmp ecx, eax");
  lgen("%s %c%d", inst, lab);
  gen("inc edx");
  genlab(lab);
  gen("mov eax, edx");
}
void cgeq() { cgcmp("jne"); }
void cgne() { cgcmp("je"); }
void cglt() { cgcmp("jge"); }
void cggt() { cgcmp("jle"); }
void cgle() { cgcmp("jg"); }
void cgge() { cgcmp("jl"); }

void cgneg(void) { gen("neg eax"); }
void cgnot(void) { gen("not eax"); }

void cglognot(void) {
#if 0
  gen("neg eax");
  gen("sbb eax, eax");
  gen("inc eax");
#else
  // shorter and more readable
  gen("test eax, eax");
  gen("setz eax");
#endif
}
void cgscale(void) { gen("shl eax, 2"); }
void cgscale2(void) { gen("shl ecx, 2"); }
void cgunscale(void) { gen("shr eax, 2"); }

void cgbool(void) {
#if 0
  gen("neg eax");
  gen("sbb eax, eax");
  gen("neg eax");
#else
  // shorter and more readable
  gen("test eax, eax");
  gen("setnz al");
  gen("and eax, 1");
#endif
}

void cgldinc(void) { gen("mov edx, eax"); }
void cginc1pi(void) { ngen("%s [eax], 4", "add", 0); }
void cgdec1pi(void) { ngen("%s [eax], 4", "sub", 0); }
void cginc2pi(void) { ngen("%s [edx], 4", "add", 0); }
void cgdec2pi(void) { ngen("%s [edx], 4", "sub", 0); }
void cgincpl(int a) { ngen("%s [ebp + %d], 4", "add", a); }
void cgdecpl(int a) { ngen("%s [ebp + %d], 4", "sub", a); }

void cgincps(int a) { lgen("%s %c%d, 4", "add", a); }
void cgdecps(int a) { lgen("%s %c%d, 4", "sub", a); }
void cgincpg(char *s) { sgen("%s %s, 4", "add", s); }
void cgdecpg(char *s) { sgen("%s %s, 4", "sub", s); }
void cginc1iw(void) { ngen("%s dword [eax]", "inc", 0); }
void cgdec1iw(void) { ngen("%s dword [eax]", "dec", 0); }
void cginc2iw(void) { ngen("%s dword [edx]", "inc", 0); }
void cgdec2iw(void) { ngen("%s dword [edx]", "dec", 0); }
void cginclw(int a) { ngen("%s dword [ebp + %d]", "inc", a); }
void cgdeclw(int a) { ngen("%s dword [ebp + %d]", "dec", a); }
void cgincsw(int a) { lgen("%s %c%d", "inc", a); }
void cgdecsw(int a) { lgen("%s %c%d", "dec", a); }
void cgincgw(char *s) { sgen("%s %s", "inc", s); }
void cgdecgw(char *s) { sgen("%s %s", "dec", s); }
void cginc1ib(void) { ngen("%s byte [eax]", "inc", 0); }
void cgdec1ib(void) { ngen("%s byte [eax]", "dec", 0); }
void cginc2ib(void) { ngen("%s byte [edx]", "inc", 0); }
void cgdec2ib(void) { ngen("%s byte [edx]", "dec", 0); }
void cginclb(int a) { ngen("%s byte [ebx + %d]", "inc", a); }
void cgdeclb(int a) { ngen("%s byte [ebx + %d]", "dec", a); }
void cgincsb(int a) { lgen("%s %c%d", "inc", a); }
void cgdecsb(int a) { lgen("%s %c%d", "dec", a); }
void cgincgb(char *s) { sgen("%s %s", "inc", s); }
void cgdecgb(char *s) { sgen("%s %s", "dec", s); }

void cgbr(char *how, int n) {
  // this looks damned inefficient
  int lab;
  lab = label();
  gen("or eax, eax");   // why not test?
  lgen("%s %c%d", how, lab);
  lgen("%s %c%d", "jmp", n);
  genlab(lab);
}
void cgbrtrue(int n) { cgbr("jz", n); }
void cgbrfalse(int n) { cgbr("jnz", n); }

void cgjump(int n) { lgen("%s %c%d", "jmp", n); }
void cgldswtch(int n) { lgen("%s edx, %c%d", "mov", n); }

void cgcalswtch(void) { gen("jmp\tswitch"); }               // ?
void cgcase(int v, int l) { lgen2(".long %d,%c%d", v, l); } // ?

void cgpopptr(void) { gen("pop edx"); }
void cgstorib(void) { ngen("%s [edx], al", "mov", 0); }
void cgstoriw(void) { ngen("%s [edx], eax", "mov", 0); }

void cgstorlb(int n) {
  ngen("%s [ebp + %d], al", "mov", n);
}

void cgstorlw(int n) {
  ngen("%s [ebp + %d], eax", "mov", n);
}

void cgstorsb(int n) { lgen("%s %c%d, al", "mov", n); }
void cgstorsw(int n) { lgen("%s %c%d, eax", "mov", n); }
void cgstorgb(char *s) { sgen("%s %s, al", "mov", s); }
void cgstorgw(char *s) { sgen("%s %s, eax", "mov", s); }

// init local word
void cginitlw(int v, int a) { ngen2("%s dword [ebp + %d], %d", "mov", a, v); }

// call function
void cgcall(char *s) { sgen("%s %s", "call", s); }

// call register
void cgcalr(void) {
  gen("call eax");
}

// alloca
void cgstack(int n) {
  if (n > 0) {
    ngen("%s esp, %d", "add", n);
  }
  if (n < 0) {
    // `sub esp, n` more readable then `add esp, -n`
    ngen("%s esp, %d", "sub", -n);
  }
}

// prologue
void cgentry(void) {
  // push new stack frame
  gen("push ebp");
  gen("mov ebp, esp");
}

// epilogue
void cgexit(void) {
  // pop stack frame and return
  gen("pop ebp");
  gen("ret");
}

void cgdefb(int v) { ngen("%s %d", "db", v); }
void cgdefw(int v) { ngen("%s %d", "dw", v); }
void cgdefp(int v) { ngen("%s %d", "dw", v); }
void cgdefl(int v) { lgen("%s %c%d", "dw", v); }
void cgdefc(int c) { ngen("%s '%c'", "db", c); }
void cgdefs(char *s, int len) {
  int i;
  genraw("\tdb ");
  for (i = 0; i < len; ++i) {
#if 0
    if (s[i] < 32) {
      // XXX: handle escape chars here!
    }
    else {
      genchar(s[i]);
    }
#else
    if (i > 0) {
      fprintf(Outfile, ", %02xh", s[i]);
    }
    else {
      fprintf(Outfile, "%02xh", s[i]);
    }
#endif
  }
  genraw("\n");
}

// ?
void cgbss(char *s, int z) { ngen("common %s %d", s, z); }

