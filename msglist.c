#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "h/UDPcomm.h"
#include "h/sla_type.h"
// my comment
#include <unistd.h> 
#include <errno.h> 
#include <netdb.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 

char * messages[] = { 
                        "001|Hello|100|", 				// 0
                        "100|OK|999|", 				   // 1
                        "101|WildFly don't response|102|", 				// 2
                        "102|I made reset of WildFly|100|", 				// 3
                        "103|Reset of WildFly done|100|", 				// 4
                        "104|Cluster ifxcluster Arbitrator FOC has a non-unique PRIORITY value|105|" ,				// 5
                        "105|Get IP Address of CM|106|", 				// 6
                        "106| |107|", 				// 7
                        "107|I got the IP of CM and make my job|100|", 				// 8
                        "108|ConnectioManager is not running|105|", 				// 9
                        "109|I got the IP of CM and start the CM|100|", 				// 10
                        "999|Bye|999|", 				// 11
                        "NULL|" 				// 12
                        };
                        
char additionalMessageContent[100];
char controlledHostIP[16];
extern int ScreenDebug;
extern int Debug;
char	progname[BUFSIZ];

void Log(int level, char * format, char * text ){
	

	DEBUG(level,"%s # ", timestamp());
	DEBUG(level,format, text);
	if( fd_out != NULL ){
		DEBUG_1(level,"%s # ", timestamp());
		DEBUG_1(level,format, text);
		fflush(fd_out);
	}
}

void getLogname ( char * logname, char * name){

strcpy(logname,"/var/log/");
printf("name:%s\n",name);
strcat(logname, name);
strcat(logname,".log");
printf("File log %s \n", logname);
}

void openLogging(char * logname) {

printf("Open File %s \n", logname);
if( (fd_out=fopen(logname,"a+")) == (FILE *) NULL )
    {           

    perror("fopen");
    exit(ERROR);
    }

printf("File %s opened\n", logname);



}

void checkHostName(int hostname) 
{ 
    if (hostname == -1) 
    { 
        perror("gethostname"); 
        exit(1); 
    } 
} 
  
// Returns host information corresponding to host name 
void checkHostEntry(struct hostent * hostentry) 
{ 
    if (hostentry == NULL) 
    { 
        perror("gethostbyname"); 
        exit(1); 
    } 
} 
  
// Converts space-delimited IPv4 addresses 
// to dotted-decimal format 
void checkIPbuffer(char *IPbuffer) 
{ 
    if (NULL == IPbuffer) 
    { 
        perror("inet_ntoa"); 
        exit(1); 
    } 
} 
  
//********************************************************************************
char * getLocalIpAddress(){
//********************************************************************************

    char hostbuffer[256]; 
    char *IPbuffer; 
    struct hostent *host_entry; 
    int hostname; 
	char * addresse ;
  
    // To retrieve hostname 
    hostname = gethostname(hostbuffer, sizeof(hostbuffer)); 
    checkHostName(hostname); 
  
    // To retrieve host information 
    host_entry = gethostbyname(hostbuffer); 
    checkHostEntry(host_entry); 
  
    // To convert an Internet network 
    // address into ASCII string 
    IPbuffer = inet_ntoa(*((struct in_addr*) 
                           host_entry->h_addr_list[0])); 
  
//    printf("Hostname: %s\n", hostbuffer); 
//    printf("Host IP: %s", IPbuffer); 
return IPbuffer;
}
//********************************************************************************
void getMessages(){
//********************************************************************************
int idx = 0;

while (messages[++idx] != NULL) 
	printf("(%d) --> %s\n", idx, messages[idx]);
}
//********************************************************************************
char * clipped(char * string)
//********************************************************************************
{
char * ptr;
ptr=string;

while(*ptr++)
        ;
ptr--;
ptr--;

while(*ptr)
        {
        if(*ptr!=' ')
                {
                ptr++;
                break;
                }
        ptr--;
        }
*ptr=0;
return(string);
}



//****************************************************************************************************************
int getResponse(char * request, char * response, char * additionalMessageContent ) {
//****************************************************************************************************************
 /*
        REQUEST
 __________________________________
 I RQ_MNR  I MESSAGE  I RQ_EX_MNR      I
 +---------+----------+---------------+

  
        RESPONSE
 +---------+------------------------+
 I RS_MNR  I  MESSAGE  I EX_MNR     I
 L---------+------------------------+

 MNR      - Message Number
 Message  - Plain Text
 RQEXMNR - Request Expected Message Number
 RSMNR   - Response Message Number
 EXMNR   - Expected Message Number

	where:
  RQ_EX_MNR === RS_MNR

*/
	
	int idx;
	char string[4];
	char string2[4];
	int errornr ;
	char * response_ptr ;
	//char * RQ_EX_MNR;
	//char response[MAXLINE];
	//char msg[MAXLINE];
	char buffer[MAXLINE];
	char RQ_EX_MNR[100];
	char RS_EX_MNR[100];
	char RS_MNR[100];
	
	Log(2,"\nREQUEST;(%s)\n", request);
	strcpy(buffer,request);
	strncpy(string, request,3 );
	string[3] = 0;
	Log(3,"additionalMessageContent:(%s)\n", additionalMessageContent);
//fprintf(stderr,"%s\n", getLocalIpAddress());
	

	char * RQ_MNR = string;
	strncpy( RQ_MNR,  request,3 );

	if( strcmp(RQ_MNR,"999") == 0 ) // OK- Talk end
		RS_MNR[MSG_NR_LEN] = 0;

    char * msg  ;
    msg = &buffer[3];
	memset((char*)RQ_EX_MNR,0,sizeof(RQ_EX_MNR));
	memset((char*)RS_MNR,0,sizeof(RS_MNR));

	if( strcmp(RQ_MNR,"999") == 0 ) // OK- Talk end
		return 0;
	// search request and get RQ_EX_MNR
	int res = getMsgByMsgNr(RQ_MNR, msg, RQ_EX_MNR);
	if( res < 0 ){
		Log(0,"Message number %s not found\n", RS_MNR);
		memset(response, 0, MAXLINE );
		return -1;	
		}
	Log(3,"msg:%s\n", msg);
	Log(3,"RQ_EX_MNR:%s\n", RQ_EX_MNR);
	// get response
	res = getMsgByMsgNr(RQ_EX_MNR, msg, RS_EX_MNR);
	if( res < 0 ){
		Log(0,"Message number %s not found\n", RS_MNR);
		return -1;	
		}
	// build response	
	memset(response, 0, sizeof(msg)); 
	strcat(response,RQ_EX_MNR);
	strcat(response,"|");
	if ( (strlen(msg) == 1) &&  msg[0] == ' ' )
		msg = clipped(msg);
	else
		strcat(response,msg);
	if( strlen(additionalMessageContent) > 0 )
		strcat(response,additionalMessageContent);
	strcat(response,"|");
	strcat(response, RS_EX_MNR);
	Log(2,"RESPONSE;(%s)\n", response);
	memset(additionalMessageContent,0,sizeof(additionalMessageContent));
return (0);
}
//********************************************************************************
int getMsgByMsgNr( char * msgNr, char * msg, char * expectedResponseNumber ) {
//********************************************************************************

char *str, *token, *saveptr;
char *str2, *subtoken, *saveptr2;
int idx, j, k;

Log(3,"getMsgByMsgNr( %s , ", msgNr );
Log(3," %s )\n", msg );

char buffer[100];
char help[15];
idx=0;
while (messages[idx] != NULL) { 
	ITOA(help,idx);
	Log(3,"messages[%s]=", help);
	Log(3,"%s\n", messages[idx]);
        str=messages[idx];
	memset(buffer,0,100);
	j=0;
        while(*str!=0)
        	buffer[j++]= *str++;
        buffer[j++]= 0;
 	str = buffer;
        token = strtok_r(str, "|", &saveptr);
	Log(3,"Token:%s\n", token);
	if( strcmp(token, msgNr) == 0 ) {
		// MsgNr found
        str=messages[idx];
	memset(buffer,0,100);
	j=0;
        while(*str!=0)
        	buffer[j++]= *str++;
        buffer[j++]= 0;
 		for ( k=1, str2 = buffer; ; k++, str2 = NULL) {
                   subtoken = strtok_r(str2, "|", &saveptr2);
			ITOA(help,k);
                   Log(3,"%s --> ", help);
                   Log(3,"%s\n", subtoken);
                   if (subtoken == NULL)
				return 0;	
			if( k ==1 )
				msgNr = subtoken;
			if( k==2 )
				strcpy(msg,subtoken);
			if( k == 3 ){
				strcpy(expectedResponseNumber,subtoken);
				return 0;	
			}	
               }
	}
idx++;
}


return -1;
}
//********************************************************************************
splitMessage(char * message,char * msgNr, char * msgContent, char * expectedResponseNumber ){
//********************************************************************************
	
char buffer[MAXLINE];
char *str2, *subtoken, *saveptr2;
int idx, j, k;
char help[15];

	strcpy(buffer,message);
 		for ( k=1, str2 = buffer; ; k++, str2 = NULL) {
                   subtoken = strtok_r(str2, "|", &saveptr2);
			ITOA(help,k);
                   Log(3,"%s --> ", help);
                   Log(3,"%s\n", subtoken);
                   if (subtoken == NULL)
				return 0;	
			if( k ==1 )
				msgNr = subtoken;
			if( k==2 )
				strcpy(msgContent, subtoken);
			if( k == 3 ){
				strcpy(expectedResponseNumber,subtoken);
				return 0;	
			}	
               }
}

//********************************************************************************
int handleMessage(char * msg, int server, char * cmd){
//********************************************************************************
char *ptr;
char msgContent[100];
char msgnr[MSG_NR_LEN+1];
char expectedResponseNumber[MSG_NR_LEN+1];


		memset(msgnr,0,sizeof(msgnr));
         strncpy(msgnr, msg, MSG_NR_LEN);
        msgnr[MSG_NR_LEN] = 0;
	Log(3,"\nhandleMessageClient(%s)\n", msg);
	Log(3,"handleMessageClient:msgnr:(%s)\n", msgnr);
	if ( strcmp(msgnr, "101")== SUCCESS){
	      //memset(additionalMessageContent,0,sizeof(additionalMessageContent));
	      if( server == 1){
		strcat(cmd, "/etc/init.d/wildfly restart ");
		return SUCCESS;
		}
	}
      if ( strcmp(msgnr, "105")== SUCCESS){
	      memset(additionalMessageContent,0,sizeof(additionalMessageContent));
		strcpy(additionalMessageContent, "144.99.111.146");	
		fprintf(stderr,"additionalMessageContent:%s\n", additionalMessageContent);	
		return SUCCESS;
	}
	if ( strcmp(msgnr, "106")== SUCCESS ){
		Log(3,"msgnr:%s\n",msgnr); 
		splitMessage(msg, msgnr, msgContent, expectedResponseNumber );
		fprintf(stderr,"msg %s (msgnr:%s msgContent:%s expectedResponseNumber:%s)\n", msg, msgnr, msgContent, expectedResponseNumber);
		if( strlen(msgContent) > 0 )
				strcpy(controlledHostIP,msgContent);
		Log(3,"controlledHostIP:%s\n", controlledHostIP);	
		if( server == 1){
			//strcat(cmd, "/usr/bin/ping -c3 ");
			//strcat(cmd, controlledHostIP);
			strcat(cmd, "/usr/local/bin/resetCM.sh");	
			//strcat(cmd, "/usr/local/bin/stopCM.sh");	
		}
		return SUCCESS;
	}
	if( strcmp(msgnr, "999")  == 0 )
		return COMM_END;

}
//
