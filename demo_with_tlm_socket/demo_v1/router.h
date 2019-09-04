/******************************************************
 * Author        : feng
 * Creation time : 2019-05-16 23:09
 * Update time   : 2019-05-16 23:09
 * Description   : 
 *	1.	本例中的router只有1个target接口，即只能连接到
 *		1个initiator，需要改进
 *	2.	本例中的router有N_TARGETS个init接口，对应到
 *		target数量
 *	3.	router需要实现fw方法和bw方法，其中fw方发供
 *		initiator使用，用于从initiator发送到router；
 *		bw方法供target使用，用于从target往initiator
 *		传送请求的内容
******************************************************/

#ifndef __ROUTER_H__
#define __ROUTER_H__

#include "systemc"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;
using namespace tlm_utils;
using namespace tlm;

// *********************************************
// Generic payload non-blocking transport router
// *********************************************

template<unsigned int N_TARGETS>
//template<unsigned int N_INIT>

SC_MODULE(Router)
{
	// TLM-2 socket, defaults to 32-bits wide, base protocol
	// 此处改为使用tagged以及数组
	simple_target_socket<Router>		targ_socket;

	// *********************************************
	// Use tagged sockets to be able to distinguish incoming backward path calls
	// *********************************************

	simple_initiator_socket_tagged<Router>*	init_socket[N_TARGETS];

	SC_CTOR(Router)
	{
		// Register callbacks for incoming interface method calls
		targ_socket.register_nb_transport_fw( this, &Router::nb_transport_fw);

		for (unsigned int i = 0; i < N_TARGETS; i++)
		{
			char txt[30];
			sprintf(txt, "init_socket_%d", i);
			init_socket[i] = new simple_initiator_socket_tagged<Router>(txt);

			// *********************************************
			// Register callbacks for incoming interface method calls, including tags
			// *********************************************
			init_socket[i]->register_nb_transport_bw(this, &Router::nb_transport_bw, i);
		}
	}

	// ****************
	// FORWARD PATH
	// ****************

	// TLM-2 non blocking transport_fw method
	virtual tlm_sync_enum nb_transport_fw( tlm_generic_payload& trans, 
										   tlm_phase &phase, sc_time& delay )
	{
		uint64 address = trans.get_address();
		uint64 masked_address;
		unsigned int target_nr = decode_address( address, masked_address);

		// Modify address within transaction
		trans.set_address( masked_address );

		// Forward transaction to appropriate target
		( *init_socket[target_nr] )->nb_transport_fw( trans, phase, delay );
		return tlm::TLM_ACCEPTED;
	}
  
	// ****************
	// BACKWARD PATH
	// ****************
	virtual tlm_sync_enum nb_transport_bw(	int id, 
											tlm_generic_payload& trans, 
											tlm_phase &phase, sc_time& delay )
	{
		// Backward transaction to initiator
		targ_socket->nb_transport_bw( trans, phase, delay );
		return tlm::TLM_ACCEPTED;
	}

	// ****************
	// ROUTER INTERNALS
	// ****************

	// Simple fixed address decoding
	inline unsigned int decode_address(uint64 address, uint64& masked_address )
	{
		unsigned int target_nr = static_cast<unsigned int>( (address >> 8) & 0x3 );
		masked_address = address & 0xFF;
		return target_nr;
	}

	inline uint64 compose_address( unsigned int target_nr, uint64 address)
	{
		return (target_nr << 8) | (address & 0xFF);
	}
};

#endif