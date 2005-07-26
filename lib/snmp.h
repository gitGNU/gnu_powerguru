// 
//   Copyright (C) 2005 Free Software Foundation, Inc.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#ifndef __SNMP_H__
#define __SNMP_H__ 1

// This is generated by autoconf
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdarg.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>

/* If you have sys/time.h on your machine: */
#include <sys/time.h>

/* Else pick one of these instead:
#include <time.h>
#include <sys/timeb.h>
*/

/* if on windows: */
#ifdef WIN32
#include <winsock.h>
#endif

/* put a proper extern in front of certain functions */
#define NETSNMP_IMPORT extern

/* don't use inline constructs (define to <i>inline</i> if you want them) */
#define NETSNMP_INLINE

/* run man signal and see what the user function is supposed to return
   to avoid warnings, or leave as "void" below if you don't care about
   any warnings you might be seeing */
#define RETSIGTYPE void

/* tell the Net-SNMP core functionality headers that we've done the
   minimal requirements */
#define NET_SNMP_CONFIG_H

/* include the core Net-SNMP functionality headers */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/agent/scalar.h>

#include <string>
#include "err.h"

class SnmpClient
{
 public:
  SnmpClient();
  ~SnmpClient();
  retcode_t open(std::string init, std::string mibname);
  retcode_t close(void);
  struct snmp_pdu *read(std::string mibnode);
 private:
  struct snmp_session   _session;
  struct snmp_session   *_handle;
  struct tree           *_mibtree;
  std::string           *_hostname;
  struct snmp_pdu       *_pdu;
  struct snmp_pdu       *_response;
};


class SnmpDaemon
{
 public:
  SnmpDaemon();
  ~SnmpDaemon();
  retcode_t main(bool background);
  retcode_t start(std::string init, std::string mibname);
  retcode_t close(void);
  struct snmp_pdu *readSnmp(std::string mibnode);
  retcode_t process(void);
  
 private:
  std::string           *_hostname;
};


// end of __SNMP_H__
#endif