#ifndef REG_FILE_H_
#define REG_FILE_H_

/*
	define register file, contain :
	1.prim_register class define;
	2.insn_register class define;
	3.task_list_entry define;
	4.whole reg_file class define;
*/

#include "dmac_global.h"

extern ostream & operator << (ostream& os, const reg_name rn);

extern ostream & operator << (ostream& os, const CH_NAME rn);

extern ostream & operator << (ostream& os, const task_list_type tlt);

class reg_file: public sc_module
{
public:
	//sc_in_clk clk;

	SC_HAS_PROCESS(reg_file);
	reg_file(sc_module_name mn, unsigned int CORE_ID);
	~reg_file(){}


	//void clock_tick();

//read regs by reg_name, for master/slave side
	//unsigned int read_reg(reg_name rn, CH_NAME ch_id);

//read regs by reg_addr, for core side
	bool read_reg(unsigned int addr, unsigned int & result, bool JtagFlag = false);				

//write regs by reg_name, for master/slave side
	//bool write_reg(reg_name rn,unsigned int val,unsigned int ch_id);
	
//write regs by reg_addr, for core side
	bool write_reg(unsigned int addr,unsigned int val ,bool JtagFlag = false);		


    void dma_wr_mission_start(unsigned int CH_ID);
	void dma_rd_mission_start(unsigned int CH_ID);
	void dma_wr_mission_reset(unsigned int CH_ID);
	void dma_rd_mission_reset(unsigned int CH_ID);

//read insn_regs by reg_name, for master/slave side
	//MSG_INSN_RW_MEM	read_insn_reg(reg_name rn, unsigned int ch_id);

//write insn_regs by reg_name, for master/slave side
	//void write_insn_reg(reg_name rn, MSG_INSN_RW_MEM& val,unsigned int ch_id);


/*
add mission to TASK_LIST, can be all four task type,
when it is triggered by regs from core/slave write 
or by DMAC
*/
	bool mission_add(unsigned int ch_id, task_list_type mission_type, bool wait_data_back);


/*
clear mission in TASK_LIST, can be all four task type when it is end;
end means get resp ok or get resp stop from slave
or when cancel dma get the transfer end accept
*/
	//bool mission_end(unsigned int ch_id, task_list_type mission_type);

	bool mission_start(unsigned int ch_id, task_list_type mission_type);

	bool mission_stall(unsigned int ch_id, task_list_type mission_type);

	bool mission_restore(unsigned int ch_id, task_list_type mission_type);

	bool mission_cancel(unsigned int ch_id, task_list_type mission_type);

	check_mission_return mission_check(unsigned int ch_id, task_list_type mission_type);

	
//be called at the same time of setting regs for start or cancel a dma task
	void mission_start_cancel_watch_reg(unsigned int ch_id, reg_behavior_type behavior, bool wait_data_back);	   //process to resolve the start regs change

	bool wait_data_back_check(unsigned int ch_id, task_list_type mission_type);					//return TASK_LIST[ch_id][mission_type].wait_ack_old
	
	//void clear_regs_after_resp_ok(task_list_type MISSION_TYPE, unsigned int ch_id);		//be called by void get_resp_ok_process(); for set the regs of different task type to initialization value 




//-----------------------------------------------------------reg file data store elem
	RegType GLB_STATUS_reg;						//ofst_addr = 1
	
	RegType HCH_START_R;						// ofst_addr =  32 + 0x00
	RegType HCH_STATUS_R;						// ofst_addr =  32 + 0x01
	RegType HCH_CTL_R;						// ofst_addr =  32 + 0x02
	RegType HCH_DST_START_ADDRH_R;				// ofst_addr =  32 + 0x0c //add 2015.1.12
	RegType HCH_SRC_START_ADDR_R;				// ofst_addr =  32 + 0x10
	RegType HCH_SRC_LEN_X_R;					// ofst_addr =  32 + 0x11
	RegType HCH_SRC_ADDR_STEP_R;				// ofst_addr =  32 + 0x12
	RegType HCH_SRC_LEN_Y_R;					// ofst_addr =  32 + 0x13
	RegType HCH_SRC_CURR_ADDR_R;				// ofst_addr =  32 + 0x14
	RegType HCH_SRC_LEN_REM_X_R;				// ofst_addr =  32 + 0x15
	RegType HCH_SRC_LEN_REM_Y_R;				// ofst_addr =  32 + 0x16
	RegType HCH_DST_START_ADDRL_R;				// ofst_addr =  32 + 0x18 //mod 2015.1.12
	RegType HCH_DST_LEN_X_R;					// ofst_addr =  32 + 0x19
	RegType HCH_DST_ADDR_STEP_R;				// ofst_addr =  32 + 0x1a
	RegType HCH_DST_LEN_Y_R;					// ofst_addr =  32 + 0x1b
	RegType HCH_DST_CURR_ADDR_R;				// ofst_addr =  32 + 0x1c
	RegType HCH_DST_LEN_REM_X_R;				// ofst_addr =  32 + 0x1d
	RegType HCH_DST_LEN_REM_Y_R;				// ofst_addr =  32 + 0x1e


	RegType XWCHi_START_R[CHANNEL_NUM];					// ofst_addr =  (1+i)*64 + 0x00
	RegType XWCHi_STATUS_R[CHANNEL_NUM];					// ofst_addr =	(1+i)*64 + 0x01
	RegType XWCHi_CTL_R[CHANNEL_NUM];						// ofst_addr =	(1+i)*64 + 0x02
	RegType XWCHi_DST_START_ADDRH_R[CHANNEL_NUM];				// ofst_addr =  (1+i)*64 + 0x0c //add 2015.1.12
	RegType XWCHi_SRC_START_ADDR_R[CHANNEL_NUM];			// ofst_addr =  (1+i)*64 + 0x10
	RegType XWCHi_SRC_LEN_X_R[CHANNEL_NUM];				// ofst_addr =  (1+i)*64 + 0x11
	RegType XWCHi_SRC_ADDR_STEP_R[CHANNEL_NUM];			// ofst_addr =  (1+i)*64 + 0x12
	RegType XWCHi_SRC_LEN_Y_R[CHANNEL_NUM];				// ofst_addr =  (1+i)*64 + 0x13
	RegType XWCHi_SRC_CURR_ADDR_R[CHANNEL_NUM];			// ofst_addr =  (1+i)*64 + 0x14
	RegType XWCHi_SRC_LEN_REM_X_R[CHANNEL_NUM];			// ofst_addr =  (1+i)*64 + 0x15
	RegType XWCHi_SRC_LEN_REM_Y_R[CHANNEL_NUM];			// ofst_addr =  (1+i)*64 + 0x16
	RegType XWCHi_DST_START_ADDRL_R[CHANNEL_NUM];				// ofst_addr =  (1+i)*64 + 0x18//mod 2015.1.12
	RegType XWCHi_DST_LEN_X_R[CHANNEL_NUM];				// ofst_addr =  (1+i)*64 + 0x19
	RegType XWCHi_DST_ADDR_STEP_R[CHANNEL_NUM];			// ofst_addr =  (1+i)*64 + 0x1a
	RegType XWCHi_DST_LEN_Y_R[CHANNEL_NUM];				// ofst_addr =  (1+i)*64 + 0x1b
	RegType XWCHi_DST_CURR_ADDR_R[CHANNEL_NUM];			// ofst_addr =  (1+i)*64 + 0x1c
	RegType XWCHi_DST_LEN_REM_X_R[CHANNEL_NUM];			// ofst_addr =  (1+i)*64 + 0x1d
	RegType XWCHi_DST_LEN_REM_Y_R[CHANNEL_NUM];			// ofst_addr =  (1+i)*64 + 0x1e

	RegType XRCHi_START_R[CHANNEL_NUM];					// ofst_addr =  (1+i)*64 + 0x20
	RegType XRCHi_STATUS_R[CHANNEL_NUM];					// ofst_addr =  (1+i)*64 + 0x21
	RegType XRCHi_CTL_R[CHANNEL_NUM];						// ofst_addr =  (1+i)*64 + 0x22
	RegType XRCHi_DST_START_ADDRH_R[CHANNEL_NUM];				// ofst_addr =  (1+i)*64 + 0x2c //add 2015.1.12
	RegType XRCHi_SRC_START_ADDR_R[CHANNEL_NUM];			// ofst_addr =  (1+i)*64 + 0x30
	RegType XRCHi_SRC_LEN_X_R[CHANNEL_NUM];				// ofst_addr =  (1+i)*64 + 0x31
	RegType XRCHi_SRC_ADDR_STEP_R[CHANNEL_NUM];			// ofst_addr =  (1+i)*64 + 0x32
	RegType XRCHi_SRC_LEN_Y_R[CHANNEL_NUM];				// ofst_addr =  (1+i)*64 + 0x33
	RegType XRCHi_SRC_ADDR_CURR_R[CHANNEL_NUM];			// ofst_addr =  (1+i)*64 + 0x34
	RegType XRCHi_SRC_LEN_REM_X_R[CHANNEL_NUM];			// ofst_addr =  (1+i)*64 + 0x35
	RegType XRCHi_SRC_LEN_REM_Y_R[CHANNEL_NUM];			// ofst_addr =  (1+i)*64 + 0x36
	RegType XRCHi_DST_START_ADDRL_R[CHANNEL_NUM];				// ofst_addr =  (1+i)*64 + 0x38 //mod 2015.1.12
	RegType XRCHi_DST_LEN_X_R[CHANNEL_NUM];				// ofst_addr =  (1+i)*64 + 0x39
	RegType XRCHi_DST_ADDR_STEP_R[CHANNEL_NUM];			// ofst_addr =  (1+i)*64 + 0x3a
	RegType XRCHi_DST_LEN_Y_R[CHANNEL_NUM];				// ofst_addr =  (1+i)*64 + 0x3b
	RegType XRCHi_DST_CURR_ADDR_R[CHANNEL_NUM];			// ofst_addr =  (1+i)*64 + 0x3c
	RegType XRCHi_DST_LEN_REM_X_R[CHANNEL_NUM];			// ofst_addr =  (1+i)*64 + 0x3d
	RegType XRCHi_DST_LEN_REM_Y_R[CHANNEL_NUM];			// ofst_addr =  (1+i)*64 + 0x3e
	
	//	vector<DMAC_RW_REG_S>  ShadowRegVec;

//zqw+ 2019.1.11 SRIO��ؼĴ���
	//дDMA����
	RegType RAB_WDMA_N_CTRL_R[SRIO_NUM];			// дDMA���ƼĴ���  0x20800
	RegType RAB_WDMA_N_ADDR_R[SRIO_NUM];            // дDMA��������ַ�Ĵ��� 0x20804
	RegType RAB_WDMA_N_STAT_R[SRIO_NUM];            // дDMA״̬�Ĵ��� 0x20808
	RegType RAB_WDMA_N_ADDR_EXT_R[SRIO_NUM];        // дDMA��λ��ַ�Ĵ��� 0x2080C
	//��DMA����
	RegType RAB_RDMA_N_CTRL_R[SRIO_NUM];			// ��DMA���ƼĴ���  0x20880
	RegType RAB_RDMA_N_ADDR_R[SRIO_NUM];            // ��DMA��������ַ�Ĵ��� 0x20884
	RegType RAB_RDMA_N_STAT_R[SRIO_NUM];            // ��DMA״̬�Ĵ��� 0x20888
	RegType RAB_RDMA_N_ADDR_EXT_R[SRIO_NUM];        // ��DMA��λ��ַ�Ĵ��� 0x2088C
	//DMA������
	RegType RAB_DMA_IADDR_DESC_SEL_R[SRIO_NUM];	        // DMA������ѡ��Ĵ���  0x20900
	RegType RAB_DMA_IADDR_DESC_CTRL_R[SRIO_NUM];	    // DMA�������Ĵ���-���������ƼĴ���  0x20904
	RegType RAB_DMA_IADDR_DESC_SRC_ADDR_R[SRIO_NUM];    // DMA�������Ĵ���-Դ��ַ�Ĵ���      0x20908
	RegType RAB_DMA_IADDR_DESC_DEST_ADDR_R[SRIO_NUM];   // DMA�������Ĵ���-Ŀ�ĵ�ַ�Ĵ���    0x2090C
	RegType RAB_DMA_IADDR_DESC_NEXT_ADDR_R[SRIO_NUM];   // DMA�������Ĵ���-��һָ���ַ�Ĵ���  0x209010
	
	//zqw+ 2019.2.25 ���豸ID�Ĵ�����ҳ��
	RegType Base_Device_ID_CSR_R[SRIO_NUM];             //���豸ID�Ĵ��������ڴ��Source ID
	RegType RAB_APB_CSR_R[SRIO_NUM];                    //RAB_APB_CSR Register��APB����״̬�Ĵ���  0x20030  ���ڴ��ҳ��


	task_list_entry TASK_LIST[CHANNEL_NUM][TASK_TYPE_NUM];		//task_list_entry, meet evaluate_update attribute, can be multi_driver

	prim_register  DMA_WR_MISSION_START[CHANNEL_NUM]; //1: this dmac's ch0's dma wr mission is on;  0: off
	prim_register  DMA_RD_MISSION_START[CHANNEL_NUM]; //1: this dmac's ch0's dma rd mission is on;  0: off
	
private:
	unsigned int core_id;
	//zqw+ 2019.3.11 ҳ��
	unsigned int page_number;

};



#endif