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

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <mysql.h>
#include <sys/types.h>
#include <time.h>
#include <syslog.h>

#ifndef SHAGA_LIB_H
#define SHAGA_LIB_H

#define LINE_MAXLEN 512
#define TMP_TABLE "tmp_traff"
#define MAX_USER_COUNT 9999
//
char TMP_QUERY[LINE_MAXLEN];
char MYSQL_ERR[LINE_MAXLEN];
char timestr[25];
struct tm *thistime;
int u_count; /* users count */

char *db_host;
char *db_name;
char *db_user;
char *db_pwd;
unsigned int db_port;

struct {
	char host[16];
}excludeHosts[MAX_USER_COUNT];

MYSQL_RES *result;
MYSQL_ROW row;
MYSQL *connection, mysql;
int state;
//
#endif
