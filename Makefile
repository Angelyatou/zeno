O=arts/testsmoke.zsg
#O=arts/adaptiveTest.zsg
#O=arts/prim.zsg
#O=arts/testlitsock.zsg
#O=graphs/BulletRigidSim.zsg
# O=graphs/Xuben_ZFX_IISPH.zsg
#O=arts/ZFXv2.zsg
#O=arts/lowResMPM.zsg
#O=arts/literialconst.zsg
#O=arts/blendtest.zsg
#default: justrun
#default: optrun
default: run

#####################################################
###### THIS FILE IS USED BY ARCHIBATE AND ZHXX ######
## NORMAL USERS SHOULD USE `make -C build` INSTEAD ##
#####################################################

#O=arts/ZFXv2.zsg
#default: run

all:
	cmake -B build
	cmake --build build --parallel 

release_all:
	cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/tmp/tmp-install
	cmake --build build --parallel `python -c 'from multiprocessing import cpu_count;print(cpu_count())`

debug_all:
	cmake -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=/tmp/tmp-install
	cmake --build build --parallel `python -c 'from multiprocessing import cpu_count;print(cpu_count())`

configure:
	cmake -B build
	ccmake -B build

test: all
	build/tests/zentest

easygl: all
	build/projects/EasyGL/zeno_EasyGL_main

run: all
	ZEN_TIMER=/tmp/timer ZEN_OPEN=$O python3 -m zenqt

debug: all
	ZEN_TIMER=/tmp/timer ZEN_NOSIGHOOK=1 USE_GDB=1 ZEN_SPROC=1 ZEN_OPEN=$O gdb python3 -ex 'r -m zenqt'

optrun: all
	ZEN_TIMER=/tmp/timer ZEN_OPEN=$O optirun python3 -m zenqt

noviewrun: all
	ZEN_TIMER=/tmp/timer ZEN_NOFORK=1 ZEN_NOVIEW=1 ZEN_OPEN=$O python3 -m zenqt

justrun:
	ZEN_OPEN=$O python3 -m zenqt
