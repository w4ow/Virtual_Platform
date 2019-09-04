/*
 * main.cpp
 *
 *  Created on: Jan 31, 2012
 *      Author: root
 */
//#include <vld.h>

#define DSP_MAX_SIM_CYCLE 5000
#define DSP_CLK_PERIOD 10

#include "dsp_top.h"
#include <systemc.h>

//std::ofstream flogout("log.txt");
int check_license();

int sc_main (int argc , char **argv)
{
	
	int a =0xff;

	sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);
	unsigned int port=0;
	unsigned int u32CRtnValue;
	string ipAdr="";
	vector<string> vFileName;

	if( (argc == 2) && (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "-V") == 0) )
	{
		printf("Build date\t: %s %s\n", __DATE__, __TIME__);
		printf("Architecture\t: BWDSP104x\n");
		return 0;
	}
	/*
	cout << "************************************************" << endl;
	check_license();
	cout << "************************************************" << endl;
	*/
	//�����пɴ�������Ҳ�ɲ�������
	//������������������Ŀ���ļ���IP��ַ���˿ں�������Ϣ��
	//������ʽΪ��
	//�˺� Ŀ���ļ����������ñ���������ģ��������ʱ���������ļ���
	//-ip ϣ��BIND���ĵ�ַ��������һ�����ʡ�ԣ�Ĭ��BIND������IP��ֻ�е������ж�����������IPʱ������Ҫ���ñ�������
	//-p �˿ں� ��Ĭ��Ϊ5688���������ñ���������Ϊ5688��
	//���磬����������ΪSIM����
	//SIM 0 example0.out 1 example1.out -ip 10.100.3.109 -p 5689
	//��ʾģ��������ʱ0�˼���example0.out��1�˼���example1.out�����ӵ�IP��ַ
	//10.100.3.109�ϵ�5689�˿���
	//����
	//SIM
	//��ʾģ��������ʱ�������κ�Ŀ���ļ������ӵ�������5688�˿���
#ifdef BWSIM_SUPPORT_IO
	ExtractAppPara(argc,argv,vFileName);
#else
	if(ExtractPara(argc,argv,vFileName))
	{
		cout<<"Illegal paramter!"<<endl;
		return 1;
	}
#endif
	/*
	cout<<"port : "<<port<<endl; 
	cout<<"IP: "<<ipAdr<<endl;
	for(int i=0;i<ISS_NUM_DEF;i++)
	{
		cout<<"core "<<i<<" exe name : "<<vFileName[i]<<endl;
	}
	cout << "Main clock frequency is 100MHz" << endl;
	cout << "Initialization completed!" << endl;
	cout << "************************************************" << endl;
	cout << "************************************************" << endl;
	cout << "Running BW104x simulator" << endl;
	cout << "************************************************" << endl;
	*/

	//sc_clock main_clk("MainCLK", 20, SC_NS);
	// ʱ�����ڣ�DSP_CLK_PERIOD ����
	// ռ�ձȣ�0.5
	// ��һ�����س���ʱ�䣺DSP_CLK_PERIOD / 2 ����
	// ��һ�����أ�������	
	sc_clock main_clk("MainCLK", DSP_CLK_PERIOD, 0.5, DSP_CLK_PERIOD / 2, true);

	dsp_top* p_dsp_top;
	sc_iss* p_iss[ISS_NUM_DEF];

	p_dsp_top = new dsp_top("DSP_TOP");
	p_dsp_top->m_dsp_top_clk(main_clk);


	int ret = LoadFile(vFileName,p_dsp_top);

#ifndef SC_ISS_RUN_ALONE
#ifndef BWSIM_SUPPORT_IO//��ģ����֧��IO������Ϊ���Ա������ṩ�������ټ���JTAGģ��
	boost::thread jtag_accept(boost::bind(&jtag_accept_thread,p_dsp_top));
	boost::thread jatg_notify(boost::bind(&jtag_notify_thread));
	boost::thread gateway_accept(boost::bind(&gateway_accept_thread));
#else
	if(ret)
	{
		return 0;
	}
#endif
#endif

	//sc_trace_file * my_trace_file;
	//my_trace_file = sc_create_vcd_trace_file("BWDSP104x_trace");
	//sc_trace(my_trace_file, main_clk, "clk");

	//sc_start(-1);  // ��������
	/*
	if(argc != 5)
	{
		fprintf(stderr, "error command, please enter: ./sim 0 a.out -t 100000\n");
		exit(-1);
	}
	if( strcmp(argv[3], "-t") != 0)
	{
		fprintf(stderr, "you should specify simulation cycle\n");
		exit(-1);
	}
	int sim_cycles = atoi(argv[4]);

	clock_t start = clock();
	sc_start(sim_cycles);
	clock_t end = clock();
	float t = end - start;
	t /= CLOCKS_PER_SEC;
	fprintf(stderr, "simulation time is %.6f seconds\n", t);
	*/
	// �������У�ֱ����⵽��0�г��ֽ�����־(rtrap 3)
	p_iss[0] = p_dsp_top->GetCoreHandle(0);
	p_iss[1] = p_dsp_top->GetCoreHandle(1);
    sc_time t1(DSP_CLK_PERIOD,SC_NS);
	while (1)  
	{
		sc_start(t1);  // ����һ��DSPʱ������
#ifdef BWSIM_SUPPORT_IO
		if (((vFileName[0].length()==0)||p_iss[0]->IsTestEnded()) &&
			((vFileName[1].length()==0)||p_iss[1]->IsTestEnded()))
		{
			break;
		}
#else
		if (p_iss[0]->IsTestEnded()||p_iss[1]->IsTestEnded())
		{
			break;
		}
#endif


		if(p_iss[0]->isIdle() && p_iss[1]->isIdle())
		{
			p_iss[0]->ReleaseIdle();
			p_iss[1]->ReleaseIdle();
		}
	}
	//sc_close_vcd_trace_file(my_trace_file);

	u32CRtnValue = p_dsp_top->ReadXR8ofCore(0);
	return u32CRtnValue;
}

int check_license()
{
	const int MAX_RANGE = 9999;
	const int MIN_RANGE = 1000;
	FILE *fp;
	int r2;
	long long int r3;

	fp = fopen("./key.lic", "r+");
	if(fp == NULL)
	{
		fprintf(stderr, "cannot open key.lic\n");
		exit(-1);
	}

	char c[4], d;
	int cnt = 0;
	long long int r[4];

	while(cnt < 4)
	{
		fread(c, sizeof(char), 4, fp);
		fread(&d, sizeof(char), 1, fp);
		int num = atoi(c);
		if(num > MAX_RANGE || num < MIN_RANGE || (cnt < 3 && d != '-')) 
		{
			fprintf(stderr, "licence format error\n");
			exit(-1);
		}
		r[cnt] = num;
		cnt++;
	}

	r2 = (r[0] + r[1]) / 2;
	r3 = r[0];
	r3 *= (r[1] * r[2]);
	r3 -= (r[0] + r[1] + r[2]);
	r3 %= (MAX_RANGE - MIN_RANGE);
	r3 += MIN_RANGE;

	if(r2 != r[2] || r3 != r[3])
	{
		fprintf(stderr, "license check failed\n");
		exit(-1);
	}

	fprintf(stderr, "license check success\n");
	return 0;
}