/******************************************************
 * Author        : feng
 * Creation time : 2019-05-17 09:32
 * Update time   : 2019-05-27 18:53
 * Description   :
 *	1.	target中定义fw函数，该函数由initiator(即router)调用
******************************************************/
#ifndef __TARGET_H__
#define __TARGET_H__

// Needed for the simple_target_socket
#include "common.h"


class Target
	: public sc_core::sc_module
{
protected:

	SC_HAS_PROCESS(Target);

public:
	// TLM-2 socket, defaults to 32-bits wide, base protocol
	tlm_utils::simple_target_socket<Target> targ_socket;

	enum { SIZE = 256 };
	const sc_time LATENCY;

	Target(sc_core::sc_module_name name)
	{
		// Register callbacks for incoming interface method calls
		targ_socket.register_nb_transport_fw( this, &Target::nb_transport_fw);

		// Initialize memory with random data
		for (int i = 0; i < SIZE; i++)
			mem[i] = 0xAA000000 | (mem_nr << 20) | (rand() % 256);

		++mem_nr;
	}

	~Target() {}

	// TLM-2 blocking transport method
	virtual tlm_sync_enum nb_transport_fw (	tlm_generic_payload& trans, 
											tlm_phase& phase, 
											sc_time& delay )
	{
		tlm_command		cmd = trans.get_command();
		sc_dt::uint64   adr = trans.get_address() / 4;
		unsigned char*  ptr = trans.get_data_ptr();
		unsigned int    len = trans.get_data_length();
		unsigned char*  byt = trans.get_byte_enable_ptr();
		unsigned int    wid = trans.get_streaming_width();

		// Obliged to check address range and check for unsupported features,
		//   i.e. byte enables, streaming, and bursts
		// Can ignore extensions

		// Generate the appropriate error response
		if (adr >= SIZE) {
			trans.set_response_status( TLM_ADDRESS_ERROR_RESPONSE );
			 return TLM_ACCEPTED;
		}
		if (byt != 0) {
			trans.set_response_status( TLM_BYTE_ENABLE_ERROR_RESPONSE );
			return TLM_ACCEPTED;
		}
		if (len > 4 || wid < len) {
			trans.set_response_status( TLM_BURST_ERROR_RESPONSE );
			return TLM_ACCEPTED;
		}

		wait(delay);
		delay = SC_ZERO_TIME;

		// Obliged to implement read and write commands
		if ( cmd == TLM_READ_COMMAND )
			memcpy(ptr, &mem[adr], len);
		else if ( cmd == TLM_WRITE_COMMAND )
			memcpy(&mem[adr], ptr, len);

		// Obliged to set response status to indicate successful completion
		trans.set_response_status( TLM_OK_RESPONSE );

		// 输出模块名
		cout << "Targ=" << this->name() << ", ";
	}

	int mem[SIZE];
	static unsigned int mem_nr;
};

unsigned int Target::mem_nr = 0;

#endif