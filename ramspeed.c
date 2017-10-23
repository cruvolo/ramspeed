/*
**  RAMspeed, a cache & memory benchmarking tool
**
**  (for uniprocessor machines running UNIX-like operating systems)
**
**
**  This software is distributed under the terms of The Alasir Licence (TAL).
**  You should have received a copy of the licence together with the software.
**  If not, you should download it from http://www.alasir.com/licence/TAL.txt
**
**
**  Copyright (c) 2002-05 Rhett M. Hollander <rhett@alasir.com>
**  Copyright (c) 2006-09 Paul V. Bolotoff <walter@alasir.com>
**
**  All rights reserved.
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "defines.h"

extern UTL intwr(UTL blksize, UTL passnum);
extern UTL intrd(UTL blksize, UTL passnum);
extern UTL floatwr(UTL blksize, UTL passnum);
extern UTL floatrd(UTL blksize, UTL passnum);
extern UTL intcp(UTL maxblk, UTL passnum);
extern UTL intsc(UTL maxblk, UTL passnum);
extern UTL intad(UTL maxblk, UTL passnum);
extern UTL inttr(UTL maxblk, UTL passnum);
extern UTL floatcp(UTL maxblk, UTL passnum);
extern UTL floatsc(UTL maxblk, UTL passnum);
extern UTL floatad(UTL maxblk, UTL passnum);
extern UTL floattr(UTL maxblk, UTL passnum);

#if (I386_ASM) || (AMD64_ASM)

#if (I386_ASM)

#if (FreeBSD)
#include <fcntl.h>
#elif (NetBSD) || (OpenBSD)
#include <sys/types.h>
#include <machine/sysarch.h>
#elif (Linux)
#include <sys/io.h>
#endif

extern U32 cpuinfo_main(U32 mode);
extern U32 cpuinfo_scalar_ext(U32 mode);
extern U32 cpuinfo_vector_ext(U32 mode);
extern U32 cpuinfo_general_ext(U32 mode);
extern U32 cpuinfo_address_ext(U32 mode);
extern U32 cpuinfo_monitor_ext(U32 mode);
extern U32 cpuinfo_other_ext(U32 mode);
extern U32 cpuinfo_htt_check(U32 mode);

#endif

extern U32 mmxwr(U32 blksize, U32 passnum);
extern U32 mmxrd(U32 blksize, U32 passnum);
extern U32 mmxcp(U32 maxblk, U32 passnum);
extern U32 mmxsc(U32 maxblk, U32 passnum);
extern U32 mmxad(U32 maxblk, U32 passnum);
extern U32 mmxtr(U32 maxblk, U32 passnum);

extern U32 ssewr(U32 blksize, U32 passnum);
extern U32 sserd(U32 blksize, U32 passnum);
extern U32 ssecp(U32 maxblk, U32 passnum);
extern U32 ssesc(U32 maxblk, U32 passnum);
extern U32 ssead(U32 maxblk, U32 passnum);
extern U32 ssetr(U32 maxblk, U32 passnum);

extern U32 mmxwr_nt(U32 blksize, U32 passnum);
extern U32 mmxrd_nt(U32 blksize, U32 passnum);
extern U32 mmxcp_nt(U32 maxblk, U32 passnum);
extern U32 mmxsc_nt(U32 maxblk, U32 passnum);
extern U32 mmxad_nt(U32 maxblk, U32 passnum);
extern U32 mmxtr_nt(U32 maxblk, U32 passnum);

extern U32 ssewr_nt(U32 blksize, U32 passnum);
extern U32 sserd_nt(U32 blksize, U32 passnum);
extern U32 ssecp_nt(U32 maxblk, U32 passnum);
extern U32 ssesc_nt(U32 maxblk, U32 passnum);
extern U32 ssead_nt(U32 maxblk, U32 passnum);
extern U32 ssetr_nt(U32 maxblk, U32 passnum);

extern U32 mmxcp_nt_t0(U32 maxblk, U32 passnum);
extern U32 mmxsc_nt_t0(U32 maxblk, U32 passnum);
extern U32 mmxad_nt_t0(U32 maxblk, U32 passnum);
extern U32 mmxtr_nt_t0(U32 maxblk, U32 passnum);

extern U32 ssecp_nt_t0(U32 maxblk, U32 passnum);
extern U32 ssesc_nt_t0(U32 maxblk, U32 passnum);
extern U32 ssead_nt_t0(U32 maxblk, U32 passnum);
extern U32 ssetr_nt_t0(U32 maxblk, U32 passnum);

#endif

void help(void) {

    printf("\n%s\n%s\n%s\n%s\n%s\n",
      "USAGE: ramspeed -b ID [-g size] [-m size] [-l runs]",
      "-b  runs a specified benchmark (by an ID number):",
      "     1 -- INTmark [writing]          4 -- FLOATmark [writing]",
      "     2 -- INTmark [reading]          5 -- FLOATmark [reading]",
      "     3 -- INTmem                     6 -- FLOATmem");

#if (I386_ASM) || (AMD64_ASM)
    printf("%s\n%s\n%s\n%s\n%s\n%s\n",
      "     7 -- MMXmark [writing]         10 -- SSEmark [writing]",
      "     8 -- MMXmark [reading]         11 -- SSEmark [reading]",
      "     9 -- MMXmem                    12 -- SSEmem",
      "    13 -- MMXmark (nt) [writing]    16 -- SSEmark (nt) [writing]",
      "    14 -- MMXmark (nt) [reading]    17 -- SSEmark (nt) [reading]",
      "    15 -- MMXmem (nt)               18 -- SSEmem (nt)");
#endif

    printf("%s\n%s\n%s\n%s\n%s\n",
      "-g  specifies a # of Gbytes per pass (default is 8)",
      "-m  specifies a # of Mbytes per array (default is 32)",
      "-l  enables the BatchRun mode (for *mem benchmarks only),",
      "    and specifies a # of runs (suggested is 5)",
      "-r  displays speeds in real megabytes per second (default: decimal)");

#if (I386_ASM) || (AMD64_ASM)
    printf("-t  changes software prefetch mode (see the manual for details)\n");
#endif

#if (I386_ASM)
    printf("-i  shows processor related information (CPUinfo) and exits\n\n");
#else
    printf("\n");
#endif

}

int main(int argc, char *argv[]) {
    UTL maxblk  = (MEMSIZE << 10);
    UTL passize = MEMPASS;
    U32 bid = 0, bnum = 1;
    F64 cplr = 0, adlr = 0, sclr = 0, trlr = 0, mb = 1048576.0;
    S8  bch = 'B';
    UTL blksize, passnum, time;
    STL opt, temp;
    U32 bmark, cnt;
    S32 ch;
    F64 cp, ad, sc, tr;
#if (I386_ASM)
    U32 cpuinfomode = 0, cpucaps = 0, sw_prf = 0;
    U32 mask, ret;
#elif (AMD64_ASM)
    U32 sw_prf = 0;
#endif

    printf("%s%s%s%s%s%s\n",
      "RAMspeed (", TARGET_OS, ") v", RVERSION,
      " by Rhett M. Hollander and Paul V. Bolotoff, 2002-", RYEAR);

#if (I386_ASM)
    while((ch = getopt(argc, argv, "b:g:m:l:t:ri")) != -1)
#elif (AMD64_ASM)
    while((ch = getopt(argc, argv, "b:g:m:l:t:r")) != -1)
#else
    while((ch = getopt(argc, argv, "b:g:m:l:p:r")) != -1)
#endif

      switch(ch) {

        case 'b':
	    opt = atol(optarg);
	    if(opt < 0) opt = 0;
	    bid = (U32)opt;
	    break;

	case 'g':
	    opt = atol(optarg);
	    temp = 1;
	    passize = 0;
	    for(cnt = 0; cnt < (sizeof(STL) << 3); cnt++) {
		if(opt == temp) passize = opt;
		temp = (temp << 1);
	    }
	    if(!passize) {
		printf("\nERROR: invalid pass size\n");
		return(1);
	    }
	    break;

	case 'm':
	    opt = (atol(optarg) << 10);
	    temp = 1;
	    maxblk = 0;
	    for(cnt = 0; cnt < (sizeof(STL) << 3); cnt++) {
		if(opt == temp) maxblk = opt;
		temp = (temp << 1);
	    }
	    if(!maxblk) {
		printf("\nERROR: invalid array size\n");
		return(1);
	    }
	    break;

	case 'l':
	    opt = atol(optarg);
	    if(opt < 1) opt = 1;
	    bnum = (U32)opt;
	    break;

#if (I386_ASM) || (AMD64_ASM)
	case 't':
	    opt = atol(optarg);
	    if((opt < 0) || (opt > 2)) {
		printf("\nERROR: invalid software prefetch mode\n");
		return(1);
	    }
	    sw_prf = (U32)opt;
	    break;
#endif

	case 'r':
	    mb = 1000000.0;
	    bch = 'b';
	    break;

#if (I386_ASM)
	case 'i':
	    cpuinfomode = 1;
	    break;
#endif

	default:
	    help();
	    return(1);

    }

#if (I386_ASM)
    if(cpuinfomode) {

    printf("\n");

/* the following OS dependent calls will fail without super-user's privileges */

#if (FreeBSD)    
    ret = open("/dev/io", O_RDWR);
    if(ret > 0) {
    	/* in verbose privileged mode */
    	mask = cpuinfo_main(3);
	close(ret);
    } else {
	/* in verbose regular mode */
    	mask = cpuinfo_main(1);
    }
#elif (NetBSD) || (OpenBSD)
    ret = i386_iopl(3);
    if(ret == 0) mask = cpuinfo_main(3);
    else mask = cpuinfo_main(1);
#elif (Linux)
    ret = iopl(3);
    if (ret == 0) mask = cpuinfo_main(3);
    else mask = cpuinfo_main(1);
#else
    /* ret is here just to shut up the compiler */
    ret = mask = cpuinfo_main(1);
#endif

    /* processor extensions detectable through CPUID */
    if(mask & 0x20000000) {
	cpuinfo_scalar_ext(1);
	cpuinfo_vector_ext(1);
	cpuinfo_general_ext(1);
	cpuinfo_address_ext(1);
	cpuinfo_monitor_ext(1);
	cpuinfo_other_ext(1);
    }

    /* check for Hyper-Threading (Intel Pentium 4 processors) */
    if((mask & 0x000F000F) == 0x000000F1) cpuinfo_htt_check(1);

    /* check fo Hyper-Threading (Intel Core processors [0x61xx]) */
    if((mask & 0x000FF00F) == 0x00061001) cpuinfo_htt_check(1);

    return(0);

    } else {

    /* in silent regular mode */
    ret = mask = cpuinfo_main(0);

    if(mask & 0x20000000) cpucaps = cpuinfo_vector_ext(0);

    }

#endif

    /* display the built-in help and exit if no benchmark ID is specified */
    if(!bid) {
	help();
	return(1);
    }

    /* in fact, per pass values are only true for *mark benchmarks;
     * when it comes to *mem ones, cp & sc use 2 times more, ad & tr -- 3 */
    printf("\n%luGb per pass mode\n\n", passize);

    passnum = (passize << 20)/maxblk;

    switch(bid) {

	case(1):
	    for(blksize = STARTBLK; blksize <= maxblk; blksize = (blksize << 1)) {
		passnum = (passize << 20)/blksize;
		time = intwr(blksize, passnum);
		printf("INTEGER & WRITING  %8lu Kb block: %.2f M%c/s\n",
		  blksize, (F64)(passize << 10)*mb/(F64)time, bch);
	    }
	    break;

	case(2):
	    for(blksize = STARTBLK; blksize <= maxblk; blksize = (blksize << 1)) {
		passnum = (passize << 20)/blksize;
		time = intrd(blksize, passnum);
		printf("INTEGER & READING  %8lu Kb block: %.2f M%c/s\n",
		  blksize, (F64)(passize << 10)*mb/(F64)time, bch);
	    }
	    break;

	case(3):
	    if(bnum > 1) printf("%u-benchmark INTmem BatchRun mode\n\n", bnum);
	    for(bmark = 0; bmark < bnum; bmark++) {
		if(bnum > 1) printf("Benchmark #%u:\n", bmark+1);
		time = intcp(maxblk, passnum);
		cp = (F64)(passize << 11)*mb/(F64)time;
		printf("INTEGER   Copy:      %.2f M%c/s\n", cp, bch);
		time = intsc(maxblk, passnum);
		sc = (F64)(passize << 11)*mb/(F64)time;
		printf("INTEGER   Scale:     %.2f M%c/s\n", sc, bch);
		time = intad(maxblk, passnum);
		ad = (F64)(passize*3072)*mb/(F64)time;
		printf("INTEGER   Add:       %.2f M%c/s\n", ad, bch);
		time = inttr(maxblk, passnum);
		tr = (F64)(passize*3072)*mb/(F64)time;
		printf("INTEGER   Triad:     %.2f M%c/s\n", tr, bch);
		printf("---\nINTEGER   AVERAGE:   %.2f M%c/s\n\n",
		  (cp + sc + ad + tr)/4.0, bch);
		if(bnum > 1) {
		    cplr += cp; sclr += sc; adlr += ad; trlr += tr;
		}
	    }
	    if(bnum > 1) {
		printf("INTEGER BatchRun   Copy:      %.2f M%c/s\n",
		  cplr/(F64)bnum, bch);
		printf("INTEGER BatchRun   Scale:     %.2f M%c/s\n",
		  sclr/(F64)bnum, bch);
		printf("INTEGER BatchRun   Add:       %.2f M%c/s\n",
		  adlr/(F64)bnum, bch);
		printf("INTEGER BatchRun   Triad:     %.2f M%c/s\n",
		  trlr/(F64)bnum, bch);
		printf("---\nINTEGER BatchRun   AVERAGE:   %.2f M%c/s\n",
		  (cplr + sclr + adlr + trlr)/(F64)(bnum << 2), bch);
	    }
	    break;

	case(4):
	    for(blksize = STARTBLK; blksize <= maxblk; blksize = (blksize << 1)) {
		passnum = (passize << 20)/blksize;
		time = floatwr(blksize, passnum);
		printf("FL-POINT & WRITING %8lu Kb block: %.2f M%c/s\n",
		  blksize, (F64)(passize << 10)*mb/(F64)time, bch);
	    }
	    break;

	case(5):   
	    for(blksize = STARTBLK; blksize <= maxblk; blksize = (blksize << 1)) {
		passnum = (passize << 20)/blksize;
		time = floatrd(blksize, passnum);
		printf("FL-POINT & READING %8lu Kb block: %.2f M%c/s\n",
		  blksize, (F64)(passize << 10)*mb/(F64)time, bch);
	    }
	    break;

	case(6):
	    if(bnum > 1) printf("%u-benchmark FLOATmem BatchRun mode\n\n", bnum);
	    else bnum = 1;
	    for(bmark = 0; bmark < bnum; bmark++) {
		if(bnum > 1) printf("Benchmark #%u:\n", bmark+1);
		time = floatcp(maxblk, passnum);
		cp = (F64)(passize << 11)*mb/(F64)time;
		printf("FL-POINT  Copy:      %.2f M%c/s\n", cp, bch);
		time = floatsc(maxblk, passnum);
		sc = (F64)(passize << 11)*mb/(F64)time;
		printf("FL-POINT  Scale:     %.2f M%c/s\n", sc, bch);
		time = floatad(maxblk, passnum);
		ad = (F64)(passize*3072)*mb/(F64)time;
		printf("FL-POINT  Add:       %.2f M%c/s\n", ad, bch);
		time = floattr(maxblk, passnum);
		tr = (F64)(passize*3072)*mb/(F64)time;
		printf("FL-POINT  Triad:     %.2f M%c/s\n", tr, bch);
		printf("---\nFL-POINT  AVERAGE:   %.2f M%c/s\n\n",
		  (cp + sc + ad + tr)/4.0, bch);
		if(bnum > 1) {
		    cplr += cp; sclr += sc; adlr += ad; trlr += tr;
		}
	    }
	    if(bnum > 1) {
		printf("FL-POINT BatchRun  Copy:      %.2f M%c/s\n",
		  cplr/(F64)bnum, bch);
		printf("FL-POINT BatchRun  Scale:     %.2f M%c/s\n",
		  sclr/(F64)bnum, bch);
		printf("FL-POINT BatchRun  Add:       %.2f M%c/s\n",
		  adlr/(F64)bnum, bch);
		printf("FL-POINT BatchRun  Triad:     %.2f M%c/s\n",
		  trlr/(F64)bnum, bch);
		printf("---\nFL-POINT BatchRun  AVERAGE:   %.2f M%c/s\n",
		  (cplr + sclr + adlr + trlr)/(F64)(bnum << 2), bch);
	    }
	    break;

#if (I386_ASM) || (AMD64_ASM)
	case(7):
#if (I386_ASM)
	    if(!(cpucaps & 0x1)) {
		printf("ERROR: no MMX support available\n");
		return(1);
	    }
#endif
	    for(blksize = STARTBLK; blksize <= maxblk; blksize = (blksize << 1)) {
		passnum = (passize << 20)/blksize;
		time = mmxwr(blksize, passnum);
		printf("MMX & WRITING      %8lu Kb block: %.2f M%c/s\n",
		  blksize, (F64)(passize << 10)*mb/(F64)time, bch);;
	    }
	    break;

	case(8):
#if (I386_ASM)
	    if(!(cpucaps & 0x1)) {
		printf("ERROR: no MMX support available\n");
		return(1);
	    }
#endif
	    for(blksize = STARTBLK; blksize <= maxblk; blksize = (blksize << 1)) {
		passnum = (passize << 20)/blksize;
		time = mmxrd(blksize, passnum);
		printf("MMX & READING      %8lu Kb block: %.2f M%c/s\n",
		  blksize, (F64)(passize << 10)*mb/(F64)time, bch);
	    }
	    break;

	case(9):
#if (I386_ASM)
	    if(!(cpucaps & 0x1)) {
		printf("ERROR: no MMX support available\n");
		return(1);
	    }
#endif
	    if(bnum > 1) printf("%u-benchmark MMXmem BatchRun mode\n\n", bnum);
	    else bnum = 1;
	    for(bmark = 0; bmark < bnum; bmark++) {
		if(bnum > 1) printf("Benchmark #%u:\n", bmark+1);
		time = mmxcp(maxblk, passnum);
		cp = (F64)(passize << 11)*1000000.0/(F64)time;
		cp = (F64)(passize << 11)*mb/(F64)time;
		printf("MMX       Copy:      %.2f M%c/s\n", cp, bch);
		time = mmxsc(maxblk, passnum);
		sc = (F64)(passize << 11)*mb/(F64)time;
		printf("MMX       Scale:     %.2f M%c/s\n", sc, bch);
		time = mmxad(maxblk, passnum);
		ad = (F64)(passize*3072)*mb/(F64)time;
		printf("MMX       Add:       %.2f M%c/s\n", ad, bch);
		time = mmxtr(maxblk, passnum);
		tr = (F64)(passize*3072)*mb/(F64)time;
		printf("MMX       Triad:     %.2f M%c/s\n", tr, bch);
		printf("---\nMMX       AVERAGE:   %.2f M%c/s\n\n",
		  (cp + sc + ad + tr)/4.0, bch);
		if(bnum > 1) {
		    cplr += cp; sclr += sc; adlr += ad; trlr += tr;
		}
	    }
	    if(bnum > 1) {
		printf("MMX BatchRun       Copy:      %.2f M%c/s\n",
		  cplr/(F64)bnum, bch);
		printf("MMX BatchRun       Scale:     %.2f M%c/s\n",
		  sclr/(F64)bnum, bch);
		printf("MMX BatchRun       Add:       %.2f M%c/s\n",
		  adlr/(F64)bnum, bch);
		printf("MMX BatchRun       Triad:     %.2f M%c/s\n",
		  trlr/(F64)bnum, bch);
		printf("---\nMMX BatchRun       AVERAGE:   %.2f M%c/s\n",
		  (cplr + sclr + adlr + trlr)/(F64)(bnum << 2), bch);
	    }
	    break;

	case(10):
#if (I386_ASM)
	    if(!(cpucaps & 0x10)) {
		printf("ERROR: no SSE support available\n");
		return(1);
	    }
#endif
	    for(blksize = STARTBLK; blksize <= maxblk; blksize = (blksize << 1)) {
		passnum = (passize << 20)/blksize;
		time = ssewr(blksize, passnum);
		printf("SSE & WRITING      %8lu Kb block: %.2f M%c/s\n",
		  blksize, (F64)(passize << 10)*mb/(F64)time, bch);
	    }
	    break;

	case(11):
#if (I386_ASM)
	    if(!(cpucaps & 0x10)) {
		printf("ERROR: no SSE support available\n");
		return(1);
	    }
#endif
	    for(blksize = STARTBLK; blksize <= maxblk; blksize = (blksize << 1)) {
		passnum = (passize << 20)/blksize;
		time = sserd(blksize, passnum);
		printf("SSE & READING      %8lu Kb block: %.2f M%c/s\n",
		  blksize, (F64)(passize << 10)*mb/(F64)time, bch);
	    }
	    break;

	case(12):
#if (I386_ASM)
	    if(!(cpucaps & 0x10)) {
		printf("ERROR: no SSE support available\n");
		return(1);
	    }
#endif
	    if(bnum > 1) printf("%u-benchmark SSEmem BatchRun mode\n\n", bnum);
	    else bnum = 1;
	    for(bmark = 0; bmark < bnum; bmark++) {
		if(bnum > 1) printf("Benchmark #%u:\n", bmark+1);
		time = ssecp(maxblk, passnum);
		cp = (F64)(passize << 11)*mb/(F64)time;
		printf("SSE       Copy:      %.2f M%c/s\n", cp, bch);
		time = ssesc(maxblk, passnum);
		sc = (F64)(passize << 11)*mb/(F64)time;
		printf("SSE       Scale:     %.2f M%c/s\n", sc, bch);
		time = ssead(maxblk, passnum);
		ad = (F64)(passize*3072)*mb/(F64)time;
		printf("SSE       Add:       %.2f M%c/s\n", ad, bch);
		time = ssetr(maxblk, passnum);
		tr = (F64)(passize*3072)*mb/(F64)time;
		printf("SSE       Triad:     %.2f M%c/s\n", tr, bch);
		printf("---\nSSE       AVERAGE:   %.2f M%c/s\n\n",
		  (cp + sc + ad + tr)/4.0, bch);
		if(bnum > 1) {
		    cplr += cp; sclr += sc; adlr += ad; trlr += tr;
		}
	    }
	    if(bnum > 1) {
		printf("SSE BatchRun       Copy:      %.2f M%c/s\n",
		  cplr/(F64)bnum, bch);
		printf("SSE BatchRun       Scale:     %.2f M%c/s\n",
		  sclr/(F64)bnum, bch);
		printf("SSE BatchRun       Add:       %.2f M%c/s\n",
		  adlr/(F64)bnum, bch);
		printf("SSE BatchRun       Triad:     %.2f M%c/s\n",
		  trlr/(F64)bnum, bch);
		printf("---\nSSE BatchRun       AVERAGE:   %.2f M%c/s\n\n",
		  (cplr + sclr + adlr + trlr)/(F64)(bnum << 2), bch);
	    }
	    break;

	case(13):
#if (I386_ASM)
	    if(!(cpucaps & 0x2)) {
		printf("ERROR: no MMX+ support available\n");
		return(1);
	    }
#endif
	    for(blksize = STARTBLK; blksize <= maxblk; blksize = (blksize << 1)) {
		passnum = (passize << 20)/blksize;
		time = mmxwr_nt(blksize, passnum);
		printf("MMX (nt) & WRITING %8lu Kb block: %.2f M%c/s\n",
		  blksize, (F64)(passize << 10)*mb/(F64)time, bch);
	    }
	    break;

	case(14):
#if (I386_ASM)
	    if(!(cpucaps & 0x2)) {
		printf("ERROR: no MMX+ support available\n");
		return(1);
	    }
#endif
	    for(blksize = STARTBLK; blksize <= maxblk; blksize = (blksize << 1)) {
		passnum = (passize << 20)/blksize;
		time = mmxrd_nt(blksize, passnum);
		printf("MMX (nt) & READING %8lu Kb block: %.2f M%c/s\n",
		  blksize, (F64)(passize << 10)*mb/(F64)time, bch);
	    }
	    break;

	case(15):
#if (I386_ASM)
	    if(!(cpucaps & 0x2)) {
		printf("ERROR: no MMX+ support available\n");
		return(1);
	    }
#endif
	    if(bnum > 1) printf("%u-benchmark MMXmem (non-temporal) BatchRun mode\n\n", bnum);
	    else bnum = 1;
	    for(bmark = 0; bmark < bnum; bmark++) {
		if(bnum > 1) printf("Benchmark #%u:\n", bmark+1);

		if(sw_prf == 2) time = mmxcp_nt_t0(maxblk, passnum);
		else time = mmxcp_nt(maxblk, passnum);
		cp = (F64)(passize << 11)*mb/(F64)time;
		printf("MMX (nt)  Copy:      %.2f M%c/s  ", cp, bch);
		if(sw_prf == 2) printf("[T0 prefetch]\n");
		else printf("[NTA prefetch]\n");

		if(sw_prf == 2) time = mmxsc_nt_t0(maxblk, passnum);
		else time = mmxsc_nt(maxblk, passnum);
		sc = (F64)(passize << 11)*mb/(F64)time;
		printf("MMX (nt)  Scale:     %.2f M%c/s  ", sc, bch);
		if(sw_prf == 2) printf("[T0 prefetch]\n");
		else printf("[NTA prefetch]\n");

		if(sw_prf == 1) time = mmxad_nt(maxblk, passnum);
		else time = mmxad_nt_t0(maxblk, passnum);
		ad = (F64)(passize*3072)*mb/(F64)time;
		printf("MMX (nt)  Add:       %.2f M%c/s  ", ad, bch);
		if(sw_prf == 1) printf("[NTA prefetch]\n");
		else printf("[T0 prefetch]\n");

		if(sw_prf == 1) time = mmxtr_nt(maxblk, passnum);
		else time = mmxtr_nt_t0(maxblk, passnum);
		tr = (F64)(passize*3072)*mb/(F64)time;
		printf("MMX (nt)  Triad:     %.2f M%c/s  ", tr, bch);
		if(sw_prf == 1) printf("[NTA prefetch]\n");
		else printf("[T0 prefetch]\n");

		printf("---\nMMX (nt)  AVERAGE:   %.2f M%c/s\n\n",
		  (cp + sc + ad + tr)/4.0, bch);
		if(bnum > 1) {
		    cplr += cp; sclr += sc; adlr += ad; trlr += tr;
		}
	    }
	    if(bnum > 1) {
		printf("MMX (nt) BatchRun  Copy:      %.2f M%c/s  ",
		  cplr/(F64)bnum, bch);
		if(sw_prf == 2) printf("[T0 prefetch]\n");
		else printf("[NTA prefetch]\n");
		printf("MMX (nt) BatchRun  Scale:     %.2f M%c/s  ",
		  sclr/(F64)bnum, bch);
		if(sw_prf == 2) printf("[T0 prefetch]\n");
		else printf("[NTA prefetch]\n");
		printf("MMX (nt) BatchRun  Add:       %.2f M%c/s  ",
		  adlr/(F64)bnum, bch);
		if(sw_prf == 1) printf("[NTA prefetch]\n");
		else printf("[T0 prefetch]\n");
		printf("MMX (nt) BatchRun  Triad:     %.2f M%c/s  ",
		  trlr/(F64)bnum, bch);
		if(sw_prf == 1) printf("[NTA prefetch]\n");
		else printf("[T0 prefetch]\n");
		printf("---\nMMX (nt) BatchRun  AVERAGE:   %.2f M%c/s\n\n",
		  (cplr + sclr + adlr + trlr)/(F64)(bnum << 2), bch);
	    }
	    break;

	case(16):
#if (I386_ASM)
	    if(!(cpucaps & 0x10)) {
		printf("ERROR: no SSE support available\n");
		return(1);
	    }
#endif
	    for(blksize = STARTBLK; blksize <= maxblk; blksize = (blksize << 1)) {
		passnum = (passize << 20)/blksize;
		time = ssewr_nt(blksize, passnum);
		printf("SSE (nt) & WRITING %8lu Kb block: %.2f M%c/s\n",
		  blksize, (F64)(passize << 10)*mb/(F64)time, bch);
	    }
	    break;

	case(17):
#if (I386_ASM)
	    if(!(cpucaps & 0x10)) {
		printf("ERROR: no SSE support available\n");
		return(1);
	    }
#endif
	    for(blksize = STARTBLK; blksize <= maxblk; blksize = (blksize << 1)) {
		passnum = (passize << 20)/blksize;
		time = sserd_nt(blksize, passnum);
		printf("SSE (nt) & READING %8lu Kb block: %.2f M%c/s\n",
		  blksize, (F64)(passize << 10)*mb/(F64)time, bch);
	    }
	    break;

	case(18):
#if (I386_ASM)
	    if(!(cpucaps & 0x10)) {
		printf("ERROR: no SSE support available\n");
		return(1);
	    }
#endif
	    if(bnum > 1) printf("%u-benchmark SSEmem (non-temporal) BatchRun mode\n\n", bnum);
	    else bnum = 1;
	    for(bmark = 0; bmark < bnum; bmark++) {
		if(bnum > 1) printf("Benchmark #%u:\n", bmark+1);

		if(sw_prf == 2) time = ssecp_nt_t0(maxblk, passnum);
		else time = ssecp_nt(maxblk, passnum);
		time = ssecp_nt(maxblk, passnum);
		cp = (F64)(passize << 11)*mb/(F64)time;
		printf("SSE (nt)  Copy:      %.2f M%c/s  ", cp, bch);
		if(sw_prf == 2) printf("[T0 prefetch]\n");
		else printf("[NTA prefetch]\n");

		if(sw_prf == 2) time = ssesc_nt_t0(maxblk, passnum);
		else time = ssesc_nt(maxblk, passnum);
		sc = (F64)(passize << 11)*mb/(F64)time;
		printf("SSE (nt)  Scale:     %.2f M%c/s  ", sc, bch);
		if(sw_prf == 2) printf("[T0 prefetch]\n");
		else printf("[NTA prefetch]\n");

		if(sw_prf == 1) time = ssead_nt(maxblk, passnum);
		else time = ssead_nt_t0(maxblk, passnum);
		ad = (F64)(passize*3072)*mb/(F64)time;
		printf("SSE (nt)  Add:       %.2f M%c/s  ", ad, bch);
		if(sw_prf == 1) printf("[NTA prefetch]\n");
		else printf("[T0 prefetch]\n");

		if(sw_prf == 1) time = ssetr_nt(maxblk, passnum);
		else time = ssetr_nt_t0(maxblk, passnum);
		tr = (F64)(passize*3072)*mb/(F64)time;
		printf("SSE (nt)  Triad:     %.2f M%c/s  ", tr, bch);
		if(sw_prf == 1) printf("[NTA prefetch]\n");
		else printf("[T0 prefetch]\n");

		printf("---\nSSE (nt)  AVERAGE:   %.2f M%c/s\n\n",
		  (cp + sc + ad + tr)/4.0, bch);
		if(bnum > 1) {
		    cplr += cp; sclr += sc; adlr += ad; trlr += tr;
		}
	    }
	    if(bnum > 1) {
		printf("SSE (nt) BatchRun  Copy:      %.2f M%c/s  ",
		  cplr/(F64)bnum, bch);
		if(sw_prf == 2) printf("[T0 prefetch]\n");
		else printf("[NTA prefetch]\n");
		printf("SSE (nt) BatchRun  Scale:     %.2f M%c/s  ",
		  sclr/(F64)bnum, bch);
		if(sw_prf == 2) printf("[T0 prefetch]\n");
		else printf("[NTA prefetch]\n");
		printf("SSE (nt) BatchRun  Add:       %.2f M%c/s  ",
		  adlr/(F64)bnum, bch);
		if(sw_prf == 1) printf("[NTA prefetch]\n");
		else printf("[T0 prefetch]\n");
		printf("SSE (nt) BatchRun  Triad:     %.2f M%c/s  ",
		  trlr/(F64)bnum, bch);
		if(sw_prf == 1) printf("[NTA prefetch]\n");
		else printf("[T0 prefetch]\n");
		printf("---\nSSE (nt) BatchRun  AVERAGE:   %.2f M%c/s\n\n",
		  (cplr + sclr + adlr + trlr)/(F64)(bnum << 2), bch);
	    }
	    break;
#endif

	default:
	    help();
	    printf("\nERROR: unknown benchmark ID\n");
	    return(1);
    }

    return(0);
}
