#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib,"ws2_32")

using namespace std;

namespace tracert
{
	struct IP_header
	{
		//Out of the align order.
		unsigned char h_len:4;
		unsigned char version:4;
		
		unsigned char type;
		unsigned short p_len;
		unsigned short id;
		unsigned short flag_n_offset;
		unsigned char TTL;
		unsigned char proto;
		unsigned short checksum;
		unsigned long src_IP;
		unsigned long dest_IP;
	};

	struct ICMP_header
	{
		unsigned char type;
		unsigned char code;
		unsigned short checksum;
		unsigned short id;
		unsigned short seq;
	};
	struct decoding_res
	{
		unsigned short seq_index;
		unsigned long round_time;
		in_addr IP_addr;
	};
//ICMP type:
	const unsigned char ICMP_ECHO_REQUEST=8;
	const unsigned char ICMP_ECHO_REPLY=0;
	const unsigned char ICMP_TIMEOUT=11;

	const unsigned long ICMP_TIMEOUT_DEF=1000;
	const int ICMP_DATA_LEN=32;
	const int ICMP_PACKET_LEN_MAX=1024;
	const int HOP_NUM_MAX=30;


	class instance
	{
	public:
		static void init(const char* IP_addr,CListCtrl* l_ptr,CProgressCtrl* p_ptr,CString* c_ptr);
		static unsigned long __stdcall run(void* arg);
		static void terminate();


	private:
		static struct tracert_row
		{
			int t1;
			int t2;
			int t3;
			char* addr;
		}temp_row;


		static unsigned short generate_checksum(unsigned short* p_buf,int size);
		static int decode_ICMP(char* p_buf,int size,decoding_res& res,int time_i);
		static void show_exception(const char* info,int err_code);
		static void show_exception(const char* info);
		
		static void add_row(int t1,int t2,int t3,const char* addr);


		static sockaddr_in dest_sock_addr;
		static SOCKET sock;
		static char ICMP_send_buf[sizeof(ICMP_header)+ICMP_DATA_LEN];
		static char ICMP_recv_buf[ICMP_PACKET_LEN_MAX];

		static CListCtrl* p_lst;
		static CProgressCtrl* p_prgrs;
		static CString* p_cap;


		

	};



}