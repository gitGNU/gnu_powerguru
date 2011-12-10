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

#ifndef __DATABASE_H__
#define __DATABASE_H__

// This is generated by autoconf
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream> 
#include <string>
#include <list>
#include <iomanip> 
#include <vector>

//#include "database.h"

#ifdef HAVE_MYSQL
#include <mysql/errmsg.h>
#include <mysql/mysql.h>
#else
#error "You need to install MySQL!"
#endif

// This shuts up warnings about these constants not being used.
#pragma GCC system_header

static const char *DBTABLE = "powerguru";
static const char *DBNAME  = "powerguru";
static const char *DBUSER  = "powerguru";
static const char *DBPASS  = "powerguru";
static const char *DBHOST  = "localhost";

enum dbtype {NODB, DBDATABASE, DBPGSQL, DBODBC, DBGDBM, DBSTL};

typedef enum { NO_DEVICE, MX_OUTBACK, FX_OUTBACK, SW_XANTREX } device_t;

typedef struct
{
    int   unit;                   // The unit number for the device
    device_t type;                // The type of the device
    int   charge_amps;            // The amperage being put into the batteries
    int   ac_load_amps;           // The load in amps
    float battery_volts;          // Actual Battery Voltage
    float ac_volts_out;           // The voltage the inverter is producing
    float ac1_volts_in;           // Grid AC input
    float ac2_volts_in;           // Generator AC input. Xantrex only
    int   pv_amps_in;             // The current coming from the PV panels.
    float pv_volts_in;            // The DC voltage coming from the PV panels.
    float buy_amps;               // The AC current taken from the Grid
    // and used to charge the batteries; 
    float sell_amps;              // The AC current the batteries are
    // putting into the grid.
    float daily_kwh;              // The daily kilowatts put into the
    // batteries from the PV
    // source. Outback only.
    int   hertz;                  // Frequency in Hertz. Xantrex only
    float tempcomp_volts;         // Battery Voltage temperate
    // compensated. Xantrex Only.
  
}  meter_data_t ;

class Database
{
public:
    Database();
    ~Database();
  
    bool openDB ();
    bool closeDB ();
    bool queryInsert(std::vector<meter_data_t *> data);
    bool queryInsert(meter_data_t *data);
    bool queryInsert(const char *query);
    void *queryResults(const char *query);
    //bool deleteDB(etamsg_t *data);
    //bool insertDB(etamsg_t *data);
    char *gettime();

    // Accessors
    void dbUserSet(std::string user);
    void dbPasswdSet(std::string passwd);
    void dbNameSet(std::string name);
    void dbHostSet(std::string host);
private:
    enum {CLOSED, OPENED} state;
    dbtype          _dbtype;
    int             _dbport;
    std::string     _dbuser;
    std::string     _dbpasswd;
    std::string     _dbhost;
    std::string     _dbname;
    std::string     _tblname;
    MYSQL           *_connection;
    MYSQL           _mysql;
};

// __DATABASE_H__
#endif

// local Variables:
// mode: C++
// indent-tabs-mode: nil
// End:
