# I really am not happy about using symlinks to make this work.
#
# I think that there should be a better way to do this.
# see module26.sh in packaging/makefiles
#

OPENSWANSRCDIR=$(shell cd ../../..;pwd)

include ${OPENSWANSRCDIR}/Makefile.inc

UMLPLAIN=$(shell source ${OPENSWANSRCDIR}/umlsetup.sh; echo $${POOLSPACE}/plain$${KERNVER})
KLIPS_TOP=${OPENSWANSRCDIR}/linux

INPUTS=${OPENSWANSRCDIR}/testing/klips/inputs/02-sunrise-sunset-esp.pcap cleartext.pcap
PCAP2SKB=${OPENSWANSRCDIR}/testing/utils/pcap2skb/pcap2skb

CC=gcc 
KLIPS_FLAGS=-I${OPENSWANSRCDIR}/testing/kunit/include
KLIPS_FLAGS+=-I${KLIPS_TOP}/include 
#KLIPS_FLAGS+=-I${UMLPLAIN}/include
#KLIPS_FLAGS+=-I${UMLPLAIN}/arch/um/include
#KLIPS_FLAGS+=-I${UMLPLAIN}/arch/um/kernel/tt/include
#KLIPS_FLAGS+=-I${UMLPLAIN}/arch/um/kernel/skas/include
KLIPS_FLAGS+=-D__KERNEL__ -DKUNIT
KLIPS_FLAGS+=-g -O2 -U__i386__ -Ui386  -D__arch_um__ -DSUBARCH=\"i386\" 
KLIPS_FLAGS+=-D_LARGEFILE64_SOURCE 
KLIPS_FLAGS+=-Wall -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing
KLIPS_FLAGS+=-fno-common -ffreestanding -fomit-frame-pointer

KLIPS_FLAGS+=-DCONFIG_NETFILTER

UCFLAGS=${KLIPS_FLAGS}

#KCFLAGS+=-nostdinc -isystem /usr/lib/gcc-lib/i486-linux/3.3.5/include
KCFLAGS+=${KLIPS_FLAGS} 
KCFLAGS+=-DHASH_DEBUG
KCFLAGS+=-include ${OPENSWANSRCDIR}/packaging/linus/config-all.h
KCFLAGS+=-DIPCOMP_PREFIX  -DMODULE -DKBUILD_MODNAME=rcv01test

KLIPS_TOP:= ${OPENSWANSRCDIR}/linux
KLIPSSRC=${KLIPS_TOP}/net/ipsec
KUTILLIB=../libkern/libkern.a

IPSEC_OBJS=ipsec_rcv.o ipsec_esp.o ipsec_ah.o ipsec_ipcomp.o
IPSEC_OBJS+=ipsec_md5c.o ipsec_sha1.o ipsec_sa.o 
IPSEC_OBJS+=ipsec_life.o
IPSEC_OBJS+=cbc_enc.o set_key.o des_enc.o ecb_enc.o ipsec_alg_3des.o
IPSEC_OBJS+=ipsec_snprintf.o ipsec_alg.o
IPSEC_OBJS+=aes.o aes_cbc.o aes_xcbc_mac.o ipsec_alg_aes.o

rcv01main:	rcv01main.o ${IPSEC_OBJS} Makefile
	${CC} -g -o rcv01main rcv01main.o ${IPSEC_OBJS} ${KUTILLIB} ${OPENSWANLIB} -lefence

rcv01main.o: rcv01main.c rcv01pack.c 
	${CC} ${UCFLAGS} -c -o rcv01main.o rcv01main.c

rcv01pack.c: ${INPUTS}
	${PCAP2SKB} ${INPUTS} >$*.c

%.o: %.c 
	${CC} ${UCFLAGS} -DKBUILD_BASENAME=$* -c -o $*.o $< 

%.o: ${KLIPSSRC}/%.c
	${CC} ${KCFLAGS} -DKBUILD_BASENAME=$* -c -o $*.o $< 

%.o: ${KLIPSSRC}/des/%.c
	${CC} ${KCFLAGS} -DKBUILD_BASENAME=$* -c -o $*.o $< 

%.o: ${KLIPSSRC}/aes/%.c
	${CC} ${KCFLAGS} -DKBUILD_BASENAME=$* -c -o $*.o $< 

# build version.c using version number from Makefile.ver
version.c:	${KLIPSSRC}/version.in.c ${OPENSWANSRCDIR}/Makefile.ver
	sed '/"/s/xxx/$(IPSECVERSION)/' ${KLIPSSRC}/version.in.c >$@

check:	rcv01main
	(./rcv01main && echo done)| sed -f ../fixup.sed | tee rcv01main-check.txt | diff - rcv01main.out

clean:
	rm -rf *.o rcv01pack.c

# 
# Local Variables: 
# compile-command: "make"
# End: 
