#include <iostream>
#include <boost/asio.hpp>
#include <string>

#include "net.hpp"
#include <ui.hpp>
#include <file.hpp>
#include <crypto.hpp>
#include "ev3_usbcom.hpp"

#include <unistd.h>
#include <syslog.h>

#define PORT 4242
#define LENCMD 20
#define LENDATA 100
#define CONFIGFILE "/etc/local/mt-server.conf"
#define OUTPUTFILE "/home/marius/Desktop/mt-server.out"

using boost::asio::ip::tcp;
using namespace std;

void *newThread( void* arg );
void daemonize();

void debug(string s);
void error(string s);
void output(string s);

bool passwd(tcp::socket *sock);
int processConnection(tavaron::net::connection_t *thread, ev3_USBCom *ev3);


pthread_mutex_t outfile_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Startfunktion der Threads */
void *newThread( void* arg ) {
	
	/* Wandle Argument um */
	tavaron::net::connection_t *myThread = 
		(tavaron::net::connection_t*) arg; 
		
	myThread->parent->add(myThread);
	
	
	/* Verbindungsdetails */
	string temp = "Verbindung von ";
	temp += myThread->socket->remote_endpoint().address().to_string();
	temp += " angenommen";
	debug(temp);
	
	
	ev3_USBCom *ev3 = new ev3_USBCom(0x02,0x04);
	while( processConnection(myThread, ev3) == 0 );
	return NULL;
}

/* Daemonisierung */
void daemonize()
{
	pid_t pid, sid;
	
	if( getppid() == 1 )
		return;
		
	pid = fork();
	
	if(pid<0)
		exit(1);
		
	if(pid>0)
		exit(0);
		
	sid = setsid();
	if(sid<0)
		exit(1);
		
	if(chdir("/") < 0)
		exit(1);
		
	freopen( "/dev/null", "r", stdin);
    freopen( "/dev/null", "w", stdout);
    freopen( "/dev/null", "w", stderr);
}

int main(int argc, char** argv)
{
	tavaron::ui::parameter *param = new tavaron::ui::parameter(argc, argv, "d");
	
	if(param->isTrue('d')) {
		FILE* f = fopen(OUTPUTFILE, "w");
		fclose(f);
		/* Syslog oeffnen */
		openlog(argv[0], LOG_PID, LOG_LOCAL0);
		cout << "Starte Server als Daemon..." << endl;
		daemonize();
		syslog( LOG_DEBUG, "Starte Server auf Port %d", PORT);
	}
	else
		/* Kontrollausgabe */
		debug("Starte Server auf Port 4242");
	
	/* Verkettete Liste mit Threads */
	tavaron::net::connections *threads = 
		new tavaron::net::connections();
		
	/* Boost TCP-Netzwerkklassen */
	boost::asio::io_service ioService;
	tcp::acceptor acceptor( ioService, tcp::endpoint(tcp::v4(), PORT) );
	tcp::socket *socket;
	
	/* Hauptschleife */
	while(true) {
		
		/* Socket erstellen */
		socket = new tcp::socket(ioService);
		acceptor.accept(*socket);
		
		/* Listenelement erstellen */
		tavaron::net::connection_t* conn;
		conn = (tavaron::net::connection_t*)
				malloc(sizeof(tavaron::net::connection_t));
		conn->tid = (pthread_t*) malloc(sizeof(pthread_t));
		conn->socket = socket;
		conn->parent = threads;
		
		
		
		/* Neuen Thread starten */
		pthread_create(conn->tid, NULL, newThread, (void*) conn);

	}
	
	closelog();
	return 0;
}


/* Worker */
int processConnection(tavaron::net::connection_t *thread, ev3_USBCom *ev3) {
	
	string temp = "";
	
	if(tavaron::net::recvCmd(thread) > 0) {
		drive_t drv;
		printf("recv:");
		for( int i = 0; i<LENCMD; i++ )
			printf("  %2x", thread->rcmd[i]);
		printf("\ndec: ");
		for( int i = 0; i<LENCMD; i++ )
			printf(" %3d", thread->rcmd[i]);
		printf("\n\n");
			
		drv.speed = thread->rcmd[0];
		drv.turn = thread->rcmd[4];
		drv.brakes = thread->rcmd[8];
		drv.brake_mode = thread->rcmd[12];
		drv.acc_steering = thread->rcmd[16];
		
		ev3->drive(drv);
	}
	
	if( thread->status != -1 ) {
		
		thread->scmd = new unsigned char[LENCMD];
		thread->scmd[0] = ev3->status(false);
		for(int i = 1; i<LENCMD; i++)
			thread->scmd[i] = 0x00;
		int bytes = tavaron::net::sendCmd(thread);
		printf("sent %2d bytes\n", bytes);
	}
	
	if( thread->status == 1 || thread->status == -1 )
		pthread_exit(NULL);
		
	
	return 0;
}

bool passwd(tcp::socket *sock) {
	bool r = false;
	string pwHash = tavaron::file::getConfig(CONFIGFILE, "password");
	char data[LENDATA] = "\0";
	string pw("");
	if(pwHash.empty())
		r = true;
	else{
		try {
			sock->write_some(boost::asio::buffer("PWGO", LENCMD));
			output("send: PWGO");
			sock->read_some(boost::asio::buffer(data,LENDATA));
			pw = tavaron::crypto::SHA256HashString(data);
			
			if(pwHash.compare(pw)==0)
				r = true;
		}
		catch(boost::exception_detail::clone_impl<boost::exception_detail::error_info_injector<boost::system::system_error> >& e)
		{
			error(e.what());
			r = false;
		}
	}
	return r;
}

/* Ausgabe-Funktionen */
void debug(string s) {
	if(getppid() == 1)
		syslog( LOG_DEBUG, "%s", s.c_str() );
	else
		cout << s << endl;
}

void error(string s) {
	if(getppid() == 1)
		syslog( LOG_ERR, "%s", s.c_str() );
	else
		cerr << s << endl;
}

void output(string s) {
	if(getppid() == 1) {
		pthread_mutex_lock(&outfile_mutex);
		FILE* f = fopen(OUTPUTFILE, "a");
		if(f == NULL) {
			debug("konnte outfile nicht oeffnen");
			pthread_mutex_unlock(&outfile_mutex);
			return;
		}
		fputc('\n', f);
		fputs(s.c_str(), f);
		fclose(f);
		pthread_mutex_unlock(&outfile_mutex);
	}
	else
		cout << s << endl;
}

void output(char* s) {
	if(getppid() == 1) {
		pthread_mutex_lock(&outfile_mutex);
		FILE* f = fopen(OUTPUTFILE, "a");
		if(f == NULL) {
			debug("konnte outfile nicht oeffnen");
			pthread_mutex_unlock(&outfile_mutex);
			return;
		}
		fputc('\n', f);
		fputs(s, f);
		fclose(f);
		pthread_mutex_unlock(&outfile_mutex);
	}
	else
		cout << s << endl;
}
