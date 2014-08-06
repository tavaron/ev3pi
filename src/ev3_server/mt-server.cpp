#include "mt-server.hpp"

/* Startfunktion der Threads */
void *newThread( void* arg ) {

	/* explicit cast of argument */
	tavaron::net::connection_t *myThread =
		(tavaron::net::connection_t*) arg;
	
	/* add thread to list */
	myThread->parent->add(myThread);
	
	/* get ip as string */
    string ip = myThread->socket->remote_endpoint().address().to_string();

	/* print connection details */
	string temp = "Verbindung von ";
	temp += ip;
	temp += " angenommen";
	debug(temp);
	
	/* received client command */
	if(tavaron::net::recvCmd(myThread) > 0)
	
		/* process receidved command */
		switch(myThread->rcmd[LENCMD-1]) {
			
			/* close connection */
		    default:
                sendCmd(myThread, 255);
                break;
                
            /* restart motion webcam server daemon via service-script */
			case 4:
				system("service motion restart");
                sendCmd(myThread, 255);
				break;
				
			/* drive EV3 */
            case 0:
				ev3_USBCom *ev3 = new ev3_USBCom(0x02,0x08);
				while( processConnection(myThread, ev3) == 0 );
				break;

		}/* process receidved command */
	
	/* else deploy error message */
    else
        error("Did not receive a command from client");

	/* deploy debug message about closed connection */
	temp.clear();
	temp = "Verbindung mit ";
	temp += ip;
	temp += " geschlossen";
	debug(temp);
	
	/* close connection */
    try {
        myThread->socket->close();
        myThread->parent->remove(myThread->tid);
    }
    catch(boost::exception_detail::clone_impl
            <boost::exception_detail::error_info_injector
            <boost::system::system_error> >& e)
    {
        error(*myThread->strerr);
    }

	return NULL;
}

/* Worker */
int processConnection(tavaron::net::connection_t *thread, ev3_USBCom *ev3) {

	string temp = "";
	
	/* check for broken connection */
    if( thread->status != -1 ) {
		/* send status of EV3 */
		sendStatus(thread, ev3);
	}
	
	/* check for broken connection and received command */
	if( thread->status != -1 && tavaron::net::recvCmd(thread) > 0) {
		drive_t drv;
		
		/* debugging info on received command */
		if(getppid() != 1) {
			printf("recv:");
			for( int i = 0; i<LENCMD; i++ )
				printf("  %2x", thread->rcmd[i]);
			printf("\n\n");
		}
		
		/* process received command */
		switch(thread->rcmd[LENCMD-1]) {
			
			/* drive EV3 */
			case 0:
				drv.speed = utos(thread->rcmd[0]);
				drv.turn = utos(thread->rcmd[4]);
				drv.brakes = thread->rcmd[8];
				drv.brake_mode = thread->rcmd[12];
				drv.acc_steering = thread->rcmd[16];
				ev3->drive(drv);
				break;
				
			/* restart motion webcam server daemon via service-script */
			case 4:
				system("service motion restart");
				break;
			
			/* exit */
			case 255:
				return 255;
				break;
		}
	}

	/* check for broken or closed connection */
	if( thread->status == 1 || thread->status == -1 )
		return 1;

	return 0;
}

/* Daemonisierung */
void daemonize() {
	pid_t pid, sid;
	
	/* check if already a deamon */
	if( getppid() == 1 )
		return;

	/* fork a new process */
	pid = fork();

	/* fork failed */
	if(pid<0)
		exit(1);
	
	/* exit old process */
	if(pid>0)
		exit(0);

	/* set a sid for the daemon */
	sid = setsid();
	if(sid<0)
		exit(1);

	/* change to root dir (exit if not possible)*/
	if(chdir("/") < 0)
		exit(1);
	
	/* get rid of stdio */
	freopen( "/dev/null", "r", stdin);
    freopen( "/dev/null", "w", stdout);
    freopen( "/dev/null", "w", stderr);
}

int main(int argc, char** argv) {
	
	tavaron::ui::parameter *param = new tavaron::ui::parameter(argc, argv, "d");
	
	/* check for daemonisation */
	if(param->isTrue('d')) {
		cout << "Starte EV3-Server als Daemon..." << endl;
		createPidFile(PIDFILE);
		FILE* f = fopen(OUTPUTFILE, "w");
		fclose(f);
		/* Syslog oeffnen */
		openlog(argv[0], LOG_PID, LOG_LOCAL0);
		daemonize();
		syslog( LOG_DEBUG, "Starte EV3-Server auf Port %d", PORT);
		string s = "PID: ";
		s += getpid();
		output(s);
	}
	else
		/* Kontrollausgabe */
		debug("Starte EV3-Server auf Port 4242");

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
	if(getppid() == 1)
        removePidFile(PIDFILE);
	return 0;
}


/* convert unsigned char to signed integer */
int utos(unsigned char u) {
	int s = 0;

	if(u>127)
		s -= 256;
	s += u;

	return s;
}



int sendCmd(tavaron::net::connection_t* con, unsigned char cmd) {
    
    /* init new char array */
    con->scmd = new unsigned char[LENCMD];
    for(int i = 0; i<LENCMD; i++)
		con->scmd[i] = 0x00;
	
	/* set command byte */
    con->scmd[LENCMD-1] = cmd;
	
	/* send status and return error code */
    return tavaron::net::sendCmd(con);
}

int sendStatus(tavaron::net::connection_t* con, ev3_USBCom* ev3) {
	
	/* init new char array */
    con->scmd = new unsigned char[LENCMD];
    for(int i = 1; i<LENCMD; i++)
        con->scmd[i] = 0x00;
	
	/* get EV3 status */
    con->scmd[0] = ev3->status(false);

	/* send status and return error code */
    return tavaron::net::sendCmd(con);
}

int sendStatusCmd(tavaron::net::connection_t* con, ev3_USBCom* ev3, unsigned char cmd) {
	
	/* init new char array */
    con->scmd = new unsigned char[LENCMD];
    for(int i = 1; i<LENCMD-1; i++)
        con->scmd[i] = 0x00;
	
	/* get EV3 status */
    con->scmd[0] = ev3->status(false);
    
    /* set command byte */
    con->scmd[LENCMD-1] = cmd;
	
	/* send status and return error code */
    return tavaron::net::sendCmd(con);
}

/* print debugging string to stdout or syslog */ 
void debug(string s) {
	
	/* check if daemon */
	if(getppid() == 1)
		/* print string to syslog as debug info */
		syslog( LOG_DEBUG, "%s", s.c_str() );
	
	/* else print string to stdout */
	else
		cout << s << endl;
}

/* print error string to stderr or syslog */
void error(string s) {
	
	/* check if daemon */
	if(getppid() == 1)
		/* write string to syslog as error */
		syslog( LOG_ERR, "%s", s.c_str() );
	
	/* else print string to stderr */
	else
		cerr << s << endl;
}

/* output to stdout or file */
void output(string s) {
	
	/* check if daemon */
	if(getppid() == 1) {
		
		/* lock file */
		pthread_mutex_lock(&outfile_mutex);
		
		/* open file */
		FILE* f = fopen(OUTPUTFILE, "a");
		
		/* check if file is open */
		if(f == NULL) {
			debug("konnte outfile nicht oeffnen");
			pthread_mutex_unlock(&outfile_mutex);
			return;
		}
		
		/* new line */
		fputc('\n', f);
		
		/* append string to file */
		fputs(s.c_str(), f);
		
		/* close file */
		fclose(f);
		
		/* unlock file */
		pthread_mutex_unlock(&outfile_mutex);
		
	} /* check if daemon */
	
	/* else print string to stdout */
	else
		cout << s << endl;
}

