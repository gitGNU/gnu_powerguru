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

// This is generated by autoconf.
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <string>
#include <vector>
#include <stdlib.h>
#include <unistd.h>

#include <string>
#include <iostream>
#include <fstream>

#ifdef HAVE_GETOPT_H
#include <getopt.h>
#else
extern int optind;
extern char *optarg;
#endif

// local header files
#include "log.h"
#include "err.h"
#include "outbackpower.h"
#include "console.h"
#include "fakeuart.h"
#include "rc.h"

using namespace std;
using namespace rcinit;
using namespace outbackpower;

static void usage (const char *);
extern LogFile dbglogfile;
 
retcode_t sim_mx(const char *filespec);
retcode_t sim_fx(const char *filespec);
  
main(int argc, char *argv[])
{
    int c, i, ch, ret;
    char *filespec = 0;
    ErrCond Err;
    outback_type_t otype;
    
    if (argc == 1) {
        //usage(argv[0]);
    }

    // Load the database config variable so they can be overridden by
    // the command line arguments.
    RCinitFile config;
    config.load_files();
    if (config.deviceGet().size() > 0) {    
      filespec = (char *)config.deviceGet().c_str();
    }

    // Default to being an MX charge controller device
    otype = OUTBACK_MX;
    
    filespec = DEFAULT_UART;
    // Process the command line arguments.
    while ((c = getopt (argc, argv, "hvd:t:")) != -1) {
      switch (c) {
      case 'h':
	usage (argv[0]);
	break;

      case 'v':
        // verbosity++;
	dbglogfile.set_verbosity();
	dbglogfile << "Verbose output turned on" << endl;
	break;
        
      case 'd':
	filespec = strdup(optarg);
	break;

      case 't':
        if (memcmp(optarg, "mx", 2) == 0) {
          otype = OUTBACK_MX;
        } else if (strncmp(optarg, "fx", 2) == 0) {
          otype = OUTBACK_MX;
        } else {
          cerr << "ERROR: No Device Type specified, \"" << optarg << "\" is bogus!" << endl;
          exit(-1);
        }
        
	break;

      default:
	usage (argv[0]);
        break;
      }
    }

    // Say which device we're simulating
    if (otype == OUTBACK_MX) {
      dbglogfile << "Simulating a Outback MX60 charge controller" << endl;
      sim_mx(filespec);
    }
    if (otype == OUTBACK_FX) {
      dbglogfile << "Simulating a Outback FX Series Inverter" << endl;
      sim_fx(filespec);
    }

}

static void
usage (const char *prog)
{
    cerr <<"This program implements a command line interface" << endl; 
    cerr << "for an Outback Power Systems Device" << endl;
    cerr << "Usage: " << prog << " [hvd]" << endl;
    cerr << "\t-h\t\t\t\tHelp (this display)" << endl;
    cerr << "\t-v\t\t\t\tVerbose mode" << endl;
    cerr << "\t-d [filespec]\t\t\tSpecify Serial Port (default=/dev/ttyS0)" << endl;
    cerr << "\t-t [t]\t\t\t\tSpecify Device Type (default=mx)" << endl;
    
    exit (-1);
}

retcode_t
sim_mx(const char *filespec)
{
    struct stat stats;
    char *home;
    string loadfile;
    ErrCond Err;
    string line, newline;
    ifstream in;
    FakeUart fu;
    if (filespec == 0) {
        return ERROR;
    }
    
    fu.Open(&Err);
    
    // Check the users home directory
    home = getenv("HOME");
    if (home) {
        loadfile = home;
        loadfile += "/.powerguru/mx60.data";
    }
    
    // See if it exists in the right place
    dbglogfile << "Seeing if " << loadfile.c_str() << " exists." << endl;
    if (stat(loadfile.c_str(), &stats) == 0) {
        in.open(loadfile.c_str());
        
        if (!in) {
            dbglogfile << "ERROR: Couldn't open file: " << filespec << endl;
            return ERROR;
        }
        
        // Read in each line and send it
        while (getline(in, line)) {
            newline = '*';
            newline += line;
            newline += '\n';
            fu.Write((unsigned char *)newline.c_str(), newline.size(), Err);
            // All outback products send their data at 1 second intervals
            sleep(1);
        }
    } 
    in.close();
    return ERROR;
}

retcode_t
sim_fx(const char *filespec)
{
}

// local Variables:
// mode: C++
// indent-tabs-mode: nil
// End:
