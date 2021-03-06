// 
// Copyright (C) 2005, 2006, 2007, 2008, 2009, 2010, 2011
//      Free Software Foundation, Inc.
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#ifndef __MSGS_H
#define __MSGS_H__

// This is generated by autoconf
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#ifdef __STDC_HOSTED__
#include <sstream>
#else
#include <strstream>
#endif

#include "database.h"
#include "log.h"
#include "err.h"
#include "xml.h"
#include "tcpip.h"

class Msgs : public Tcpip {
public:

    typedef enum {
        NONET,
        DAEMON,
        CLIENT
    } net_mode_e;

    typedef enum {
        CHARGE_AMPS,
        AC_LOAD_AMPS,
        BATTERY_VOLTS,
        AC_VOLTS_OUT,
        AC1_VOLTS_IN,
        AC2_VOLTS_IN,
        PV_AMPS_IN,
        PV_VOLTS_IN,
        BUY_AMPS,
        SELL_AMPS,
        DAILY_KWH,
        HERTZ,
        TEMPCOMP_VOLTS
    } xml_meters_e;

    typedef enum {
        SYSVERSION,
        REVISION,
        OPMODE,
        ERRORMODE,
        WARNINGMODE
    } xml_status_e;
  
    typedef enum {
        GENERATOR,
        GRID,
        RELAY,
        AUXILARY,
        POLL,
        INVERTER,
        CHARGER
    } xml_command_e;
  
    typedef enum {
        GENCONFIG,
        GRIDCONFIG,
        BUY,
        SELL,
        CHARGE
    } xml_config_e;

    typedef enum {
        METER,
        STATUS,
        COMMAND,
        CONFIG,
        HEARTBEAT
    } xml_msg_e;
  
    typedef retcode_t (Msgs::*methodPtr_t)(XMLNode *node);

//   struct msg_data 
//   {
//     FPtr ptr;
//     std::string name;
//   };

    Msgs();
    Msgs(std::string host, std::string ip);
    Msgs(Tcpip *tcpip);
    ~Msgs();

    retcode_t init(void);
    retcode_t init(net_mode_e mode);
    retcode_t init(std::string hostname);
    retcode_t init(net_mode_e mode, std::string hostname);
    retcode_t init(bool block);
    retcode_t init(net_mode_e mode, bool block);
  
    void dump(XMLNode *datain);
    void process(XMLNode *datain);
  
    // These parse incoming messages for the daemon
    retcode_t statusProcess(XMLNode *node);
    retcode_t powerguruProcess(XMLNode *node);
  
    retcode_t heloProcess(XMLNode *node);
    retcode_t serverProcess(XMLNode *node);
    retcode_t clientProcess(XMLNode *node);
  
    retcode_t configProcess(XMLNode *node);

    // These are all the tags associated with meters
    retcode_t metersProcess(XMLNode *node);
    retcode_t chargeAmpsProcess(XMLNode *node);
    retcode_t loadAmpsProcess(XMLNode *node);
    retcode_t pvAmpsProcess(XMLNode *node);
    retcode_t pvVoltsProcess(XMLNode *node);
    retcode_t dailyKwhProcess(XMLNode *node);
    retcode_t hertzProcess(XMLNode *node);
    retcode_t batteryVoltsProcess(XMLNode *node);
    retcode_t buyAmpsProcess(XMLNode *node);
    retcode_t sellAmpsProcess(XMLNode *node);
    retcode_t acVoltsOutProcess(XMLNode *node);
    retcode_t ac1InProcess(XMLNode *node);
    retcode_t ac2InProcess(XMLNode *node);

    retcode_t unimplementedProcess(XMLNode *node);

    // These are all the system commands
    retcode_t commandProcess(XMLNode *node);

    // These format client side messages to the daemon
    std::string statusCreate(meter_data_t *data);
  
    std::string heloCreate(float version);
  
    std::string configCreate(std::string tag, int value);
    std::string configCreate(std::string tag, float value);
    std::string metersRequestCreate(std::string str);
    std::string metersRequestCreate(xml_meters_e type);

    // This formats a response from the daemon to the client
    std::string metersResponseCreate(const xmlChar *tag, int val);
    std::string metersResponseCreate(const xmlChar *tag, float val);
    std::string metersResponseCreate(const xmlChar *tag, std::string);

    std::string responseCreate(xml_msg_e type, const xmlChar *tag, std::string);
  
    std::string requestCreate(xml_meters_e tag);
    std::string requestCreate(xml_status_e tag);
    std::string requestCreate(xml_config_e tag);
    std::string requestCreate(xml_command_e tag);

    std::string packet(void) { return _body.str(); }
    void print_msg(std::string msg);

    void methodSet(const xmlChar *name, methodPtr_t func);
    methodPtr_t methodGet(const xmlChar *name);
    retcode_t methodProcess(const xmlChar *name, XMLNode *node);
    void methodsDump(void);

    std::string cacheGet(const xmlChar *name);
    retcode_t cacheAdd(const xmlChar *name, std::string);
    void cacheDump(void);

    std::string thisIPGet(void) { return _thisip; };
    std::string remoteIPGet(void) { return _thishost; }; 
    std::string thisHostnameGet(void) { return _remoteip; };
    std::string remoteHostnameGet(void) { return _remotehost; };

    retcode_t findTag(std::string tag);
  
private:
    float               _version;
    static std::map<const xmlChar *, methodPtr_t> _methods;
    static std::map<const xmlChar *, std::string> _cache;
    static net_mode_e   _net_mode;
#ifdef __STDC_HOSTED__
    std::ostringstream  _body;
#else
    std::ostrstream     _body;
#endif
    std::string         _thisip;
    std::string         _thishost;
    std::string         _remoteip;
    std::string         _remotehost;
};

// end of __MSGS_H__
#endif

// local Variables:
// mode: C++
// indent-tabs-mode: nil
// End:
