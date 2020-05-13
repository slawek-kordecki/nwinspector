/*SRC_ANF 
*********************************************************************
* Gesellschaft fuer Bargeldlose Zahlungs- und Abrechnungssysteme mbH*
*                      BZA   D-88682 Salem                           *
*********************************************************************
* project    :  CMS													*
* module     :  dfue                                                *
* components :                                                      *
* file       :  timeproc.c                                          *
* author     :  A.Knobelspies                                       *
* date       :  1990                                                *
* version    :  1.1                                                 *
* description:  Verschiedene zeitfunktionen für dfue-Modul          *
* compiler   :  cc                                                  *
*********************************************************************
*                        u p d a t e                                *
* date       : 2.09.93                                              *
* author     : Kordecki                                             *
* description: Die Zeitzone wird nicht mehr berücksichtigt, weil sie*
*				zu falschen Anrufzeiten fürt, wenn Anrufzeit zwi-	*
*				schen 0 und 1 Uhr bestimmt ist.                     *
* date       : 09.95                                              *
* author     : Kordecki                                             *
* description: datetime2sec-> Umwandlung von Datum und Urzeit  in die *
*				anzahl sekunden  *
* date       : 2.09.98                                              *
* author     : Kordecki                                             *
* description: Funktionen heute, heute_de, heute4abrech
* date       : 2.09.98                                              *
* author     : Kordecki                                             *
* description: Mehrere Funktionen: für Jahr,Monat,Tag,Stunde,Minute	*
*				Sekunde,Tag des Jahres, Tag der Woche				*
* date       : 2.09.98                                              *
* author     : Kordecki                                             *
* description: GetDaysOfPeriod- neue Funktion						*
*********************************************************************
SRC_END*/

#include <sys/types.h>
#include <sys/timeb.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

//#include	<system_def.h>
#include	"h/sla_type.h"

/*************************** DEFINES    ******************************/
#define _ZEIT_TEST
#define _MAIN_ZEIT_TEST

#define DATE_CALENDER_L		10 + 1

/*************************** GLOBAL     ******************************/
/**e*/
int     year ;
/**e*/
int     month;
/**e*/
int     day ;
/**e*/
int     hour;
/**e*/
int     minute  ;
/**e*/
int     second  ;
/**e*/
int     week_d ;
/**e*/
int     year_d ;
/**e*/
int     daylight;
/**e*/
short 	zeit_hhmm;
/**e*/
char	** DateArray;
extern long timezone;
typedef	struct
	{
	int		nr;
	char *	format;
	}DATEFORMAT;

#define		MAXDATEFORMAT	6
DATEFORMAT	format_array[MAXDATEFORMAT]={
		0,"dd.mm.yyyy",1,"dd/mm/yyyy",2,"ddmmyyyy",3,"yyyymmdd",4,"yyyy-mm-dd",5,"mm-dd-yyyy" };

	
/*************************** PROTOTYPES ******************************/


/*************************** FUNCTIONS  ******************************/
/**f*/
double currtime()
{
char	buf[30];
struct timeval tv;
struct timezone tz;
gettimeofday(&tv, &tz );
sprintf(buf,"%ld.%ld",tv.tv_sec, tv.tv_usec);
fprintf(stderr,"%s\n",buf);
return(atof(buf));
}

/**f*/
char * hole_env()
{
static char path[BUFSIZ];

strcpy(path, (char *)getenv("HOME"));
if ( path == NULL)
  {
  fprintf(stderr,"HOME Variable nicht definiert.\n");
  return((char *) NULL);
  }

return(path);
}

/*FUN_ANF 
*********************************************************************
* function   : get_act_datetime()                                   *
* description: Get current date and time data into global declared  *
*				variable											*
* external   :                                                      *
* return     :                                                      *
* error      :                                                      *
* sample     :                                                      *
*********************************************************************
FUN_END*/

/**f*/
void get_act_datetime()
{
struct tm * ptr_tm;
long secs_now;

time(&secs_now);
ptr_tm  = localtime(&secs_now);
year    = ptr_tm->tm_year + 1900 ;
month   = ptr_tm->tm_mon + 1;
day     = ptr_tm->tm_mday;
hour    = ptr_tm->tm_hour;
minute  = ptr_tm->tm_min;
second  = ptr_tm->tm_sec;
week_d = ptr_tm->tm_wday;
year_d = ptr_tm->tm_yday + 1;
daylight=ptr_tm->tm_isdst;
}
/*FUN_ANF 
*********************************************************************
* function   :heute ()                                              *
* description:Write current Date-String                             *
* external   :                                                      *
* return     :Format dd/mm/yyyy                                     *
* error      :                                                      *
* sample     :                                                      *
*********************************************************************
FUN_END*/
/**f*/
char * heute ()
{
long    secs_now;
static char DateStr[13];
struct tm *t;
 
time( &secs_now);
t = (struct tm *)localtime(&secs_now);
sprintf(DateStr,"%02d/%02d/%04d",t->tm_mday, t->tm_mon + 1, 1900+t->tm_year);
return(DateStr);
}
/*FUN_ANF 
*********************************************************************
* function   :timestamp ()                                          *
* description:Write current Date-String                             *
* external   :                                                      *
* return     :Format yyyy-mm-dd HH:MM:SS                            *
* error      :                                                      *
* sample     :                                                      *
*********************************************************************
FUN_END*/
/**f*/
char * timestamp()
{
long    secs_now;
static char Timestamp[20];
struct tm *t;
 
time( &secs_now);
t = (struct tm *)localtime(&secs_now);
sprintf(Timestamp,"%04d-%02d-%02d %02d:%02d:%02d",1900+t->tm_year, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

return(Timestamp);
}
/*FUN_ANF 
*********************************************************************
* function   :heute_de()                                            *
* description:Write current Date-String                             *
* external   :                                                      *
* return     :Format dd.mm.yyyy                                     *
* error      :                                                      *
* sample     :                                                      *
*********************************************************************
FUN_END*/

/**f*/
char * heute_de ()
{
long    secs_now;
static char DateStr[13];
struct tm *t;
 
time( &secs_now);
t = (struct tm *)localtime(&secs_now);
sprintf(DateStr,"%02d.%02d.%04d",t->tm_mday, t->tm_mon + 1, 1900 + t->tm_year);
return(DateStr);
}
/*FUN_ANF 
*********************************************************************
* function   :heute4abrech()                                        *
* description:Write current Date-String                             *
* external   :                                                      *
* return     :Format ddmmyyyy                                       *
* function   :                                                      *
* description:                                                      *
* external   :                                                      *
* return     :                                                      *
* error      :                                                      *
* sample     :                                                      *
*********************************************************************
FUN_END*/

/**f*/
char * heute4abrech ()
{
long    secs_now;
static char DateStr[9];
struct tm *t;
 
time( &secs_now);
t = (struct tm *)localtime(&secs_now);
sprintf(DateStr,"%02d%02d%04d",t->tm_mday, t->tm_mon + 1, t->tm_year);
return(DateStr);
}

/*FUN_ANF
************************************************************************
* function   : Year()                                      		       *
* description: Gets the actual year							           * 
* external   : get_act_datetime()                                      *
* return     : String in form YYYY	    								*	 
* error      :                                                         *
* sample     : 															*
************************************************************************
FUN_END*/

/**f*/
char * Year()
{
static char DateStr[7];
get_act_datetime();
sprintf(DateStr,"%04d",year);
return(DateStr);
}
/*FUN_ANF
************************************************************************
* function   : Month()                                      		       *
* description: Gets the actual month							           * 
* external   : get_act_datetime()                                      *
* return     : String in form MM										*	 
* error      :                                                         *
* sample     : 															*
************************************************************************
FUN_END*/

/**f*/
char * Month()
{
static char DateStr[7];
get_act_datetime();
sprintf(DateStr,"%02d",month);
return(DateStr);
}
/*FUN_ANF
************************************************************************
* function   : Day()                                      		       *
* description: Gets the actual day							           * 
* external   : get_act_datetime()                                      *
* return     : String in form dd										*	 
* error      :                                                         *
* sample     : 															*
************************************************************************
FUN_END*/

/**f*/
char * Day()
{
static char DateStr[7];
get_act_datetime();
sprintf(DateStr,"%02d",day);
return(DateStr);
}
/*FUN_ANF
************************************************************************
* function   : Hour()                                      		       *
* description: Gets the actual Hour							           * 
* external   : get_act_datetime()                                      *
* return     : String in form HH										*	 
* error      :                                                         *
* sample     : 															*
************************************************************************
FUN_END*/

/**f*/
char * Hour()
{
static char DateStr[7];
get_act_datetime();
sprintf(DateStr,"%02d",hour);
return(DateStr);
}
/*FUN_ANF
************************************************************************
* function   : Min()                                      		       *
* description: Gets the actual Min							           * 
* external   : get_act_datetime()                                      *
* return     : String in form MM										*	 
* error      :                                                         *
* sample     : 															*
************************************************************************
FUN_END*/

/**f*/
char * Min()
{
static char DateStr[7];
get_act_datetime();
sprintf(DateStr,"%02d",minute);
return(DateStr);
}
/*FUN_ANF
************************************************************************
* function   : Sec()                                      		       *
* description: Gets the actual number of sec				           * 
* external   : get_act_datetime()                                      *
* return     : String in form SS										*	 
* error      :                                                         *
* sample     : 															*
************************************************************************
FUN_END*/

/**f*/
char * Sec()
{
static char DateStr[7];
get_act_datetime();
sprintf(DateStr,"%02d",second);
return(DateStr);
}
/*FUN_ANF
************************************************************************
* function   : Week_day()                                      		       *
* description: Gets the number of days since Sunday			           * 
* external   : get_act_datetime()                                      *
* return     : String in form D										*	 
* error      :                                                         *
* sample     : 															*
************************************************************************
FUN_END*/

/**f*/
char * Week_day()
{
static char DateStr[7];
get_act_datetime();
sprintf(DateStr,"%d",week_d);
return(DateStr);
}
/*FUN_ANF
************************************************************************
* function   : Year_day()                                      		       *
* description: Gets the number of days since 31 December	           * 
* external   : get_act_datetime()                                      *
* return     : String in form DDD										*	 
* error      :                                                         *
* sample     : 															*
************************************************************************
FUN_END*/

/**f*/
char * Year_day()
{
static char DateStr[7];
get_act_datetime();
sprintf(DateStr,"%03d",year_d);
return(DateStr);
}
/*FUN_ANF
************************************************************************
* function   : Daylight()                                      		   *
* description: Gets the flag for alternate daylight savings time		* 
* external   : get_act_datetime()                                      *
* return     : String in form YYYY										*	 
* error      :                                                         *
* sample     : 															*
************************************************************************
FUN_END*/

/**f*/
char * Daylight()
{
static char DateStr[7];
get_act_datetime();
sprintf(DateStr,"%d",daylight);
return(DateStr);
}




/*FUN_ANF 
*********************************************************************
* function   :secs2date(long secs_now)                              *
* description:Convert number of sec into date and time              *
* external   :                                                      *
* return     :Format YYYYMMDDhhmmss                                 *
* error      :                                                      *
* sample     :                                                      *
*********************************************************************
FUN_END*/

/**f*/
char *secs2date(long secs_now)
{
static char DateStr[15];
struct tm *t;

t = (struct tm *)localtime(&secs_now);
sprintf(DateStr,"%04d%02d%02d%02d%02d%02d",t->tm_year+1900,
                                           t->tm_mon + 1,
                                           t->tm_mday,
                                           t->tm_hour,
                                           t->tm_min,
                                           t->tm_sec);
return DateStr;
}

/*FUN_ANF 
*********************************************************************
* function   :TermDatum(long secs_now)                              *
* description:Convert number of sec into date and time              *
* external   :                                                      *
* return     :Format YYMMDDhhmmss                                 *
* error      :                                                      *
* sample     :                                                      *
*********************************************************************
FUN_END*/

/**f*/
int TermDatum(long delta_sec,char * Datum)
{
long secs_now;
char	datum[15];
memset(datum,0,sizeof(datum));
time(&secs_now);
secs_now += delta_sec;	/* Diferrenz wegen Zeitzone addieren */
strcpy(datum,secs2date(secs_now));
strcpy(Datum,&datum[2]);
return(SUCCESS);
}
/*FUN_ANF 
*********************************************************************
* function   :Datum(long secs_now)                                  *
* description:Convert number of sec into date and time string       *
* external   :                                                      *
* return     :Format YYYYMMDDhhmmss                                 *
* function   :                                                      *
* description:                                                      *
* external   :                                                      *
* return     :                                                      *
* error      :                                                      *
* sample     :                                                      *
*********************************************************************
FUN_END*/

/**f*/
char *Datum(long delta_sec)
{
long secs_now;

time(&secs_now);
secs_now += delta_sec;	/* Diferrenz wegen Zeitzone addieren */
return(secs2date(secs_now));
}
/*FUN_ANF 
*********************************************************************
* function   :rel_hhmm(int hh, int mm)                              *
* description:Convert number of hours and minuts into number of     *
*				seconds                                             *
* external   :                                                      *
* return     :                                                      *
* error      :                                                      *
* sample     :                                                      *
*********************************************************************
FUN_END*/


/**f*/
long rel_hhmm(int hh, int mm)
{
return((((long)(hh)) * 3600L + ((long)(mm)) * 60L));
}

/*FUN_ANF 
*********************************************************************
* function   :act_time()                                            *
* description:Get the current time us long value                    *
* external   :                                                      *
* return     : HHMM                                                     *
* error      :                                                      *
* sample     :                                                      *
*********************************************************************
FUN_END*/

/**f*/
long act_time()
{
long Zeit;
struct tm *t;

time(&Zeit);
t=(struct tm *)localtime(&Zeit);
/*Zeit += timezone;  knall die Zeitzone drauf */
zeit_hhmm = (t->tm_hour * 100) + t->tm_min ;
return(Zeit);
}
/*FUN_ANF
************************************************************************
* function   : act_time_hhmm()                                              *
* description: Gets the actual time from the system clock              * 
* external   : localtime()                                             *
* return     : short int in form "hhmm"                                * 
* error      :                                                         *
* sample     : short_int = act_time_hhmm()                                  *
************************************************************************
FUN_END*/

/**f*/
short act_time_hhmm(void)
{
long systime, time();
struct tm *tr, *localtime();

  time(&systime);
  tr = localtime(&systime);
  return((short)(tr->tm_hour * 100) + tr->tm_min );
}
/*FUN_ANF 
*********************************************************************
* function   :Date_ddmmyy()                                         *
* description: Get current Date-String                              *
* external   :                                                      *
* return     :YYYYMMDD                                              *
* error      :                                                      *
* sample     :                                                      *
*********************************************************************
FUN_END*/

/**f*/
char * Date_ddmmyy()
{
long Zeit;
static char DateStr[9];
struct tm *t;

time(&Zeit);
t = (struct tm *)localtime(&Zeit);
sprintf(DateStr,"%02d%02d%02d", t->tm_mday, t->tm_mon + 1,1900+t->tm_year-2000);
//sprintf(DateStr,"%02d%02d%02d",1900+t->tm_year, t->tm_mon + 1, t->tm_mday);
return(DateStr);
}
/*FUN_ANF 
*********************************************************************
* function   :Datums()                                              *
* description: Get current Date-String                              *
* external   :                                                      *
* return     :YYYYMMDD                                              *
* error      :                                                      *
* sample     :                                                      *
*********************************************************************
FUN_END*/

/**f*/
char * Datums()
{
long Zeit;
static char DateStr[9];
struct tm *t;

time(&Zeit);
t = (struct tm *)localtime(&Zeit);
sprintf(DateStr,"%04d%02d%02d",1900+t->tm_year, t->tm_mon + 1, t->tm_mday);
return(DateStr);
}
/*FUN_ANF 
*********************************************************************
* function   :Date_dd_mm_yy()                                              *
* description: Get current Date-String                              *
* external   :                                                      *
* return     :DD/MM/YY                                              *
* error      :                                                      *
* sample     :                                                      *
*********************************************************************
FUN_END*/

/**f*/
char * Date_dd_mm_yy()
{
long Zeit;
static char DateStr[9];
struct tm *t;

time(&Zeit);
t = (struct tm *)localtime(&Zeit);
sprintf(DateStr,"%02d/%02d/%02d",t->tm_mday, t->tm_mon + 1, t->tm_year -100);
return(DateStr);
}
/*FUN_ANF 
*********************************************************************
* function   :Times()                                               *
* description:Get current Time-String                               *
* external   :                                                      *
* return     :HHMMSS                                                *
* error      :                                                      *
* sample     :                                                      *
*********************************************************************
FUN_END*/


/**f*/
char * Times()
{
long Zeit;
static char ZeitStr[13];
struct tm *t;

time(&Zeit);
t = (struct tm *)localtime(&Zeit);
sprintf(ZeitStr,"%02d%02d%02d",t->tm_hour,
                               t->tm_min,
                               t->tm_sec);

return(ZeitStr);
}
/*FUN_ANF 
*********************************************************************
* function   :abs_hhmm(int hh, int mm)                              *
* description:                                                      *
* external   :                                                      *
* return     :                                                      *
* error      :                                                      *
* sample     :                                                      *
*********************************************************************
FUN_END*/


/**f*/
long abs_hhmm(int hh, int mm)
{
long Zeit;
time(&Zeit);
localtime(&Zeit);
Zeit /= 86400L; /* strip alle Sekunden dieses Tages ab */
Zeit *= 86400L;
/*
Wegen falschen Anrufzeiten ist die timezone ab sofort weg. (2.09.93)
Zeit += timezone;*/ /* knall die Zeitzone drauf */

/*
	Errechne Zeitpunkt des Anrufes fuer diesen Tag
*/
Zeit += (((long)(hh)) * 3600L + ((long)(mm)) * 60L); 

/*
printf("ABS_HHMM\n");
printf("Zeit %ld\n",Zeit);
printf("Zeit nach teilen %ld\n",Zeit);
printf("Zeit nach multiplizieren %ld\n",Zeit);
printf("Timezone %ld \n",timezone);
printf("Zeit nach  addieren der zeitzone %ld\n",Zeit);
printf("Zeit nach  addieren den Eingabeparameter %ld\n",Zeit);
*/

return (Zeit);
}
/*FUN_ANF 
*********************************************************************
* function   :                                                      *
* description:                                                      *
* external   :                                                      *
* return     :                                                      *
* error      :                                                      *
* sample     :                                                      *
*********************************************************************
FUN_END*/


/**f*/
int add_hhmm(int hm1, int hm2)
{
long x;
int rh;
int rm;

x = rel_hhmm(hm1 / 100, hm1 % 100) + rel_hhmm(hm2 / 100, hm2 % 100);
x = (x % 86400L) / 60L; /* Berechne tatsaechliche Minuten */
rh = (int)(x / 60L);    /* Berechne Stunden */
rm = (int)(x % 60L);    /* Berechne Minuten */
return ((rh*100) + rm);
}

/*FUN_ANF 
*********************************************************************
* function   :datetime2sec(char * Datum, long  Zeit)                *
* description:Convert current Date and Time into number of second   *
* external   :                                                      *
* return     :                                                      *
* error      :                                                      *
* sample     :                                                      *
*********************************************************************
FUN_END*/

/**f*/
long	datetime2sec(char * Datum, long  Zeit)
{
long 	sec;
struct tm t;
char	*p;

t.tm_sec =  Zeit % 100 ;
t.tm_hour= Zeit / 10000;
t.tm_min = ( Zeit % 10000 ) / 100;

p=Datum;
while (*p)
	{
	if(*p == '.')
		*p='/';
	p++;
	}

sscanf( Datum,"%d/%d/%d/", &t.tm_mday, &t.tm_mon , &t.tm_year );

if(t.tm_year <  1970 && t.tm_year >  99 )
	{
#ifdef MAIN_ZEIT_TEST
fprintf(stderr,"Year %d is unacceptable !\n");
#endif
	return(-1);
	}

if(t.tm_year <  70 )
	t.tm_year+=100;

/* years since 1900 */
if(t.tm_year >  1900 )
	t.tm_year -= 1900;

/*
 tm_year of the tm structure must be for year 1970 or  later.
     Calendar times before 00:00:00 UTC, January 1, 1970 or after
     03:14:07 UTC, January 19, 2038 cannot be represented.
*/


/* months since January [0, 11] */
t.tm_mon -= 1 ;

/* seconds after the minute [0, 61]  */
t.tm_sec += 1 ;

#ifdef  ZEIT_TEST
fprintf(stderr,"DATUM=%s ZEIT %ld\n", Datum,Zeit);
fprintf(stderr,"Jahr %d \n",t.tm_year);
fprintf(stderr,"Monat %d \n",t.tm_mon);
fprintf(stderr,"Tag %d \n",t.tm_mday);
fprintf(stderr,"Stunde %d \n",t.tm_hour);
fprintf(stderr,"Min %d \n",t.tm_min);
fprintf(stderr,"Sec %d \n",t.tm_sec);
#endif

if ((sec=mktime(&t))== -1)
	perror("mktime");
#ifdef ZEIT_TEST
/*
t.tm_sec =  0 ;
t.tm_hour= 0;
t.tm_min = 0; 
t.tm_mday=0;
t.tm_mon=0;
t.tm_year=0; 
*/
fprintf(stderr,"%s\n",asctime( & t));
#endif
return(sec);
}


/**f*/
int isdate(char * date,char * format )
{
char *p,*pf;
int	i,format_found=0;
short	daysdigit=0;
short	monthsdigit=0;

for( i=0; i<MAXDATEFORMAT; i++ )
	{
	if(strcmp(format, format_array[i].format) == SUCCESS )
		{
		format_found=1;
		break;
		}
	}
if(!format_found)
	{
	fprintf(stderr,"Error. Unknown date-format %s\n",format);
	return(ERROR);
	}
p=date;
pf=format;
while( *p)
	{
	if( (*pf=='d' || *pf=='m' || *pf=='y'))
		{
		if(!isdigit(*p))
			{
			fprintf(stderr,"Error. Character '%c' is not a digit.\n",*p);
			return(ERROR);
			}
		else
			{
			if(*pf=='d')
				{
				daysdigit++;
				if(daysdigit==1)
					{
					if(*p>='0' && *p <='3')
						;
					else
						{
						fprintf(stderr,"Error.Character '%c' is not allowed for %hd position of day.\n",*p,daysdigit);
						return(ERROR);
						}
					}
				if(daysdigit==2)
					{
					if(*p>='0' && *p <='9')
						;
					else
						{
						fprintf(stderr,"Error.Character '%c' is not allowed for %hd position of day.\n",*p,daysdigit);
						return(ERROR);
						}
					}
				}
			if(*pf=='m')
				{
				monthsdigit++;	
				if(monthsdigit==1)
					{
					if(*p>='0' && *p <='2')
						;
					else
						{
						fprintf(stderr,"Error.Character '%c' is not allowed for %hd position of month.\n",*p,monthsdigit);
						return(ERROR);
						}
					}
				if(monthsdigit==2)
					{
					if(*p>='0' && *p <='9')
						;
					else
						{
						fprintf(stderr,"Error.Character '%c' is not allowed for %hd position of month.\n",*p,monthsdigit);
						return(ERROR);
						}
					}
				}
			if( *pf=='.' || *pf=='/' || *pf=='-' || *pf==' ')
				{
				if(*p==*pf)
					;
				else
					{
					fprintf(stderr,"Error.Character '%c' is not a '%c'\n",*p,*pf);
					return(ERROR);
					}
				}
			}
		}
	pf++;
	p++;
	}
return(SUCCESS);
}





/**f*/
int change_date_format(char * from_date,char * from_format,
 char * to_date, char * to_format)
{
char *p,*pf;
int i,format_found=0;
short   daysdigit=0;
short   monthsdigit=0;
int		to_format_nr;
int		from_format_nr;
char	days[3];
char	months[3];
char	years[5];


if(isdate(from_date,from_format)!= SUCCESS)
	return(ERROR);

memset(years,0,5);
memset(months,0,3);
memset(days,0,3);

for( i=0; i<MAXDATEFORMAT; i++ )
    {
    if(strcmp(from_format, format_array[i].format) == SUCCESS )
        {
		from_format_nr=format_array[i].nr;
        format_found=1;
        break;
        }
    }
if(!format_found)
    {
    fprintf(stderr,"Error. Unknown date-format %s\n",from_format);
    return(ERROR);
    }
format_found=0;

for( i=0; i<MAXDATEFORMAT; i++ )
    {
    if(strcmp(to_format, format_array[i].format) == SUCCESS )
        {
		to_format_nr=format_array[i].nr;
        format_found=1;
        break;
        }
    }
if(!format_found)
    {
    fprintf(stderr,"Error. Unknown date-format %s\n",to_format);
    return(ERROR);
    }

if(to_format_nr==0)
	{
	if(from_format_nr==3)
		{
		strncpy(years,from_date,4);
		strncpy(months,&from_date[4],2);
		strncpy(days,&from_date[6],2);
		}
	if(from_format_nr==4)
		{
		strncat(years,from_date,4);
		strncpy(months,&from_date[5],2);
		strncpy(days,&from_date[8],2);
		}
	if(from_format_nr==5)
		{
		strncpy(months,from_date,2);
		strncpy(days,&from_date[3],2);
		strncat(years,&from_date[6],4);
		}
	strcpy(to_date,days);
	strcat(to_date,".");
	strcat(to_date,months);
	strcat(to_date,".");
	strcat(to_date,years);
	to_date[10]=0;
	
	}



return(SUCCESS);
}



/*FUN_ANF 
*********************************************************************
* function   :GetDaysOfPeriod                                       *
* description:Function get the array of Calender-Dates between      *
*				From-date and To-date into Ptr-Address.             *
*				The last parameter contain a numer of entries in 	*
*				Data-Array											*
* external   :                                                      *
* return     :                                                      *
* error      :                                                      *
* sample     :                                                      *
	GetDaysOfPeriod("18/12/2000", "18/03/2001", &ptr_arr, &maxidx) 
*********************************************************************
FUN_END*/
/**f*/
int GetDaysOfPeriod(char * from, char * to, char *** ptr , long * index)
{
long 	sec_from;
long	sec_to;
long 	sec;
char	datum[15];
char	day[3];
char	month[3];
char	year[5];
long	i,imax;


sec_from=datetime2sec(from , 0);
sec_to=datetime2sec(to , 0);

#ifdef TEST
fprintf(stderr,"FROM:%ld\n",sec_from);
fprintf(stderr,"TO:%ld\n",sec_to);
#endif

if(sec_to < sec_from)
	{
	perror("GetDaysOfPeriod:From-Date is less as To-Date");
	return(ERROR);
	}

for (sec = sec_from, i=0; sec <=(sec_to + 86399) ; sec+=86400, i++ )
	{
	strcpy(datum,secs2date(sec));
	strncpy(year,datum,4);
	year[4]=0;
	strncpy(month,&datum[4],2);
	month[2]=0;
	strncpy(day,&datum[6],2);
	day[2]=0;
	memset((void *) datum, 0, (size_t) sizeof(datum));
	sprintf(datum,"%s.%s.%s",day,month,year);

	if((DateArray=realloc(DateArray,(i+1)* sizeof(char *) )) == (void *) NULL )
		{
		perror("GetDaysOfPeriod:realloc: no more available memory");
		return(ERROR);
		}
	if((DateArray[i]=malloc(DATE_CALENDER_L)) == (void *) NULL )
		{
		perror("GetDaysOfPeriod:malloc: no more available memory");
		return(ERROR);
		}
	memcpy(DateArray[i],datum,DATE_CALENDER_L);
#ifdef TEST
	fprintf(stderr,"A[%ld]=%s	%ld\n",i,DateArray[i], &DateArray[i]);
#endif
	}
*index=i;

#ifdef TEST
imax=i;
for (i=0;i < *index ; i++)
	fprintf(stderr,"NA[%ld]=%s	%ld\n",i,DateArray[i], &DateArray[i]);
#endif

*ptr=DateArray; 	/* Give back the adress of array of date	*/

return(SUCCESS);
}

/*FUN_ANF 
*********************************************************************
* function   :CompDate2Today(char * timestamp)							*
* description:Function compare timestamp ( in parameter ) with the 		*
*				current date-time. The parameter have to in format	*
*				"YYYY-MM-DD HH:MM:SS". This function don't check 	*
*				the regularity of format of the timestamp.						* 
* external   :                                                      *
* return     :The function returns an integer greater than, equal  to,*
*				or less  than  0, if the timestamp is greater		*
*     			than, equal to, or less than  current.				*	
* error      :                                                      *
* sample     : CompDate2Today("2002-01-03 17:31:00");
*********************************************************************
FUN_END*/
/**f*/
int CompDate2Today(char * date)
{
char	today[20];
int	res;

strcpy(today,timestamp());
res=strcmp(date,today);
#ifdef TEST
if(res>0)
	fprintf(stderr,"Date:%s is greather as today(%s)\n",date,today);
if(res<0)
	fprintf(stderr,"Date:%s is less as today(%s)\n",date,today);
if(res==0)
	fprintf(stderr,"Date:%s is equal as today(%s)\n",date,today);
#endif
return(res);
}
/*FUN_ANF 
*********************************************************************
* function   :timeofday(void)
* description:Function give back current datetime with microsec together*
*		in format "YYYYMMDDHHMMSSuuuuuu"
* external   :                                                      *
* return     :The function returns an string in format 			*
*					"YYYYMMDDHHMMSSuuuuuu 		*
* error      :                                                      *
* sample     :  :timeofday()						*
*********************************************************************
FUN_END*/
/**f*/
char * timeofday(void)
{
static char timestring[30];
struct timeval tv;
struct timezone tz;
long sec;
long usec;
char s[20];

gettimeofday( &tv,  &tz);
sec=tv.tv_sec;
usec=tv.tv_usec;

//fprintf(stderr,"Current %lds:%ldusec\n",sec, usec);
//fprintf(stderr,"Current %s s:%ld\n",secs2date(sec), usec);
strcpy(timestring,secs2date(sec));
sprintf(s,"%06ld",usec);
strcat(timestring,s);

return(timestring);
}

#ifdef MAIN_ZEIT_TEST
main()
{
int retry_cnt;
long nextcall;
long actzeit;
int terms_calltime;
long sec;
char	datum[11];
long	zeit;
char	* ptr;
long	i;
long	maxidx;
char	**ptr_arr;
long 	ptr_l;

change_date_format("20001122","yyyymmdd",datum,"dd.mm.yyyy");
printf("datum=%s\n",datum);

/*
printf("Date:\n");
scanf("%11s",datum);
printf("Time:\n");
scanf("%ld",&zeit);

sec=datetime2sec(datum,zeit);
printf("...Is %ld s \n",sec);
printf("Seconds:\n");
scanf("%ld",&sec);
printf("DateTime=%s\n",secs2date(sec));
*/
/*
for (retry_cnt = 0; retry_cnt < 2; retry_cnt++)
  {
  printf("Counter %d\n", retry_cnt);
  nextcall = abs_hhmm(00,05) + (rel_hhmm(24,00) * retry_cnt);
  printf("nextcall :%ld => Datum:%s\n",nextcall, Datum());
  time(&actzeit);
  if (nextcall < actzeit)
    {
    printf("Jetzt anrufen.\n\n");
    }
  }
terms_calltime=add_hhmm(5,2400);
printf("call_time=%d\n",terms_calltime);
printf("act_time=%ld\n",act_time());
printf("actzeit=%ld\n",actzeit);
ptr_arr=(char **) NULL;
if( GetDaysOfPeriod("18/12/2000", "18/03/2001", &ptr_arr, &maxidx) == ERROR)	
	{
	fprintf(stderr,"ERROR\n");
	return(ERROR);
	}

for(i=0;i<maxidx;i++,ptr_arr++)
	{
	fprintf(stderr,"ptr=%s %ld\n",*ptr_arr, ptr_arr);
	}
free(DateArray);
*/
exit(0);
}
#endif
