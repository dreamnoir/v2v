#
# OMNeT++/OMNEST Makefile for libv2v
#
# This file was generated with the command:
#  opp_makemake -f --deep --make-so -O out -I../mixim-sommer/base -I../mixim-sommer/base/utils -I../mixim-sommer/base/messages -I../mixim-sommer/base/modules -I../mixim-sommer/base/connectionManager -I../mixim-sommer/base/phyLayer -L../mixim-sommer/out/$(CONFIGNAME) -L../mixim-sommer/out/$(CONFIGNAME)/base -L../mixim-sommer/out/$(CONFIGNAME)/modules -L../mixim-sommer/out/$(CONFIGNAME)/tests/testUtils -lmixim -lmiximbase -lmiximmodules -lmiximtestUtils -KMIXIM_SOMMER_PROJ=../mixim-sommer
#

# Name of target to be created (-o option)
TARGET = libv2v$(SHARED_LIB_SUFFIX)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I../mixim-sommer/base \
    -I../mixim-sommer/base/utils \
    -I../mixim-sommer/base/messages \
    -I../mixim-sommer/base/modules \
    -I../mixim-sommer/base/connectionManager \
    -I../mixim-sommer/base/phyLayer \
    -I. \
    -Imessages \
    -Imessages/application \
    -Imessages/netw \
    -Imodules \
    -Imodules/application \
    -Imodules/netw \
    -Inetworks \
    -Inetworks/highway \
    -Inetworks/london \
    -Inetworks/manhattan \
    -Inetworks/small \
    -Iv2v \
    -Iv2v/bitmaps \
    -Iv2v/results

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L../mixim-sommer/out/$(CONFIGNAME) -L../mixim-sommer/out/$(CONFIGNAME)/base -L../mixim-sommer/out/$(CONFIGNAME)/modules -L../mixim-sommer/out/$(CONFIGNAME)/tests/testUtils  -lmixim -lmiximbase -lmiximmodules -lmiximtestUtils
LIBS += -Wl,-rpath,`abspath ../mixim-sommer/out/$(CONFIGNAME)` -Wl,-rpath,`abspath ../mixim-sommer/out/$(CONFIGNAME)/base` -Wl,-rpath,`abspath ../mixim-sommer/out/$(CONFIGNAME)/modules` -Wl,-rpath,`abspath ../mixim-sommer/out/$(CONFIGNAME)/tests/testUtils`

# Output directory
PROJECT_OUTPUT_DIR = out
PROJECTRELATIVE_PATH =
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = $O/modules/application/TimedApplLayer.o $O/modules/netw/WSMNetwLayer.o $O/messages/application/CCWSApplPkt_m.o $O/messages/netw/WSMPkt_m.o

# Message files
MSGFILES = \
    messages/application/CCWSApplPkt.msg \
    messages/netw/WSMPkt.msg

# Other makefile variables (-K)
MIXIM_SOMMER_PROJ=../mixim-sommer

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppenvir$D $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $(TARGET)

$(TARGET) : $O/$(TARGET)
	$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	$(SHLIB_LD) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(LIBS) $(OMNETPP_LIBS) $(LDFLAGS)
	$(SHLIB_POSTPROCESS) $O/$(TARGET)

.PHONY:

.SUFFIXES: .cc

$O/%.o: %.cc
	@$(MKPATH) $(dir $@)
	$(CXX) -c $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	-rm -rf $O
	-rm -f v2v v2v.exe libv2v.so libv2v.a libv2v.dll libv2v.dylib
	-rm -f ./*_m.cc ./*_m.h
	-rm -f messages/*_m.cc messages/*_m.h
	-rm -f messages/application/*_m.cc messages/application/*_m.h
	-rm -f messages/netw/*_m.cc messages/netw/*_m.h
	-rm -f modules/*_m.cc modules/*_m.h
	-rm -f modules/application/*_m.cc modules/application/*_m.h
	-rm -f modules/netw/*_m.cc modules/netw/*_m.h
	-rm -f networks/*_m.cc networks/*_m.h
	-rm -f networks/highway/*_m.cc networks/highway/*_m.h
	-rm -f networks/london/*_m.cc networks/london/*_m.h
	-rm -f networks/manhattan/*_m.cc networks/manhattan/*_m.h
	-rm -f networks/small/*_m.cc networks/small/*_m.h
	-rm -f v2v/*_m.cc v2v/*_m.h
	-rm -f v2v/bitmaps/*_m.cc v2v/bitmaps/*_m.h
	-rm -f v2v/results/*_m.cc v2v/results/*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc messages/*.cc messages/application/*.cc messages/netw/*.cc modules/*.cc modules/application/*.cc modules/netw/*.cc networks/*.cc networks/highway/*.cc networks/london/*.cc networks/manhattan/*.cc networks/small/*.cc v2v/*.cc v2v/bitmaps/*.cc v2v/results/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/messages/application/CCWSApplPkt_m.o: messages/application/CCWSApplPkt_m.cc \
	messages/application/CCWSApplPkt_m.h
$O/messages/netw/WSMPkt_m.o: messages/netw/WSMPkt_m.cc \
	messages/netw/WSMPkt_m.h
$O/modules/application/TimedApplLayer.o: modules/application/TimedApplLayer.cc \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseBattery.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/FWMath.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/HostState.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseUtility.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/TestApplLayer.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseLayer.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/Coord.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseApplLayer.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/ImNotifiable.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BatteryAccess.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/PassedMessage.h \
	modules/application/TimedApplLayer.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/Blackboard.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/Move.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseModule.h \
	$(MIXIM_SOMMER_PROJ)/base/messages/ApplPkt_m.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/FindModule.h
$O/modules/netw/WSMNetwLayer.o: modules/netw/WSMNetwLayer.cc \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseBattery.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/FWMath.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/HostState.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/ArpInterface.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseUtility.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/SimpleAddress.h \
	$(MIXIM_SOMMER_PROJ)/base/messages/NetwPkt_m.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseLayer.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/Coord.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/ImNotifiable.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BatteryAccess.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/Blackboard.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/PassedMessage.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseNetwLayer.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseModule.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/FindModule.h \
	modules/netw/WSMNetwLayer.h

