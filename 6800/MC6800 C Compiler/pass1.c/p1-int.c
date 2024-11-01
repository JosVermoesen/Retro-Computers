#include "pass1.h"
#include "il.h"
#include "nxtchr.h"


/* external definitions */

extern short curtok;
extern char blklev;
extern char matlev;
extern long convalu;
extern short contyp;
extern struct express *nxtmat;
extern struct symtab *symloc;
extern struct symtab **tos;
extern int ioff;
extern struct symtab *iname;
extern struct express emat[];
extern char ibindx, iwindx, ilindx, ilbindx;
extern char ibstk[];
extern short iwstk[];
extern long ilstk[];
extern struct ilab ilbstk[];
extern TABLE sym_table;
extern char cmexfl;
extern short strindx;
extern struct sstack *strngloc;
extern char initfld;
extern int crntfld;
extern unsigned fieldval;

/* do initialization parsing */

doinit() {
  char pflag = 0;
  register struct symtab *var = symloc;
  register struct express *p;
  short tp;

  curtok = 0;
  ibindx = iwindx = ilindx = ilbindx = 0;
  if (blklev) {
    if (blklev==1)
      return(rpters(20));
    if (var->stype & 0x30) {
      if ((var->stype&0x30) != (PTR<<4))
        return(rpters(21));
    }
    else {
      if (var->stype > DUBLE)
        return(rptern(23));
    }
    if (getok() == LCB) {
      curtok = 0;
      pflag++;
    }
    cmexfl++;
    if (!bildmat())
      return(FALSE);
    p=nxtmat++;
    p->moprtr = ASN;
    p->mo1loc = var;
    if (!matlev)
      p->mo2loc = *(--tos);
    else
      p->mo2loc = (struct symtab *) (matlev++);
    (p+1)->moprtr = 0;
    typechk();
    outexp();
    outstrg();
    if (pflag)
      if (getok() != RCB)
        return(rptern(17));
      else
        curtok = 0;
    return(TRUE);
  }
  else {
    if (!inthing(var))
      return(FALSE);
    iflush();
    return(TRUE);
  }
}

/* initialize an object */

inthing(var)
register struct symtab *var;
{
  short tp;

  tp = var->stype;
  if (tp > DUBLE)
    if (initfld) {
      ioff = fieldval;
      fieldval = 0;
      initfld = 0;
#ifdef ALIGN
      ilongs();
#else
      iwords();
#endif
    }
  if ((tp&0x30) == 0) {
    if (tp == STRUCT) {
      return(instruct(var));
    }
    else {
      if (tp > DUBLE)
        return(rpters(65));
      else {
        return(intype(var));
      }
    }
  }
  if ((tp&0x30) == (PTR<<4)) {
    return(intype(var));
  }
  if ((tp&0x30) == (ARAY<<4))
    return(inarray(var));
  return(rpters(24));
}

/* process data for scalar init types */

intype(p)
register struct symtab *p;
{
  char pflag = 0;

  if (p->sflags & FFIELD)
    return(infield(p));
  if (initfld) {
    ioff = fieldval;
    initfld = 0;
    fieldval = 0;
#ifdef ALIGN
    ilongs();
#else
    iwords();
#endif
  }
  if (getok() == LCB) {
    pflag++;
    curtok = 0;
  }
  if (getok() != RCB) {
    cmexfl++;
    if (!bildmat())
      return(FALSE);
  }
  else {
    emat[0].moprtr = NOP;
    emat[0].mttype = 0;
    emat[1].moprtr = 0;
    matlev = 1;
  }
  if (!oival(p->stype))
    return(FALSE);
  if (pflag)
    if (getok() != RCB)
      return(rptern(17));
    else
      curtok = 0;
  outstrg();
  return(TRUE);
}

/* output initialized data value */

oival(typ) {
  register struct express *p;
  char pflag;

  if (!matlev)
    enteru(LOD);
  iname = ioff = 0;
  pflag = 0;
  for (p=emat; p->moprtr; p++) {
    switch (p->moprtr) {
      case ADR:
        pflag++;
      case LOD:
        if (!iset(p->mo1loc))
          return(FALSE);
        break;
      case ADD:
        if (!iset(p->mo1loc))
          return(FALSE);
        if (!iset(p->mo2loc))
          return(FALSE);
      case NOP:
        break;
      case CVC:
        if (!iset(p->mo1loc))
          return(FALSE);
        if ((p->mttype&0x30)==(PTR<<4))
          pflag++;
        break;
      default:
        return(rptern(60));
    }
  }
  p--;
  if (p->mttype & 0x30)
    if ((p->mttype&0x30) == (PTR<<4))
      if (!pflag)
        return(rptern(60));
  if (!iname) {
#ifdef ALIGN
    if (typ < SHORT)
      ibytes();
    else if (typ < INT)
      iwords();
    else
      ilongs();
#else
    if (typ < SHORT)
      ibytes();
    else if (typ < LONG)
      iwords();
    else
      ilongs();
#endif
  }
  else {
    if ((!pflag) && (!(iname->stype & 0x30)))
      return(rptern(60));
    if (typ < INT)
      return(rptern(60));
    ilabels();
  }
  return(TRUE);
}

/* evaluate and set offset and or name field for initialization */

iset(p)
register struct symtab *p;
{
  int *ip;

  if ((unsigned)p < 256)
    return(TRUE);
  if (isconst(p->stype)) {
    ip = p;
    ioff += *(ip + (SIZINT/2));
  }
  else {
    if (iname)
      return(rptern(61));
    else
      iname = p;
  }
  return(TRUE);
}

/* set byte value */

ibytes() {
  if (iwindx)
    owords();
  else if (ilindx)
    olongs();
  else if (ilbindx)
    olabels();
  if (ibindx == 16)
    obytes();
  ibstk[ibindx++] = ioff;
}

/* set word value */

iwords() {
  if (ibindx)
    obytes();
  else if (ilindx)
    olongs();
  else if (ilbindx)
    olabels();
  if (iwindx == 16)
    owords();
  iwstk[iwindx++] = ioff;
}

/* set long value */

ilongs() {
  if (ibindx)
    obytes();
  else if (iwindx)
    owords();
  else if (ilbindx)
    olabels();
  if (ilindx == 16)
    olongs();
  ilstk[ilindx++] = ioff;
}

/* set label value */

ilabels() {
  if (ibindx)
    obytes();
  else if (iwindx)
    owords();
  else if (ilindx)
    olongs();
  if (ilbindx == 16)
    olabels();
  ilbstk[ilbindx].iofset = ioff;
  if (!iname) {
    ilbstk[ilbindx].ityp = 1;
    ilbstk[ilbindx++].ilabn = 0;
  }
  else {
    if (iname->sclass == STAT) {
      ilbstk[ilbindx].ityp = 0;
      ilbstk[ilbindx++].ilabn = iname->sstore;
    }
    else {
      ilbstk[ilbindx].ityp = 2;
      ilbstk[ilbindx++].ilabn = (int) (iname->sname);
    }
  }
}

/* flush all initialized data */

iflush() {
  if (initfld) {
    ioff = fieldval;
    initfld = 0;
    fieldval = 0;
#ifdef ALIGN
    ilongs();
#else
    iwords();
#endif
  }
  if (ibindx)
    obytes();
  else if (iwindx)
    owords();
  else if (ilindx)
    olongs();
  else if (ilbindx)
    olabels();
}

/* initialize array */

inarray(p)
register struct symtab *p;
{
  register int *ip;
  int size, i;
  char *sp;

  if (p->stype==((ARAY<<4)|CHR) && getok()==STC) {
    curtok = 0;
    strindx--;
    size = 0;
    sp = strngloc->stptr;
    while (*sp) {
      ioff = *sp++;
      size++;
      ibytes();
    }
    size++;
    ioff = 0;
    ibytes();
    ip = p->ssubs;
    if (*ip) {
      if (*(ip+1) != -1)
        return(rptern(51));
      if (size > *ip)
        return(rptern(52));
      else
        for (i=size; i<*ip; i++)
          ibytes();
    }
    else
      *ip = size;
  }
  else
    return(insubs(p, p->ssubs));
}

/* initialize a structure */

instruct(var)
register struct symtab *var;
{
  register struct symtab *p;
  char pflag = 0;

  if (getok() == LCB) {
    pflag++;
    curtok = 0;
  }
  p = sym_table->fwad + (var->sstrct-1);
  while (p) {
    inthing(p);
    outstrg();
    if (getok() == CMA)
      curtok = 0;
    if (p->spoint)
      p = sym_table->fwad + (p->spoint-1);
    else
      break;
  }
  if (getok() == CMA)
    curtok = 0;
  if (pflag)
    if (getok() == RCB)
      curtok = 0;
    else
      return(rptern(17));
  return(TRUE);
}

/* initialize a set of subscriptd */

insubs(vp, ip)
register struct symtab *vp;
register int *ip;
{
  int savtyp, count, i;
  int dimc = 0;
  char err = 0;
  char pflag = 0;

  if (getok() == LCB) {
    curtok = 0;
    pflag++;
  }
  count = *ip++;
  savtyp = vp->stype;
  vp->stype = remvlev(vp->stype);
  if (*ip != -1) {
    for (i=0; i<count; i++) {
      insubs(vp, ip);
      if (getok() == CMA)
        curtok = 0;
    }
  }
  else {
    if (count) {
      for (i=0; i<count; i++) {
        if (!inthing(vp)) {
          err++;
          break;
        }
        if (getok() == CMA)
          curtok = 0;
      }
    }
    else {
      while (getok() != RCB) {
        if (!inthing(vp)) {
          err++;
          break;
        }
        dimc++;
        if (getok() == CMA)
          curtok = 0;
      }
      *(ip-1) = dimc;
    }
  }
  vp->stype = savtyp;
  if (pflag)
    if (getok() != RCB)
      return(rptern(17));
    else
      curtok = 0;
  return(!err);
}

/* initialize a fielded variable */

infield(p)
register struct symtab *p;
{
  unsigned fval;
  short i;
  unsigned mask;
  char pflag = 0;

  if (getok() == LCB) {
    pflag++;
    curtok = 0;
  }
  if (getok() != RCB)
    cexp();
  else
    convalu = 0L;
  if (initfld && crntfld != p->sstore) {
    ioff = fieldval;
    fieldval = 0;
#ifdef ALIGN
    ilongs();
#else
    iwords();
#endif
  }
  initfld = 1;
  crntfld = p->sstore;
  fval = (unsigned) convalu;
  mask = 0;
  for (i = p->sstrct & 0xff; i; i--)
    mask = (mask << 1) | 1;
  if (fval > mask) {
    symloc = p;
    rpters(103);
  }
  fieldval |= (fval & mask) << ((p->sstrct >> 8) & 0xFF);
  if (pflag)
    if (getok() != RCB)
      return(rptern(17));
    else
      curtok = 0;
  return(TRUE);
}

