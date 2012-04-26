/*  AUTHOR: Arthur Tumanyan (arthurtumanyan@yahoo.com)
 *  
 *  Shared library that provide SQUID data logging into mysql database 
 *  This library is a part of 'ShagaEngine' open traffic counter and billing system
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
 *
 */

#include "libshaga.h" 

void _syslog(char *);

//
static int EX_H = 0;
static int hc = 0;
static int must_close_mhandle = 1;
/*
*
* NOTICE: getExcludeHosts() and isExHost() functions added  specially to provide 
*         correct work of 'ShagaEngine' open traffic counter 
* You can remove this functions 
* 
*/
int getExcludeHosts(){
        //
        snprintf(TMP_QUERY,LINE_MAXLEN,"%s",
                 "SELECT `hosts` FROM `ecl_hosts`");
        state = mysql_real_query(&mysql,TMP_QUERY,strlen(TMP_QUERY));
        //
        if(state)
        {
            snprintf(MYSQL_ERR,LINE_MAXLEN,"%s",(char *)mysql_error(&mysql));
        /* log about failure */    
	return 1;
        }
        result = mysql_store_result(&mysql);
        if (result)  // there are rows
        { 
	int c = 0;
            while((row = mysql_fetch_row(result)))
            {
                sscanf(row[0],"%s",excludeHosts[c].host);
		
		c++;
}
            mysql_free_result(result);
	hc = c;
            //
        }else
        {

 snprintf(MYSQL_ERR,LINE_MAXLEN,"Error getting records: %s",(char *) mysql_error(&mysql));
       /* log about failure */ 
        }
	return 0;
}

int isExHost(char *host){

int i,ret = 0;

if(strlen(host) < 7) return 0;
    for(i = 0;i < hc;i++)
    { 
	if(strncmp(host,excludeHosts[i].host,15) == 0)
        {
                ret = 1;
            break;
        }//strcmp
    }//for
    return ret;
}
//
//
void _syslog(char *log_string){
	openlog("ShagaCollector",LOG_NDELAY | LOG_PID,LOG_USER);
	syslog(LOG_INFO,log_string);
	closelog();
	}

//
int db_connect(char *dbhost,char *dbuser,char *dbpwd,char *dbname,unsigned int dbport){ 

    mysql_init(&mysql);
    connection = mysql_real_connect(&mysql,dbhost,dbuser,dbpwd,dbname,dbport,NULL,0);
    /* check for connection error */
    if(connection == NULL) {
        /* log the error message */
   /*     snprintf(MYSQL_ERR,LINE_MAXLEN,"%s\n",(char *)mysql_error(&mysql)); */
       /* log this */ 

        return 1;
    }

  return 0; 
}

//
int initShagaConnection(char *dbhost,char *dbuser,char *dbpwd,char *dbname,unsigned int dbport)
{


if ( db_connect(dbhost,dbuser,dbpwd,dbname,dbport) != 0)
 {
        snprintf(MYSQL_ERR,LINE_MAXLEN,"Cannot connect to database %s on %s\n", 
                                            dbname,dbhost);
	/* log this*/
	_syslog(MYSQL_ERR);
		must_close_mhandle = 0;
	return 1; 
}
	if(0 == getExcludeHosts()) { EX_H = 1; }

	return 0;
}
//
int writeToDB(char * __time,char * username,char * ip_address,char * byte){
	
  long int my_stamp = atol(__time);	
  time_t my_stamp2 = (time_t)my_stamp;

	thistime = localtime(&my_stamp2);
	strftime(timestr,25,"%F %T",thistime);
	int retval;
	
	snprintf(TMP_QUERY,LINE_MAXLEN,
		 "INSERT DELAYED INTO %s(`date`,`user`,`ip_addr`,`bytes`) VALUES(\'%s\',\'%s\',\'%s\',\'%s\')",
		TMP_TABLE,(char *) timestr,username,(char *)ip_address,(char *)byte);

if(EX_H == 1 && ( 0 == isExHost(ip_address) ) ){	
	state = mysql_real_query(&mysql,TMP_QUERY,strlen(TMP_QUERY));
	
}
	//
	if(state) {
		snprintf(MYSQL_ERR,LINE_MAXLEN,"%s\n",(char *)mysql_error(&mysql));
	 /* log this */	
	_syslog(MYSQL_ERR);
		return 1; 
	}
	int affected = (int)mysql_affected_rows(&mysql);
	if(affected > 0){
		retval = 0;
	}else{ 
		snprintf(MYSQL_ERR,LINE_MAXLEN,"%s\n",(char *)mysql_error(&mysql));
	/* log this */	
	_syslog(MYSQL_ERR);
		retval = 1;
	}
	return retval;
}
//
void mysqlClose(){ 

	if(must_close_mhandle == 1){ mysql_close(&mysql); }
}
//

