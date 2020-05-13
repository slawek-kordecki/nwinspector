#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <errno.h>
#include <libgen.h>
#include  "h/UDPcomm.h" 
#include  "h/sla_type.h" 

/*********************** PROTYPES *********************************************/ 
extern char * messages[];
extern char *optarg;
extern int optind;
char *optionS = "i:hmD:n:s:v";
char  * version = "UDPClient 1.06 01.07.2019";
char		progname[BUFSIZ];
int rc;
int sockfd; 
struct sockaddr_in	 servaddr, cliAddr; 
char host[16];
char net[16];
  char string[100];

extern char additionalMessageContent[];
extern char controlledHostIP[];
extern char progname[];
/*************************************************************/
void help(char * name)
/*************************************************************/
{
fprintf(stderr,"%s\t -i index of messages array\n",name);
fprintf(stderr,"\t[-n broadcast address | -s server address]\n\t-D number debug level\n\t -m list of avalaibles messages\n\t-h this help\n");

return;
}
 

/*************************************************************/
int sendMessage (char * msg, int sockfd, struct sockaddr_in servaddr){
/*************************************************************/

  	int res;
	sendto(sockfd, (const char *)msg, strlen(msg), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
	if (res < 0) {
                fprintf (stderr,"Konnte Daten nicht senden) (%s)\n",  strerror(errno));
                exit (EXIT_FAILURE);
        }
}
/*************************************************************/
createClient()
/*************************************************************/
{

	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 



	if(strlen(net) > 0 ){
//	Für broadcast
	int broadcastEnable=1;
	int ret=setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
	}

	/* Jeden Port bind(en) */
	cliAddr.sin_family = AF_INET;
	cliAddr.sin_addr.s_addr = htonl (INADDR_ANY);
	cliAddr.sin_port = htons (PORT);
	rc = bind ( sockfd, (struct sockaddr *) &cliAddr, sizeof (cliAddr) );
	if (rc < 0) {
	   sprintf (string, "Konnte Port nicht bind(en) (%s)\n", strerror(errno));
		Log(1,"%s", string );

	   exit (EXIT_FAILURE);
	}


	memset(&servaddr, 0, sizeof(servaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(PORT); 
	//servaddr.sin_addr.s_addr = INADDR_ANY; 
	// servaddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	// - correcte adresse inet_pton(AF_INET, "144.99.111.125", &servaddr.sin_addr.s_addr);
	if(strlen(net) > 0 ){
		inet_pton(AF_INET, net, &servaddr.sin_addr.s_addr);
	}else if ( strlen(host) >0 ) {

		inet_pton(AF_INET, host, &servaddr.sin_addr.s_addr);
	}
}


/*************************************************************/
int main (int argc, char **argv) {
/*************************************************************/
  	int s,n, i, idx, len;
	int             opt_chr = 0; 
	char buffer[MAXLINE]; 
	char  *msg ;
	char index[4];
// default values if not as options
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
	strcpy(progname, argv[0]);
  switch (opt_chr)
    {
        case 'm' : getMessages(); return(0); 
        case 'i' : strcpy(index,optarg); 
			if(strlen(index) == 0 ){
				help(argv[0]);
				return(EINVAL);
			}
			break;
	case 'D' : Debug = atoi(optarg);break;
	case 's' : strcpy(host,optarg);break;
	case 'n' : strcpy(net,optarg);break;
	case 'v' : fprintf(stderr,"Version %s\n", version); return(0);
    }
  }
if(strlen(host) == 0 && strlen(net) == 0 ) {
	help(argv[0]);
	exit(EXIT_FAILURE);
}
if(strlen(host) > 0 && strlen(net) > 0 ) {
	help(argv[0]);
	exit(EXIT_FAILURE);
}
char * ptr;
ptr=string;
getLogname(ptr, basename(argv[0]));
printf("Logname : %s\n", ptr);
openLogging(ptr);
createClient();
	idx = atoi(index);
	int firstRequest = 1;
	int datagramnr =0;
	while( 1==1 ){	
	char msgnr[MSG_NR_LEN+1];	
	strncpy(msgnr, msg, MSG_NR_LEN);		
	msgnr[MSG_NR_LEN+1]=0;
	if( strcmp(msgnr, "999")  == 0 )
		break;
	msg = buffer;
	
	if(firstRequest==0 ){
		
		handleMessage(msg, CLIENT, NULL);
		rc=getResponse(msg, msg, additionalMessageContent);
		handleMessage(msg, CLIENT, NULL );
	} else {
		strcpy(msg,messages[idx]);		
		msg[strlen(messages[idx])]=0;
		firstRequest = 0;
	}

	rc=sendMessage( msg, sockfd, servaddr );
	if (rc < 0) {
     		Log(0,"Konnte Daten nicht senden) (%s)\n",  strerror(errno));
     		exit (EXIT_FAILURE);
  	}
	Log(0,"Client --> %s\n", msg);
	//  Read answer
	n = recvfrom(sockfd, (char *)buffer, MAXLINE, 0, (struct sockaddr *) &servaddr, &len); 
	buffer[n] = '\0'; 
	sprintf(string,"(%d):Received %d chars from %s\n", datagramnr++,n, inet_ntoa(servaddr.sin_addr) );
	Log(1,"%s", string);
	Log(0,"Server --> %s\n", msg);
	if( strncmp(buffer,"999", MSG_NR_LEN) == 0 ) // OK- Talk end
		break;
	}
	close(sockfd); 
	return 0; 
} 
