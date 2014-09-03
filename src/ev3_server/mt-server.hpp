// Compile with g++ -lncurses -lusb -lcryptopp -lSDL -lboost_system -lpthread -I~/prg/c++/include -std=c++0x -o server server.cpp
#ifndef __MT_SERVER_HPP
#define __MT_SERVER_HPP

#include <iostream>
#include <boost/asio.hpp>
#include <string>

#include <unistd.h>
#include <syslog.h>
#include <signal.h>

#include "net.hpp"
#include "ui.hpp"
#include "ev3_usbcom.hpp"
#include "pidfile.hpp"


#define PORT 4242
#define LENCMD 20
#define LENDATA 100
#define CONFIGFILE "/etc/ev3pi.conf"
#define OUTPUTFILE "/tmp/ev3pi.log"
#define PIDFILE "/var/run/ev3_server.pid"

using boost::asio::ip::tcp;
using namespace std;

void *newThread( void* arg );
void daemonize();

void debug(string s);
void error(string s);
void output(string s);

int utos(unsigned char u);

int processConnection(tavaron::net::connection_t *thread, ev3_USBCom *ev3);
int sendStatus(tavaron::net::connection_t* con, ev3_USBCom* ev3);
int sendCmd(tavaron::net::connection_t* con, unsigned char cmd);
int sendStatusCmd(tavaron::net::connection_t* con, ev3_USBCom* ev3, unsigned char cmd);


pthread_mutex_t outfile_mutex = PTHREAD_MUTEX_INITIALIZER;

#endif // __MT-SERVER_HPP
