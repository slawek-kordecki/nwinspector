#ifndef NEW_TYPES

#define NEW_TYPES

int	Debug;
int	ScreenDebug;
FILE * fd_out;

#define DEBUG(l, f, s)  if (Debug >= l && ScreenDebug) printf(f, s)
#define DEBUG_1(l, f, s)        if (Debug >= l) fprintf(fd_out , f, s)
#define DEBUG_ERR(l, f, s)        if (Debug >= l) fprintf(stderr, f, s)

#define ITOA(string,int_var) sprintf(string,"%04d",int_var)
#define LTOA(string,long_var) sprintf(string,"%ld",long_var)
#define FTOA(string,float_var) sprintf(string,"%6.2lf",float_var)

#define BYTE unsigned char
#define UCHAR unsigned char
#define BCD unsigned char
#define USHORT unsigned short
#define UINT unsigned int
#define ULONG unsigned long

#endif
#define ERR_DBG			1
#define MSG_DBG			3
#define LOG1_DBG		5
#define LOG2_DBG		7

#define	SUBTRACTION		0
#define	ADDITION		1

#define SUCCESS			0
#define NOSUCCESS		1
#define ERROR			-1
