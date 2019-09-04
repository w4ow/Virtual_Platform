/******************************************************************************
 * Author        : feng
 * Creation time : 2019-05-27 09:51
 * Update time   : 2019-06-12 11:14
 * Description   : 
******************************************************************************/

#ifndef __COMMON_H__
#define __COMMON_H__

#define SC_INCLUDE_DYNAMIC_PROCESSES

#include <systemc>
#include <queue>
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/passthrough_target_socket.h"
#include "tlm_utils/multi_passthrough_initiator_socket.h"
#include "tlm_utils/multi_passthrough_target_socket.h"
#include "tlm_utils/peq_with_cb_and_phase.h"
#include "tlm_utils/instance_specific_extensions.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;
using namespace tlm;
using namespace tlm_utils;

#define band_width 32
#define protocol tlm::tlm_base_protocol_types
// #define DEBUG_MODULE 0

const unsigned int N_INIT = 2;
const unsigned int N_TARG = 2;

#endif