#*******************************************************************************
# Copyright 2015 Cadence Design Systems, Inc.
# All Rights Reserved.
#
#*******************************************************************************
#
# Stratus Project File
#
############################################################
# Project Parameters
############################################################
#
# Technology Libraries
#
set LIB_PATH "[get_install_path]/share/stratus/techlibs/GPDK045/gsclib045_svt_v4.4/gsclib045/timing"
set LIB_LEAF "slow_vdd1v2_basicCells.lib"
use_tech_lib    "$LIB_PATH/$LIB_LEAF"

#
# Global synthesis attributes.
#
set_attr clock_period           1.0
set_attr message_detail         3 
#set_attr default_input_delay    0.1
#set_attr sched_aggressive_1 on
#set_attr unroll_loops on
#set_attr flatten_arrays all 
#set_attr timing_aggression 0
#set_attr default_protocol true

#
# Simulation Options
#
set_attr cc_options             " -DCLOCK_PERIOD=1.0 -g"
#enable_waveform_logging -vcd
set_attr end_of_sim_command "make saySimPassed"

#
# Testbench or System Level Modules
#
define_system_module ../main.cpp
define_system_module ../Testbench.cpp

#
# SC_MODULEs to be synthesized
#
define_hls_module SobelFilter ../SobelFilter.cpp

define_hls_config SobelFilter BASIC
define_hls_config SobelFilter DPA       --dpopt_auto=op,expr

set IMAGE_DIR           ".."
set IN_FILE_NAME        "${IMAGE_DIR}/lena_std_short.bmp"
set OUT_FILE_NAME				"out.bmp"

define_sim_config B -argv "$IN_FILE_NAME $OUT_FILE_NAME"
foreach cfg { BASIC DPA } {
	define_sim_config V_${cfg} "SobelFilter RTL_V ${cfg}" -argv "$IN_FILE_NAME $OUT_FILE_NAME"
}
