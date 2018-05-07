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
#set LIB_PATH "[get_install_path]/share/stratus/techlibs/GPDK045/gsclib045_svt_v4.4/gsclib045/timing"
#set LIB_PATH "/theda21_2/CBDK_IC_Contest/cur/SynopsysDC/lib"
set LIB_PATH "/theda21_2/library/CBDK40_TSMC/CBDK_TSMC40_Arm_f2.0/CIC/SynopsysDC/lib/sc9_base_rvt"
set LIB_LEAF "sc9_cln40g_base_rvt_ss_typical_max_0p81v_m40c.lib"
set LIB_PATH_DC "/theda21_2/library/CBDK40_TSMC/CBDK_TSMC40_Arm_f2.0/CIC/SynopsysDC/db/sc9_base_rvt"
set LIB_LEAF_DC "sc9_cln40g_base_rvt_ss_typical_max_0p81v_m40c.db"

#use_tech_lib	"$LIB_PATH_DC/$LIB_LEAF_DC"
use_tech_lib	"$LIB_PATH/$LIB_LEAF"

#
# Global synthesis attributes.
#
set_attr clock_period           6.0
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
set_attr cc_options             " -DCLOCK_PERIOD=6.0 -g"
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
define_hls_module SobelFilter SobelFilter.cpp

define_hls_config SobelFilter BASIC
define_hls_config SobelFilter DPA       --dpopt_auto=op,expr

define_sim_config B 

######################################################################
# Simulation and Power Configurations
######################################################################
# The following rules are TCL code to create a simulation configuration
# for RTL_V for each hls_config defined.
# For each simulation configuration, create a JOULES power analysis
# configuration as well. NOTE: You must have waveform logging enabled
# for the power configurations to work.
#
foreach config [find -hls_config *] {
  set cname [get_attr name $config]
  define_sim_config ${cname}_V "SobelFilter RTL_V $cname"
  define_power_config P_${cname} ${cname}_V -module SobelFilter -command bdw_runjoules
}

######################################################################
# RC Logic Synthesis Configurations
######################################################################
define_logic_synthesis_config RC {SobelFilter -all} \
    -command "bdw_runrc " \
    -options \
        [list BDW_LS_TECHLIB "$LIB_PATH/$LIB_LEAF"] \
        {BDW_LS_CLK_GATING normal} \
        {BDW_LS_NOGATES 1} \
		{BDW_LS_CLK_PERIOD 6.0}

define_logic_synthesis_config DC {SobelFilter -all} \
    -command "dc_shell -f [get_install_path]/share/stratus/tcl/bdw_rundc.tcl > DC_log.txt" \
    -options \
        [list BDW_LS_TECHLIB "$LIB_PATH_DC/$LIB_LEAF_DC"] \
        {BDW_LS_CLK_GATING normal} \
        {BDW_LS_NOGATES 1} \
		{BDW_LS_CLK_PERIOD 6.6}
