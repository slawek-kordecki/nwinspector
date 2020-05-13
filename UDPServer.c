#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <errno.h>
#include <libgen.h>
#include "h/UDPcomm.h"
#include "h/sla_type.h"

 
#define MAXLINE 1024 
//****************************************************************************************************************
extern char * messages[];
//****************************************************************************************************************
/*********************** PROTYPES *********************************************/ 
char  * version = "V1.06 01.07.2019";
extern char * messages[];
extern char *optarg;
extern int optind;
char *optionS = "i:hmD:v";
extern char additionalMessageContent[];
extern char controlledHostIP[];
extern char progname[];
/*************************************************************/
void help(char * name)
/*************************************************************/
{
fprintf(stderr,"%s\t -i index of messages array\n",name);
fprintf(stderr,"\t-D number debug level\n\t -m list of avalaibles messages\t-h this help\n");

return;
}
/*************************************************************/
runSystemCmd(char * cmd ){
/*************************************************************/
char command[BUFSIZ];
   system(cmd);
}

//****************************************************************************************************************
int main (int argc, char **argv) {
//****************************************************************************************************************
	int sockfd; 
	char buffer[MAXLINE]; 
	char *hello = "Hello from server"; 
	struct sockaddr_in servaddr, cliaddr; 
	int             opt_chr = 0; 
	char cmd[100];
	char string[100];
	
ScreenDebug = 1;
Debug = 3;

while ((opt_chr != -1) && (opt_chr != '?'))
  {
  opt_chr = getopt(argc, argv, optionS);
  if (opt_chr == -1)
        break;
  if (opt_chr == '?' || opt_chr == 'h')
	{
	help(argv[0]);
	return(EINVAL);
	}
  switch (opt_chr)
    {
        case 'm' : getMessages(); return(0); 
	case 'D' : Debug = atoi(optarg);break;
	case 'v' : fprintf(stderr,"%s %s\n", argv[0], version); return(0);
    }
  }
char * ptr;
ptr=string;
getLogname(ptr, basename(argv[0]));
printf("Logname : %s\n", ptr);
openLogging(ptr);
	Log(0,"%s","Start UDPServer\n");


	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	//int broadcastEnable=0;
	//int ret=setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
	
	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&cliaddr, 0, sizeof(cliaddr)); 
	memset(buffer,0, sizeof(buffer));
	
	// Filling server information 
	servaddr.sin_family = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	servaddr.sin_port = htons(PORT); 
	
	// Bind the socket with the server address 
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	int len, n, datagramnr = 0; 
	while(1){
		n = recvfrom(sockfd, (char *)buffer, MAXLINE, 0, ( struct sockaddr *) &cliaddr, &len); 
		if ( n  <  0 ) {
     			Log(0," Keine Zeichen empfangen) (%s)\n", strerror(errno));
			continue;
  			}	

		sprintf(string,"(%d):Received %d chars from %s\n", datagramnr++,n, inet_ntoa(cliaddr.sin_addr) );
		Log(1,"%s", string );
		buffer[n] = '\0'; 
		Log(0,"Client --> %s\n", buffer); 
		memset(cmd,0, sizeof(cmd));
		handleMessage(buffer, SERVER, cmd );
		
		if ( strlen(cmd) > 0 ){
			int res = system(cmd);
			sprintf(string,"Server executed command (%s) with result (%d) \n", cmd, res);
		 	Log(0, "%s", string);
		}
		
		char * request  = buffer;
		char * response = buffer;
		
		int res = getResponse(request, response, additionalMessageContent);
		if( res == 0 ) {	
			sendto(sockfd, (const char *)response, strlen(response), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len); 
			Log(0,"Server --> %s\n", response); 
			memset(buffer,0, sizeof(buffer));
		}	 else {
		 		sprintf (string, "Server : I have not response on this request (%s)\niExit\n", request);
		 		Log(0, "%s", string);
                       		return(-1); 
				}
	}
return SUCCESS; 
}

