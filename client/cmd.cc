// 
//   Copyright (C) 2005 Free Software Foundation, Inc.
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

// This is generated by autoconf
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdarg.h>
#include <signal.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <cstdio>
#include <iostream>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <libxml/debugXML.h>

#include "dejagnu.h"
#include "tcpip.h"
#include "console.h"
#include "msgs.h"
#include "xml.h"

using namespace std;
extern LogFile dbglogfile;
static void usage (const char *);

const int INBUFSIZE = 1024;
int
main(int argc, char *argv[])
{
    int         c;
    string      filespec;
    bool        client;
    bool        daemon;
    string      hostname;
    string      user;
    string      passwd;
    char        *ptr;
    retcode_t   ret;
    
    client = true;
    daemon = false;
    hostname = "localhost";
    
    while ((c = getopt (argc, argv, "hvdcm:u:p:")) != -1) {
      switch (c) {
      case 'h':
        usage (argv[0]);
        break;
        
      case 'v':
        dbglogfile.set_verbosity();
        break;
        
      case 'm':
        hostname = strdup(optarg);
        break;
        
        // Specify database user name.
      case 'u': 
        user = strdup(optarg);
        break;
        
        // Specify database user password.
      case 'p':
        passwd = strdup(optarg);
        break;   
        
      case 'c':
        client = true;
        daemon = false;
        break;   
        
      case 'd':
        client = false;
        daemon = true;
        break;   
        
      default:
        usage (argv[0]);
        break;
      }
    }
    
    // get the file name from the command line
    if (optind < argc) {
        filespec = argv[optind];
        dbglogfile << "Will use \"" << filespec << "\" for test " << endl;
    }

    //    Tcpip tcpip;
    Console con;
    int ch;

    //    tcpip.toggleDebug(true);

    // Open a console for user input
    con.Open();

    char *buffer;
      
    buffer = (char *)new char[INBUFSIZE];
    memset(buffer, 0, INBUFSIZE);

#if 0
    cerr << "\rWelcome " << tcpip.remoteName()
         << " at IP address: " << tcpip.remoteIP() << endl;
    cerr << msg.status((meter_data_t *)0) << endl;
#endif    

    Msgs msg;
    msg.toggleDebug(true);

    // Make a client connection
    if (client == true) {
      msg.init(hostname);
    }

    // Start as a daemon
    if (daemon == true) {
      msg.init(true);
    }
    //msg.methodsDump();          // FIXME: debugging crap
    
    //msg.print_msg(msg.status((meter_data_t *)0));

    if (client) {
      msg.writeNet(msg.metersRequestCreate(Msgs::BATTERY_VOLTS));
    }
    
#if 1
    if (client) {
      //sleep(1);
      msg.writeNet(msg.metersRequestCreate(Msgs::AC1_VOLTS_IN));
      //sleep(1);
      msg.writeNet(msg.metersRequestCreate(Msgs::CHARGE_AMPS));
      //sleep(1);
      msg.writeNet(msg.metersRequestCreate(Msgs::AC_LOAD_AMPS));
      //sleep(1);
      msg.writeNet(msg.metersRequestCreate(Msgs::PV_AMPS_IN));
      //sleep(1);
      msg.writeNet(msg.metersRequestCreate(Msgs::SELL_AMPS));
    }
#endif
    while ((ch = con.Getc()) != 'q') {
      if (ch > 0){                // If we have something, process it
        //con.Putc (ch);          // echo inputted character to screen
        
        switch (ch) {
          // Toggle the DTR state, which is as close as we get to
          // flow control.
        case 'Q':
        case 'q':
          con.Puts("Qutting PowerGuru due to user input!\n");
          msg.writeNet("quit");
          exit(0);
          break;
        case '?':
          con.Puts("PowerGuru client\n");
          con.Puts("\t? - help\r\n");
          con.Puts("\tq - Quit\r\n");
          con.Puts("\tQ - Quit\r\n");
          sleep(2);
        default:
          break;
        };
      }

      XML xml;
      unsigned int i;
      memset(buffer, 0, INBUFSIZE);
#if 1
      vector<const xmlChar *> messages;
      //const xmlChar *messages[200];
      ret = msg.anydata(messages);

      if (ret == ERROR) {
        dbglogfile << "ERROR: Got error from socket " << endl;
        // Start as a daemon
        if (daemon == true) {
          msg.closeNet();
          // wait for the next connection
          if (msg.newNetConnection(true)) {
            dbglogfile << "New connection started for remote client." << endl;
          }
        } else {
          if (errno != EAGAIN) {
            dbglogfile << "ERROR: " << errno << ":\t"<< strerror(errno) << endl;
            msg.closeNet();
            exit (-1);
          }
        }
      }
      for (i=0; i < messages.size(); i++) {
        dbglogfile << "Got message " << messages[i] << endl;
        string str = (const char *)messages[i];
        //delete messages[i];
        if (xml.parseXML(str) == ERROR) {
          continue;
        }
      }
      messages.clear();

#else
      ptr = buffer;
      int bytes = msg.readNet(buffer, INBUFSIZE, 0);      
      if (bytes > 0) {
        while (ptr != NULL) {
          if (ptr != buffer) {
            ptr++;
          }
          // We're at the end of the messages
          if (strlen(ptr) == 0) {
            break;
          }
          if (xml.parseXML(ptr) == false) {
            break;
          }
          if (strncmp(buffer, "quit", 4) == 0) {
            exit(0);
          }
          ptr = strchr(ptr, '\0');
        }
      }
#endif
    }
    //con.Close();
}

static void
usage (const char *prog)
{
    cerr << "This is a simple cmmand line for the PowerGuru daemon." << endl;
    cerr << "Usage: pguru: [h] filename" << endl;
    //    cerr << "Usage: " << prog << ": [h] filename" << endl;
    cerr << "-h\tHelp" << endl;
    cerr << "-v\tVerbose output" << endl;
    cerr << "-d\tDaemon Mode" << endl;
    cerr << "-c\tClient Mode (default)" << endl;
    cerr << "-m\tRemote Machine (localhost)" << endl;
    cerr << "-u\tRemote Machine user" << endl;
    cerr << "-p\tRemote Machine password" << endl;
    exit (-1);
}
