: ==== start ====
TESTNAME=pluto-algo-01
source /testing/pluto/bin/eastlocal.sh

ipsec setup start
ipsec auto --add westnet-eastnet-cross
/testing/pluto/basic-pluto-01/eroutewait.sh trap
