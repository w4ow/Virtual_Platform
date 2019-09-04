/*************************************************
  File name:      iss_dmac_comm.h

  Author:
  Version:                 
  Date: 

  Description:    BWDSP104x�ں���DMAC���TLMͨ�Žӿڶ��塣

  Others:         // �������ݵ�˵��
  
  Function List:  // ��Ҫ�����б���ÿ����¼Ӧ���������������ܼ�Ҫ˵��
    1. ....
    
  History:

    1. Date:	2013-3-21
       Author:	GRui
       Modification: ɾ����CMD_RW_DMAC_REGS_ACK��CMD_INSN_RW_OUT_MEM_ACK��
	                 CMD_INSN_RW_CORE_MEM_ACK��CMD_BURST_WR_CORE_MEM_ACK��
					 CMD_BURST_RD_CORE_MEM_ACK����塣��Ӧ����Ϣ����ʹ��
					 phase������Ӧ��������Ϣ�������֡�

	2. Date:	2013-5-25
       Author:	GRui
       Modification: 1. ʹ��generic_payload.m_address �洢������
                  2. �޸���CMD_BURST_WR_CORE_MEM (CORE --> DMAC)�е�������Ϣ
				  3. ��CMD_RW_DMAC_REGS�滻ΪCMD_RD_DMAC_REG��CMD_WR_DMAC_REG��

    3. Date:	2013-6-5
       Author:	GRui
       Modification: 1. ��MSG_INSN_RW_MEM_ELEM�е�u32Slot�޸�Ϊu32Bus.

	4. Date:	2013-7-2
       Author:	GRui
       Modification: 1. ��MSG_INSN_RW_MEM��������arrBusInfo[4]�����

	5. Date:	2013-7-3
       Author:	GRui
       Modification: 1. ��MSG_INSN_RW_MEM_BUS�����������������
       				 ������Ϊmaster���ں�ʹ��, DMAC���账�����ṩ��
       					DSPU32 u32SlotPlus1;    // ���䵽�����ߵ�ָ����ָ�����е�slot���(1 ~ 16)	
						bool bolSingleWord;      // �Ƿ��ǵ��ַô�ָ��
						bool bolIsDM8;          // �Ƿ���˫ģ8ָ��  

	4. Date:	2013-7-11
       Author:	GRui
       Modification: 1. ��MSG_INSN_RW_MEM_ELEM��������u32AddrIdx�����

    5. Date:	2014-4-9
       Author:	GRui
       Modification: 1. ɾ��CMD_INSN_RW_CORE_MEM��Ϣ����(��������ָ�ʽ��/д�����ڲ��洢�ռ�)



*************************************************/
#ifndef SC_DMAC_H_
#define SC_DMAC_H_

#ifdef DSP_WIN32
#include ".\src\AlgoriLib\dsptypes.h"
#else
#include "./src/AlgoriLib/dsptypes.h"
#endif

enum DMAC_CHANNEL
{
	HSCH0_RW,
	XMCH0_RD,
	XMCH0_WR,
	XMCH2_RD,
	XMCH2_WR,
	XMCH3_RD,
	XMCH3_WR,
	XSCH0_RD,
	XSCH0_WR,
	XSCH2_RD,
	XSCH2_WR,
	XSCH3_RD,
	XSCH3_WR,
	XSCH4_RD,
	XSCH4_WR,
	XSCH5_RD,
	XSCH5_WR,
	XSCH6_RD,
	XSCH6_WR,
	XSCH7_RD,
	XSCH7_WR,
	HMCH0_RW,
	DMAC_MAX_CHANNELS =200
};

//zqw+ 2019.1.21  SRIO�ź�
enum SRIO_CHANNEL
{
	SRIOMCH0_RD = 100,
	SRIOMCH0_WR,
	SRIOSCH0_RD,
	SRIOSCH0_WR,
	SRIOMCH1_RD,
	SRIOMCH1_WR,
	SRIOSCH1_RD,
	SRIOSCH1_WR,
	SRIO_MAX_CHANNELS
};

/************************************ Definition ************************************************/
enum DMAC_CMD
{
	CORE_REQ_RD_REG,			// ��DMAC���ƼĴ��������DMA״̬
	CORE_REQ_WR_REG,			// дDMAC���ƼĴ���������DMA����
	JTAG_REQ_RD_REG,			// JTAG��DMA���ƼĴ���
	JTAG_REQ_WR_REG,			// JTAGдDMA���ƼĴ���
	CORE_REQ_OUT_MEM,		// CORE�����ָ�ʽ��д����洢��(�Ǻ�����ICMA��Χ)
	CORE_RESET_DMAC,			// CORE�����DMAC reset����
	DMAC_REQ_CORE_MEM,		// DMAC����ķ��ʺ��ڴ洢������
	DMAC_REQ_INT,
	DMAC_WR_REQ, //add by wangqiang for AXI 
	DMAC_WR_ACK,//add by wangqiang for AXI 
	DMAC_RD_REQ,//add by wangqiang for AXI 
	DMAC_RD_ACK,//add by wangqiang for AXI 
	DMAC_ADDR_ERR,//add by wangqiang for AXI 
	DMAC_CMD_MAX,

	SYS_CFG_MSG,		//add by wangqiang for SystemConfigReg trans Msg to Core
	DMAC_RD_REQ_DDR		//read have 2 ways, 1 is DMAC_RD_REQ 2 is only 8 words read, but write have only one way
};

typedef struct
{
	DSPU32 u32INT;
} DMAC_INT_S;

typedef struct
{
	DSPU32 u32SrcEvt;
	bool bolPosEdge;
} SRC_EVT_S;
//-------------------------------------------
// ����DMAC���ƼĴ���
//-------------------------------------------
// (CORE --> DMAC)
//		CORE_REQ_RD_REG
//		CORE_REQ_WR_REG
// ������Ϣ��
// generic_payload.m_address: CORE_REQ_RD_REG/CORE_REQ_WR_REG
// generic_payload.m_data: ָ��DMAC_RW_REG_S�ṹ���ָ��
// generic_payload.m_length: sizeof(DMAC_RW_REG_S)
// phase: BEGIN_REQ
//
// ������Ϣ��
// generic_payload.m_response_status��
//     ���󱻽��ܣ�TLM_OK_RESPONSE��
//     �����������TLM_GENERIC_ERROR_RESPONSE��
// phase: END_RESP
// 
// ��������ֵ��TLM_COMPLETED/TLM_UPDATED
//
typedef struct
{
	DSPU32 u32Addr;
	DSPU32 u32Value;
} DMAC_RW_REG_S;

//-------------------------------------------
// �ں���DMAC֮��ô�ӿ�
//-------------------------------------------
// ������Ϣ��
// generic_payload.m_address: 
//	(CORE --> DMAC)
//		CORE_REQ_OUT_MEM
//		CORE_RESP_DMAC
//	(DMAC --> CORE)
//		DMAC_REQ_CORE_MEM
//		DMAC_RESP_CORE
// generic_payload.m_data: ָ��DMAC_REQ_MEM�ṹ���ָ��
// generic_payload.m_length: sizeof(DMAC_REQ_MEM)
// phase: BEGIN_REQ/BEGIN_RESP
//
// ������Ϣ��
// generic_payload.m_response_status��
//     ���󱻽��ܣ�TLM_OK_RESPONSE��
//     �����������TLM_GENERIC_ERROR_RESPONSE��
// phase: END_RESP/END_REQ
// 
// ��������ֵ��TLM_COMPLETED/TLM_UPDATED/TLM_ACCEPTED
//
typedef struct
{
	bool bolIsRead;
	DSPU32 u32Addr;
	DSPU32 u32Value;
	DSPU32 u32Bus;
	DSPU32 u32WMask;
	DSPU32 u32AddrIdx;
} DMAC_ELEM;

typedef struct 
{
	DSPU32 u32ElemNum; 
	DSPU32 u32ChanIdx;
	DMAC_ELEM arrRWInfo[64];
	bool bolAllWrReq;
} DMAC_MEM;

//zqw+ 2019.3.6 SRIO֡��ʽ
typedef struct 
{
	DSPU32 TT; 
	DSPU32 Ftype;
	DSPU32 TargetID;
	DSPU32 SourceID; 
	DSPU32 Ttype; 
	DSPU32 Extension; 
	DSPU32 TID; 
	DSPU32 OffsetAddress;
	DSPU32 Wdptr; 
	DSPU32 Addr; 
	DSPU32 Data[64];
} SRIO_FRAME;

#endif


#if 0

//------------------------------------------------
enum tlm_core_dmac_command 
{
	// �ں˶�DMAC�����Ĵ���
    CMD_RD_DMAC_REG,

	// �ں�дDMAC�����Ĵ���
    CMD_WR_DMAC_REG,

	// �ں���ָ�ʽ��/д����洢�ռ�
	// �ں�ͨ��AHB���߶�/д����Ĵ���Ҳ���ø÷�ʽ
	CMD_INSN_RW_OUT_MEM,

	// DMAC�ڵ�DMA��������burst��ʽд���ڴ洢��
    CMD_BURST_WR_CORE_MEM,

	// DMAC�ڵ�DMA��������burst��ʽ�����ڴ洢��
	CMD_BURST_RD_CORE_MEM,

	// �ں˸�λDMAC
	CMD_RESET_DMAC,

	CMD_RESERVED
};

//------------------------------------------------
// �����ں˷���DMAC�Ĵ�����˵��
//
// �ں�ÿ���ڿɷ���DMAC�ļĴ���������
//     12�������д����8������ͨ�üĴ�����4����������������ÿ��32λ����
//     4������Ķ�����ÿ��32λ����
// ��ʱ��
//     ����д�Ĵ��������������ڼ������
//     ���ڶ��Ĵ������������ݱ����̶������Ĵ�2 ���󷵻ظ��ںˡ�
//     �����ܻᱻ���ܣ���ͨ����æ��д�����������Ч���������ᴥ���쳣�ȡ�
// ��๤������֤ͬһʱ�̶��д���󲻻�дͬһ���Ĵ�����
//

//------------------------------------------------
// CMD_RD_DMAC_REG (CORE --> DMAC)
//
// ��ʱ��
//     DMAC�Ĵ����൱��AC/EX��ˮ�߼Ĵ�����
//     �ں���AC������Ķ���д������������Ч������������ʱ��
//
// ����: nb_transport_fw() bind with tsocket of DMAC
//
// ������Ϣ��
// generic_payload.m_address: CMD_RD_DMAC_REG
// generic_payload.m_data: ָ��MSG_RW_DMAC_REG�ṹ���ָ��
// generic_payload.m_length: sizeof(MSG_RW_DMAC_REG)
// phase: BEGIN_REQ
//
// ������Ϣ��
// generic_payload.m_data: ָ���MSG_RW_DMAC_REG�ṹ���д�Ŷ������ݡ�
// generic_payload.m_response_status��
//     ���󱻽��ܣ�TLM_OK_RESPONSE��
//     �����������TLM_GENERIC_ERROR_RESPONSE��
// phase: END_RESP
// 
// ��������ֵ��TLM_COMPLETED
//
//

//------------------------------------------------
// CMD_WR_DMAC_REG (CORE --> DMAC)
//
// ��ʱ��
//     DMAC�Ĵ����൱��AC/EX��ˮ�߼Ĵ�����
//     �ں���AC������Ķ���д������������Ч������������ʱ��
//
// ����: nb_transport_fw() bind with tsocket of DMAC
//
// ������Ϣ��
// generic_payload.m_address: CMD_WR_DMAC_REG
// generic_payload.m_data: ָ��MSG_RW_DMAC_REG�ṹ���ָ��
// generic_payload.m_length: sizeof(MSG_RW_DMAC_REG)
// phase: BEGIN_REQ
//
// ������Ϣ��
// generic_payload.m_response_status��
//     ���󱻽��ܣ�TLM_OK_RESPONSE��
//     �����������TLM_GENERIC_ERROR_RESPONSE��
// phase: END_RESP
// 
// ��������ֵ��TLM_COMPLETED
//
//
typedef struct msg_rw_dmac_reg
{
	DSPU32 u32Addr;   // [addr] 12λ��ַ
	DSPU32 u32Value;  // �Ĵ���ֵ

} MSG_RW_DMAC_REG;

//------------------------------------------------
// CMD_BURST_WR_CORE_MEM (CORE <-- DMAC)
//
// burst��ʽ�£�DMAC��CORE�����дλ�����Ϊ256λ��
//
// ����: nb_transport_fw() bind with tsocket of CORE
//
// ������Ϣ��
// generic_payload.m_address: CMD_BURST_WR_CORE_MEM
// generic_payload.m_data: ָ��MSG_RW_CORE_MEM�ṹ���ָ��
// generic_payload.m_length: sizeof(MSG_RW_CORE_MEM)
// phase: BEGIN_REQ
//
// ������Ϣ��
// generic_payload.m_response_status��
//     ���󱻽��ܣ�TLM_OK_RESPONSE��
//     ����δ�����ܣ��豸æ����TLM_COMMAND_ERROR_RESPONSE
//     �����������TLM_GENERIC_ERROR_RESPONSE��
// phase: END_REQ
// 
// ��������ֵ��TLM_UPDATED
//
typedef struct msg_rw_core_mem_elem
{
	DSPU32 u32Addr;  // ��ַ
	DSPU32 u32Value; // ����

} MSG_RW_CORE_MEM_ELEM;

typedef struct msg_rw_core_mem
{
	DSPU32 u32ID;      // ��ϢID
	DSPU32 u32ElemNum; // �ô��ַ��Ԫ���������Ϊ��������ô�Ľ��ж���С��
	DSPU32 u32ChanIdx;  // burst��ʽ�ж��ͨ������������ͨ����0 ~ 10
	MSG_RW_CORE_MEM_ELEM arrRWElems[8];

} MSG_RW_CORE_MEM;

//------------------------------------------------
// CMD_BURST_WR_CORE_MEM (CORE --> DMAC)
//
// burst��ʽ�£�DMAC��CORE�����дλ�����Ϊ256λ��
//
// ����: nb_transport_bw() bind with isocket of DMAC
//
// ������Ϣ��
// generic_payload.m_address: CMD_BURST_WR_CORE_MEM
// generic_payload.m_data: ָ��MSG_RW_CORE_MEM�ṹ���ָ�룬
//                       �ṹ���е�u32ID�������뱻���������ID�����
// generic_payload.m_length: sizeof(MSG_RW_CORE_MEM)
// phase: BEGIN_RESP
//
// ������Ϣ��
// generic_payload.m_response_status��TLM_OK_RESPONSE
// phase: END_RESP
// 
// ��������ֵ��TLM_COMPLETED


//------------------------------------------------
// CMD_BURST_RD_CORE_MEM (CORE <-- DMAC)
//
// burst��ʽ�£�DMAC��CORE�����дλ�����Ϊ256λ��
//
// ����: nb_transport_fw() bind with tsocket of CORE
//
// ������Ϣ��
// generic_payload.m_address: CMD_BURST_RD_CORE_MEM
// generic_payload.m_data: ָ��MSG_RW_CORE_MEM�ṹ���ָ��
// generic_payload.m_length: sizeof(MSG_RW_CORE_MEM)
// phase: BEGIN_REQ
//
// ������Ϣ��
// generic_payload.m_response_status��
//     ���󱻽��ܣ�TLM_OK_RESPONSE��
//     ����δ�����ܣ��豸æ����TLM_COMMAND_ERROR_RESPONSE
//     �����������TLM_GENERIC_ERROR_RESPONSE��
// phase: END_REQ
// 
// ��������ֵ��TLM_UPDATED


//------------------------------------------------
// CMD_BURST_RD_CORE_MEM (CORE --> DMAC)
//
// burst��ʽ�£�DMAC��CORE�����дλ�����Ϊ256λ��
//
// ����: nb_transport_bw() bind with isocket of DMAC
//
// ������Ϣ��
// generic_payload.m_address: CMD_BURST_RD_CORE_MEM
// generic_payload.m_data: ָ��MSG_RW_CORE_MEM�ṹ���ָ��
// generic_payload.m_length: sizeof(MSG_RW_CORE_MEM)
// phase: BEGIN_RESP
//
// ������Ϣ��
// generic_payload.m_response_status��TLM_OK_RESPONSE
// phase: END_RESP
// 
// ��������ֵ��TLM_COMPLETED


//------------------------------------------------
// CMD_INSN_RW_OUT_MEM (CORE --> DMAC)
//
// һ����Ϣ�пɰ�����2��������+1��д���󣩻�1��������+2��д���󣩡�
// ÿ������ɷ���8����ַ��
// DMAC��������æ�����ܽ���������Ϣ��
// ��ʱDMAC����Ϣ�ڵ����зô�����������ܡ�
//
// ����: nb_transport_fw() bind with tsocket of DMAC
//
// ������Ϣ��
// generic_payload.m_address: CMD_INSN_RW_OUT_MEM
// generic_payload.m_data: ָ��MSG_INSN_RW_MEM�ṹ���ָ��
// generic_payload.m_length: sizeof(MSG_INSN_RW_MEM)
// phase: BEGIN_REQ
//
// ������Ϣ��
// generic_payload.m_response_status��
//     ���󱻽��ܣ�TLM_OK_RESPONSE��
//     ����δ�����ܣ��豸æ����TLM_COMMAND_ERROR_RESPONSE
//     �����������TLM_GENERIC_ERROR_RESPONSE��
// phase: END_REQ
// 
// ��������ֵ��TLM_UPDATED
//
typedef struct msg_insn_rw_mem_elem
{
	bool bolIsRead;         // true: ����false: д��
	DSPU32 u32Bus;      // ����ָ��ʹ�õ�bus��� (0��1)
						// ע:��С��bus��Ŷ�Ӧ�ϴ��slot���
	DSPU32 u32AddrIdx;   // �ǵ�ǰ�����ϵĵڼ�����ַ
						// ��ô��ַ�����������˳�����ͬ
						
	DSPU32 u32Addr;  // ��ַ
	DSPU32 u32Value; // ����


} MSG_INSN_RW_MEM_ELEM;

// �������䵽ĳ�����ߵķô�ָ�������Ϣ
typedef struct msg_insn_rw_mem_bus
{
	// DMAC��Ҫ����Ϣ
	bool bolAtom; // ʹ�ø����ߵ�ָ���Ƿ�Ϊԭ�Ӳ���	
	DSPU32 u32BankConflictNum; // ���bank��ͻ��Ҫ��������
	DSPU32 u32BankConflictState; // bank��ͻ��״̬

	// ������ϢDMAC���账�����ṩ
	DSPU32 u32SlotPlus1; // ���䵽�����ߵ�ָ����ָ�����е�slot���(1 ~ 16)	
					   // 0��ʾ��������δ������
	bool bolSingleWord;      // �Ƿ��ǵ��ַô�ָ��
	bool bolIsDM8;          // �Ƿ���˫ģ8ָ��    

} MSG_INSN_RW_MEM_BUS;

typedef struct msg_insn_rw_mem
{
	DSPU32 u32ID;      // ��ϢID
	DSPU32 u32ElemNum; // ԭʼ�ķô浥Ԫ����������ô�Ľ��ж���С�����Ϊ3 * 8 = 24��
	MSG_INSN_RW_MEM_ELEM arrInsnsRWInfo[24];  // �ô�ָ��ķô���Ϣ

	// arrBusInfo[0]��read_bus_0
    // arrBusInfo[1]��read_bus_1
    // arrBusInfo[2]��write_bus_0
    // arrBusInfo[3]��write_bus_1
	MSG_INSN_RW_MEM_BUS arrBusInfo[4]; // ���䵽�������ϵķô�ָ�������Ϣ

	// ��Ӳ�������,��һ��ִ��������һ��ԭ�ӷô����,
	// ���ִ�����е����зô����������Ϊ��ԭ�ӵġ�(�����ֶ�д)
	bool bolAtom;               // ��ִ�����Ƿ�ԭ�ӷô����

	// for special use(liuyu)
	DSPU32 u32SpecWrMask;
	
} MSG_INSN_RW_MEM;

//------------------------------------------------
// CMD_INSN_RW_OUT_MEM (CORE <-- DMAC)
//
// ����Ӧ��CMD_INSN_RW_OUT_MEM����õ��������ʱ�����ظ���Ϣ��
// һ����Ϣ�ڵ����ж��ô�����ͬʱ���صġ�
//
// ����: nb_transport_bw() bind with isocket of CORE
//
// ������Ϣ��
// generic_payload.m_address: CMD_RW_DMAC_REGS
// generic_payload.m_data: ָ��MSG_INSN_RW_MEM�ṹ���ָ��
// generic_payload.m_length: sizeof(MSG_INSN_RW_MEM)
// phase: BEGIN_RESP
//
// ������Ϣ��

// generic_payload.m_response_status��
//     ���󱻽��ܣ�TLM_OK_RESPONSE��
//     �����������TLM_GENERIC_ERROR_RESPONSE��
//
// phase: END_RESP
// 
// ��������ֵ��TLM_COMPLETED


//------------------------------------------------
// CMD_RESET_DMAC (CORE --> DMAC)
//
// ����: nb_transport_fw() bind with tsocket of DMAC
//
// ������Ϣ��
// generic_payload.m_address: CMD_RESET_DMAC
// phase: BEGIN_REQ
//
// ������Ϣ��
// generic_payload.m_response_status: TLM_OK_RESPONSE
// phase: END_RESP
// 
// ��������ֵ��TLM_COMPLETED
//
//

#endif
