#
# OMNeT++/OMNEST Makefile for v2v
#
# This file was generated with the command:
#  opp_makemake -f --deep -o v2v -O out -I../mixim-sommer/base/utils -I../mixim-sommer/base -I../mixim-sommer/base/messages -I../mixim-sommer/base/modules -I../mixim-sommer/base/connectionManager -I../mixim-sommer/base/phyLayer -I../mixim-sommer/modules/analogueModel -I../mixim-sommer/modules/phy -I../mixim-sommer/modules/mac -I../mixim-sommer/modules/obstacle -I../mixim-sommer/modules/messages -I../mixim-sommer/modules -I../mixim-sommer/modules/utility -I../mixim-sommer/modules/mobility/traci -L../mixim-sommer/out/$(CONFIGNAME)/base -L../mixim-sommer/out/$(CONFIGNAME)/modules -L../mixim-sommer/out/$(CONFIGNAME)/tests/testUtils -lmiximbase -lmiximmodules -lmiximtestUtils -KMIXIM_SOMMER_PROJ=../mixim-sommer
#

# Name of target to be created (-o option)
TARGET = v2v$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I../mixim-sommer/base/utils \
    -I../mixim-sommer/base \
    -I../mixim-sommer/base/messages \
    -I../mixim-sommer/base/modules \
    -I../mixim-sommer/base/connectionManager \
    -I../mixim-sommer/base/phyLayer \
    -I../mixim-sommer/modules/analogueModel \
    -I../mixim-sommer/modules/phy \
    -I../mixim-sommer/modules/mac \
    -I../mixim-sommer/modules/obstacle \
    -I../mixim-sommer/modules/messages \
    -I../mixim-sommer/modules \
    -I../mixim-sommer/modules/utility \
    -I../mixim-sommer/modules/mobility/traci \
    -I. \
    -Imessages \
    -Imessages/application \
    -Imessages/netw \
    -Imodules \
    -Imodules/application \
    -Imodules/control \
    -Imodules/mac \
    -Imodules/mobility \
    -Imodules/netw \
    -Imodules/nic \
    -Imodules/phy \
    -Imodules/utils \
    -Imodules/vision \
    -Inetworks \
    -Inetworks/highway \
    -Inetworks/linux \
    -Inetworks/london \
    -Inetworks/manhattan \
    -Inetworks/small \
    -Inetworks/windows \
    -Isim \
    -Isim/bitmaps \
    -Isim/results

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L../mixim-sommer/out/$(CONFIGNAME)/base -L../mixim-sommer/out/$(CONFIGNAME)/modules -L../mixim-sommer/out/$(CONFIGNAME)/tests/testUtils  -lmiximbase -lmiximmodules -lmiximtestUtils
LIBS += -Wl,-rpath,`abspath ../mixim-sommer/out/$(CONFIGNAME)/base` -Wl,-rpath,`abspath ../mixim-sommer/out/$(CONFIGNAME)/modules` -Wl,-rpath,`abspath ../mixim-sommer/out/$(CONFIGNAME)/tests/testUtils`

# Output directory
PROJECT_OUTPUT_DIR = out
PROJECTRELATIVE_PATH =
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/modules/application/CCWSApplLayer.o \
    $O/modules/mac/Mac80211p.o \
    $O/modules/mobility/TraCIMobilityV.o \
    $O/modules/netw/WSMNetwLayer.o \
    $O/modules/phy/Decider80211p.o \
    $O/modules/phy/Phy80211pLayer.o \
    $O/modules/utils/PositionEstimator.o \
    $O/modules/vision/VisionEntry.o \
    $O/modules/vision/VisionManager.o \
    $O/modules/vision/VisionMap.o \
    $O/messages/application/CCWSApplPkt_m.o \
    $O/messages/netw/WSMPkt_m.o

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
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

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
	$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

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
	-rm -f modules/control/*_m.cc modules/control/*_m.h
	-rm -f modules/mac/*_m.cc modules/mac/*_m.h
	-rm -f modules/mobility/*_m.cc modules/mobility/*_m.h
	-rm -f modules/netw/*_m.cc modules/netw/*_m.h
	-rm -f modules/nic/*_m.cc modules/nic/*_m.h
	-rm -f modules/phy/*_m.cc modules/phy/*_m.h
	-rm -f modules/utils/*_m.cc modules/utils/*_m.h
	-rm -f modules/vision/*_m.cc modules/vision/*_m.h
	-rm -f networks/*_m.cc networks/*_m.h
	-rm -f networks/highway/*_m.cc networks/highway/*_m.h
	-rm -f networks/linux/*_m.cc networks/linux/*_m.h
	-rm -f networks/london/*_m.cc networks/london/*_m.h
	-rm -f networks/manhattan/*_m.cc networks/manhattan/*_m.h
	-rm -f networks/small/*_m.cc networks/small/*_m.h
	-rm -f networks/windows/*_m.cc networks/windows/*_m.h
	-rm -f sim/*_m.cc sim/*_m.h
	-rm -f sim/bitmaps/*_m.cc sim/bitmaps/*_m.h
	-rm -f sim/results/*_m.cc sim/results/*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc messages/*.cc messages/application/*.cc messages/netw/*.cc modules/*.cc modules/application/*.cc modules/control/*.cc modules/mac/*.cc modules/mobility/*.cc modules/netw/*.cc modules/nic/*.cc modules/phy/*.cc modules/utils/*.cc modules/vision/*.cc networks/*.cc networks/highway/*.cc networks/linux/*.cc networks/london/*.cc networks/manhattan/*.cc networks/small/*.cc networks/windows/*.cc sim/*.cc sim/bitmaps/*.cc sim/results/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/messages/application/CCWSApplPkt_m.o: messages/application/CCWSApplPkt_m.cc \
	$(MIXIM_SOMMER_PROJ)/base/messages/ApplPkt_m.h \
	messages/application/CCWSApplPkt_m.h
$O/messages/netw/WSMPkt_m.o: messages/netw/WSMPkt_m.cc \
	$(MIXIM_SOMMER_PROJ)/base/messages/NetwPkt_m.h \
	messages/netw/WSMPkt_m.h
$O/modules/application/CCWSApplLayer.o: modules/application/CCWSApplLayer.cc \
	$(MIXIM_SOMMER_PROJ)/base/utils/FWMath.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/NetwControlInfo.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/HostState.h \
	modules/application/CCWSApplLayer.h \
	messages/application/CCWSApplPkt_m.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseLayer.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseApplLayer.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/Coord.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/Blackboard.h \
	modules/vision/VisionEntry.h \
	modules/control/WAVEControlInfo.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/FindModule.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseBattery.h \
	modules/utils/MinMax.h \
	modules/vision/VisionMap.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseUtility.h \
	modules/utils/WAVEAddress.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/ImNotifiable.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BatteryAccess.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/PassedMessage.h \
	modules/utils/PositionEstimator.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/Move.h \
	$(MIXIM_SOMMER_PROJ)/base/messages/ApplPkt_m.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseModule.h \
	modules/vision/VisionManager.h
$O/modules/mac/Mac80211p.o: modules/mac/Mac80211p.cc \
	messages/netw/WSMPkt_m.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/FWMath.h \
	$(MIXIM_SOMMER_PROJ)/base/messages/AirFrame_m.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/HostState.h \
	$(MIXIM_SOMMER_PROJ)/base/messages/MacPkt_m.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/MacToPhyInterface.h \
	modules/mac/Mac80211p.h \
	$(MIXIM_SOMMER_PROJ)/modules/phy/Decider80211.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseLayer.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/Coord.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/Blackboard.h \
	$(MIXIM_SOMMER_PROJ)/modules/utility/Consts80211.h \
	$(MIXIM_SOMMER_PROJ)/modules/phy/DeciderResult80211.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/MappingBase.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/AnalogueModel.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/MappingUtils.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/FindModule.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/Mapping.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/PhyToMacControlInfo.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/Signal_.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/DeciderToPhyInterface.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/Decider.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseBattery.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/MacToPhyControlInfo.h \
	$(MIXIM_SOMMER_PROJ)/base/connectionManager/NicEntry.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseWorldUtility.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/PhyUtils.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/ChannelState.h \
	$(MIXIM_SOMMER_PROJ)/base/connectionManager/BaseConnectionManager.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseUtility.h \
	modules/utils/Consts80211p.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/SimpleAddress.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/MacToNetwControlInfo.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseMacLayer.h \
	$(MIXIM_SOMMER_PROJ)/base/messages/NetwPkt_m.h \
	$(MIXIM_SOMMER_PROJ)/modules/messages/Mac80211Pkt_m.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/NetwToMacControlInfo.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BatteryAccess.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/ImNotifiable.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/PassedMessage.h \
	$(MIXIM_SOMMER_PROJ)/base/messages/ChannelSenseRequest_m.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/Move.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseModule.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/BaseDecider.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/Interpolation.h
$O/modules/mobility/TraCIMobilityV.o: modules/mobility/TraCIMobilityV.cc \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseBattery.h \
	$(MIXIM_SOMMER_PROJ)/base/connectionManager/NicEntry.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/FWMath.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseWorldUtility.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/HostState.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseMobility.h \
	$(MIXIM_SOMMER_PROJ)/base/connectionManager/BaseConnectionManager.h \
	$(MIXIM_SOMMER_PROJ)/modules/mobility/traci/TraCIMobility.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseUtility.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/Coord.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/ImNotifiable.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BatteryAccess.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/Blackboard.h \
	modules/mobility/TraCIMobilityV.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/Move.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseModule.h \
	$(MIXIM_SOMMER_PROJ)/modules/mobility/traci/TraCIScenarioManager.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/FindModule.h
$O/modules/netw/WSMNetwLayer.o: modules/netw/WSMNetwLayer.cc \
	messages/netw/WSMPkt_m.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/FWMath.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/HostState.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/NetwControlInfo.h \
	$(MIXIM_SOMMER_PROJ)/base/messages/MacPkt_m.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/MacToPhyInterface.h \
	modules/control/NetwTo80211pControlInfo.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseLayer.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/Coord.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/Blackboard.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseNetwLayer.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/MappingBase.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/MappingUtils.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/AnalogueModel.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/FindModule.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/Mapping.h \
	modules/control/WAVEControlInfo.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/Signal_.h \
	modules/netw/WSMNetwLayer.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseBattery.h \
	$(MIXIM_SOMMER_PROJ)/base/connectionManager/NicEntry.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/PhyUtils.h \
	$(MIXIM_SOMMER_PROJ)/base/connectionManager/BaseConnectionManager.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/ChannelState.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/ArpInterface.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseUtility.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/SimpleAddress.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseMacLayer.h \
	$(MIXIM_SOMMER_PROJ)/base/messages/NetwPkt_m.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/NetwToMacControlInfo.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/ImNotifiable.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BatteryAccess.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/PassedMessage.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/Move.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseModule.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/Interpolation.h
$O/modules/phy/Decider80211p.o: modules/phy/Decider80211p.cc \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/Decider.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/FWMath.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseWorldUtility.h \
	$(MIXIM_SOMMER_PROJ)/base/messages/AirFrame_m.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/HostState.h \
	modules/phy/Decider80211p.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/ChannelState.h \
	$(MIXIM_SOMMER_PROJ)/base/messages/MacPkt_m.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseUtility.h \
	modules/utils/Consts80211p.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/Coord.h \
	$(MIXIM_SOMMER_PROJ)/modules/messages/Mac80211Pkt_m.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/ImNotifiable.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/Blackboard.h \
	$(MIXIM_SOMMER_PROJ)/base/messages/ChannelSenseRequest_m.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/Move.h \
	$(MIXIM_SOMMER_PROJ)/modules/phy/DeciderResult80211.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/MappingBase.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/Interpolation.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/MappingUtils.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/BaseDecider.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/Mapping.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/Signal_.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/DeciderToPhyInterface.h
$O/modules/phy/Phy80211pLayer.o: modules/phy/Phy80211pLayer.cc \
	$(MIXIM_SOMMER_PROJ)/base/utils/FWMath.h \
	$(MIXIM_SOMMER_PROJ)/base/messages/AirFrame_m.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/HostState.h \
	$(MIXIM_SOMMER_PROJ)/base/messages/MacPkt_m.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/MacToPhyInterface.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/Coord.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/ChannelInfo.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/Blackboard.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/MappingBase.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/AnalogueModel.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/MappingUtils.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/FindModule.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/Mapping.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/Signal_.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/DeciderToPhyInterface.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/Decider.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseBattery.h \
	$(MIXIM_SOMMER_PROJ)/base/connectionManager/NicEntry.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseWorldUtility.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/PhyUtils.h \
	modules/phy/Decider80211p.h \
	$(MIXIM_SOMMER_PROJ)/base/connectionManager/BaseConnectionManager.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/ChannelState.h \
	modules/phy/Phy80211pLayer.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseUtility.h \
	modules/utils/Consts80211p.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/ImNotifiable.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BatteryAccess.h \
	$(MIXIM_SOMMER_PROJ)/base/messages/ChannelSenseRequest_m.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/Move.h \
	$(MIXIM_SOMMER_PROJ)/modules/phy/PhyLayer.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/BaseDecider.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/Interpolation.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseModule.h \
	$(MIXIM_SOMMER_PROJ)/base/phyLayer/BasePhyLayer.h \
	$(MIXIM_SOMMER_PROJ)/base/connectionManager/ChannelAccess.h
$O/modules/utils/PositionEstimator.o: modules/utils/PositionEstimator.cc \
	$(MIXIM_SOMMER_PROJ)/base/utils/Coord.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/FWMath.h \
	modules/utils/PositionEstimator.h
$O/modules/vision/VisionManager.o: modules/vision/VisionManager.cc \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseBattery.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/FWMath.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseWorldUtility.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/HostState.h \
	modules/utils/MinMax.h \
	modules/vision/VisionMap.h \
	modules/application/CCWSApplLayer.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseUtility.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseLayer.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/Coord.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseApplLayer.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/ImNotifiable.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BatteryAccess.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/Blackboard.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/PassedMessage.h \
	modules/utils/PositionEstimator.h \
	$(MIXIM_SOMMER_PROJ)/base/modules/BaseModule.h \
	$(MIXIM_SOMMER_PROJ)/base/messages/ApplPkt_m.h \
	modules/vision/VisionEntry.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/FindModule.h \
	modules/vision/VisionManager.h
$O/modules/vision/VisionEntry.o: modules/vision/VisionEntry.cc \
	$(MIXIM_SOMMER_PROJ)/base/utils/FWMath.h \
	$(MIXIM_SOMMER_PROJ)/base/utils/Coord.h \
	modules/utils/MinMax.h \
	modules/vision/VisionMap.h \
	modules/vision/VisionEntry.h
$O/modules/vision/VisionMap.o: modules/vision/VisionMap.cc \
	modules/utils/MinMax.h \
	modules/vision/VisionMap.h

