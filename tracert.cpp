#include "stdafx.h"
#include "tracert.h"


namespace tracert
{
	instance::tracert_row instance::temp_row;

	sockaddr_in instance::dest_sock_addr;
	SOCKET instance::sock;

	char instance::ICMP_send_buf[sizeof(ICMP_header)+ICMP_DATA_LEN];
	char instance::ICMP_recv_buf[ICMP_PACKET_LEN_MAX];

	CListCtrl* instance::p_lst;

	void instance::init(const char* IP_addr,CListCtrl* l_ptr)
	{
		if(!l_ptr)
		{
			show_exception("pointer of list is NULL.");
			return;
		}
		
		//Initialize the list control.
		p_lst=l_ptr;
		DWORD grid_style = p_lst->GetExtendedStyle();
		grid_style|= LVS_EX_FULLROWSELECT;
		grid_style|= LVS_EX_GRIDLINES;
		p_lst->SetExtendedStyle(grid_style);



		WSADATA w_data;
		if(WSAStartup(MAKEWORD(2,2),&w_data)!=0)
		{
			show_exception("Failed to initialize WinSock2 DLL.",WSAGetLastError());
			return;
		}

		//Turn IP_addr from string into IP address.
		unsigned long dest_IP=inet_addr(IP_addr);
		if(dest_IP==INADDR_NONE)
		{
			//DNS resolving.
			hostent* p_hostent=gethostbyname(IP_addr);
			if(p_hostent)
				dest_IP=(*(in_addr*)p_hostent->h_addr).s_addr;
			else
			{
				show_exception("Can not resolve this host name.",WSAGetLastError());
				return;
			}
		}
		
		ZeroMemory(&dest_sock_addr,sizeof(sockaddr_in));
		dest_sock_addr.sin_family=AF_INET;
		dest_sock_addr.sin_addr.s_addr=dest_IP;

		sock=WSASocket(AF_INET,SOCK_RAW,IPPROTO_ICMP,NULL,0,WSA_FLAG_OVERLAPPED);
		if(sock==INVALID_SOCKET)
		{
			show_exception("Failed to create a raw socket.",WSAGetLastError());
			WSACleanup();
			return;
		}

		//Set up socket with timeout.
		int timeout=ICMP_TIMEOUT_DEF;
		if(setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(timeout))==SOCKET_ERROR)
		{
			show_exception("Failed to set timeout.",WSAGetLastError());
			closesocket(sock);
			WSACleanup();
			return;
		}

		//Initialize buffers.
		memset(ICMP_send_buf,0,sizeof(ICMP_send_buf));
		memset(ICMP_recv_buf,0,sizeof(ICMP_recv_buf));

		//Initialize the header.
		ICMP_header* p_header=(ICMP_header*)ICMP_send_buf;
		p_header->type=ICMP_ECHO_REQUEST;
		p_header->code=0;
		p_header->id=(unsigned short)GetCurrentProcessId();
		memset(ICMP_send_buf+sizeof(ICMP_header),'E',ICMP_DATA_LEN);
	}

	void instance::add_row(int t1,int t2,int t3,const char* addr)
	{
		int index=p_lst->GetItemCount();
		char str[100];
		sprintf(str,"%d",index);
		CString cstr(str);
	
		int row_index=p_lst->InsertItem(index,_T(""));
		p_lst->SetItemText(row_index,0,cstr);

		if(t1>0)
		{
			sprintf(str,"%d",t1);
			cstr=CString(str);
			p_lst->SetItemText(row_index,1,cstr);
		}
		else
			p_lst->SetItemText(row_index,1,_T("*"));

		if(t2>0)
		{
			sprintf(str,"%d",t2);
			cstr=CString(str);
			p_lst->SetItemText(row_index,2,cstr);
		}
		else
			p_lst->SetItemText(row_index,2,_T("*"));

		if(t3>0)
		{
			sprintf(str,"%d",t3);
			cstr=CString(str);
			p_lst->SetItemText(row_index,3,cstr);
		}
		else
			p_lst->SetItemText(row_index,3,_T("*"));

		cstr=CString(addr);
		p_lst->SetItemText(row_index,4,cstr);
	}

	unsigned long __stdcall instance::run(void* arg)
	{
		decoding_res res;
		int is_dest_reached=FALSE;
		unsigned short seq_index=0;
		int TTL=1;
		int hop_limit=HOP_NUM_MAX;
		
		int i=0;

		while(!is_dest_reached && hop_limit)
		{
			//Set up the TTL in IP header.
			setsockopt(sock,IPPROTO_IP,IP_TTL,(char*)&TTL,sizeof(TTL));
			
			//Set up the ICMP header for the left part.
			((ICMP_header*)ICMP_send_buf)->checksum=0;
			((ICMP_header*)ICMP_send_buf)->seq=htons(seq_index++);
			((ICMP_header*)ICMP_send_buf)->checksum=generate_checksum((unsigned short*)ICMP_send_buf,sizeof(ICMP_header)+ICMP_DATA_LEN);

			//Record sequence index and current time.
			res.seq_index=((ICMP_header*)ICMP_send_buf)->seq;
			res.round_time=GetTickCount();

			//Send ECHO REQUEST ICMP datagram.
			if(sendto(sock,ICMP_send_buf,sizeof(ICMP_send_buf),0,(sockaddr* )&dest_sock_addr,sizeof(dest_sock_addr))==SOCKET_ERROR)
			{
				if(WSAGetLastError()==WSAEHOSTUNREACH)
					show_exception("Destination host is unreachable.");
				else
					show_exception("Failed to send ECHO REQUEST ICMP datagram.",WSAGetLastError());
				closesocket(sock);
				WSACleanup();
				return -1;
			}

			sockaddr_in src;
			int src_len=sizeof(src);
			int data_len;

			
			for(;;)
			{
				//Waiting for data sent back.
				data_len=recvfrom(sock,ICMP_recv_buf,ICMP_PACKET_LEN_MAX,0,(sockaddr* )&src,&src_len);
				
				//If there is data received.
				if(data_len!=SOCKET_ERROR)
				{
					//Decode ICMP packet, if successful then jump out of the loop
					//and send next ECHO REQUEST ICMP datagram.
					if(decode_ICMP(ICMP_recv_buf,data_len,res,i%3))
					{
						if(res.IP_addr.s_addr==dest_sock_addr.sin_addr.s_addr)
							is_dest_reached=TRUE;
						
						temp_row.addr=inet_ntoa(res.IP_addr);
						break;
					}
				}
				else if(WSAGetLastError()==WSAETIMEDOUT)
				{
					temp_row.t1=-1;
					break;
				}
				else
				{
					show_exception("Failed to receive packet.",WSAGetLastError());
					closesocket(sock);
					WSACleanup();
					return -1;
				}
			}
			
			//Loop for 3 times.
			if((++i)%3!=0)
				continue;

			TTL++;
			hop_limit--;

			//Show the new row.
			add_row(temp_row.t1,temp_row.t2,temp_row.t3,temp_row.addr);
		}
		return 0;
	}

	unsigned short instance::generate_checksum(unsigned short* p_buf,int size)
	{
		unsigned long res=0;
		while(size>1)
		{
			res+=*p_buf++;
			size-=sizeof(unsigned short);
		}
		if(size)
			res+=*(unsigned char*)p_buf;

		//Add up carry bits.
		res=(res >> 16)+(res & 0xffff);
		res+=(res >> 16);//There may be carry bit after the code above.
		return (unsigned short)(~res);
	}

	int instance::decode_ICMP(char* p_buf,int size,decoding_res& res,int time_i)
	{
		//First check whether the length is legal.
		IP_header* p_IP_h=(IP_header* )p_buf;
		int IP_h_len=p_IP_h->h_len*4;//Unit is 32 bits.
		if(size<(int)(IP_h_len+sizeof(ICMP_header)))
			return FALSE;

		//Check the identifier and sequence info of the ICMP packet
		//with correspond methods to the type of the packet,
		//and then decide whether to accept it or drop it.
		ICMP_header* p_ICMP_h=(ICMP_header* )(p_buf+IP_h_len);
		unsigned short id;
		unsigned short seq_index;
		if(p_ICMP_h->type==ICMP_ECHO_REPLY)
		{
			id=p_ICMP_h->id;
			seq_index=p_ICMP_h->seq;
		}
		else if(p_ICMP_h->type==ICMP_TIMEOUT)
		{
			 char* p_inner_IP_h=p_buf+IP_h_len+sizeof(ICMP_header);
			 int p_inner_IP_len=((IP_header*)p_inner_IP_h)->h_len*4;
			 ICMP_header* p_inner_ICMP_h=(ICMP_header* )(p_inner_IP_h+p_inner_IP_len);
			 id=p_inner_ICMP_h->id;
			 seq_index=p_inner_ICMP_h->seq;
		}
		else
			return FALSE;
		if(id!=(unsigned short)GetCurrentProcessId()||seq_index!=res.seq_index)
			return FALSE;

		//Now handle the correct datagram.
		if(p_ICMP_h->type==ICMP_ECHO_REPLY||p_ICMP_h->type==ICMP_TIMEOUT)
		{
			res.IP_addr.s_addr=p_IP_h->src_IP;
			res.round_time=GetTickCount()-res.round_time;
			if(res.round_time)
			{
				switch (time_i)
				{
				case 0:temp_row.t1=res.round_time;break;
				case 1:temp_row.t2=res.round_time;break;
				case 2:temp_row.t3=res.round_time;break;
				default:
					break;
				}
			}
			else
			{
				//<0ms.
				switch (time_i)
				{
				case 0:temp_row.t1=0;break;
				case 1:temp_row.t2=0;break;
				case 2:temp_row.t3=0;break;
				default:
					break;
				}
			
			}
			return TRUE;
		
		}
		else
			return FALSE;
	}
		


	void instance::show_exception(const char* info,int err_code)
	{
		CString info_cstr(info);
		char err[100];
		sprintf(err," Error code: %d.",err_code);
		CString err_cstr(err);
		
		MessageBox(NULL,info_cstr+err_cstr,_T("Error"),MB_ICONERROR);
	}

	void instance::show_exception(const char* info)
	{
		CString info_cstr(info);
		MessageBox(NULL,info_cstr,_T("Error"),MB_ICONERROR);
	
	}
}