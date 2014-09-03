#include "net.hpp"

/* Methoden zu connections */
int tavaron::net::connections::add(pthread_t *tid)	{

    connection_t *conn;
    connection_t *tmp;
    conn = (connection_t*) malloc(sizeof(connection_t));

    if(conn == NULL)
        return -1;

    conn->tid = tid;
    conn->status = 0;
    last = conn;
    tmp = head->next;
    conn->next = tmp;
    head->next = conn;
    index++;
    return 0;
}

int tavaron::net::connections::add(connection_t* conn)	{

    connection_t *tmp;

    if(conn == NULL)
        return -1;

    last = conn;
    tmp = head->next;
    conn->next = tmp;
    head->next = conn;

    /* Mutex initialisieren */
    if( pthread_mutex_init(&conn->mutex, NULL) != 0 )
        std::cerr << "pthread_mutex_init()\n";

    /* Exklusivzugriff sichern*/
    if( pthread_mutex_lock(&conn->mutex) != 0 );

    /* Element initialisieren */
    conn->index = index++;
    conn->sdata = new std::string("");
    conn->rdata = new std::string("");
    conn->scmd = new unsigned char[LENCMD + 1];
    conn->rcmd = new unsigned char[20];
    conn->strerr = new std::string("");

    /* Exklusivzugriff freigeben */
    if( pthread_mutex_unlock(&conn->mutex) != 0 );

    return 0;
}

int tavaron::net::connections::remove(pthread_t *tid) {

    connection_t *conn, *tmp;
    for(conn = head;conn->next != conn->next->next; conn = conn->next)
        if(pthread_equal(*conn->next->tid, *tid)) {
            tmp = conn->next;
            conn->next = conn->next->next;
            if(pthread_equal(*tmp->tid, *last->tid))
                last = NULL;
            free(tmp);
            return 0;
        }
    return -1;
}

int tavaron::net::connections::getOpenConnections() {
    return openConnections;
}

int tavaron::net::connections::getStatus() {
    return status;
}

int tavaron::net::connections::getIndex() {
    return index;
}

tavaron::net::connection_t* tavaron::net::connections::getConnection(pthread_t *tid) {

    connection_t *conn;
    conn = head;

    while(conn->next != conn->next->next) {
        if(pthread_equal(*conn->next->tid, *tid))
            return conn->next;
        conn = conn->next;
    }
    return NULL;
}

tavaron::net::connection_t* tavaron::net::connections::getLast() {
    return last;
}



/* Funktionen zur Netzwerkkommunikation */
int tavaron::net::sendData(connection_t* thread) {

    int r = -1;

    /* Beende bei leerem Puffer */
    if(thread->sdata->empty()) {
        threadStatus(thread, ST_NO_SDATA);
        std::cerr << thread->strerr << std::endl;
        return 0;
    }

    /* Exklusivzugriff sichern*/
    if( pthread_mutex_lock(&thread->mutex) != 0 ) {
        threadStatus(thread, ST_CONN_ABORTED);
        std::cerr << thread->strerr << std::endl;
	}

    /* Sende Daten */
    try {
        thread->socket->write_some(
            boost::asio::buffer(
                *thread->sdata, thread->sdata->size() ) );
        r = thread->sdata->size();
    }

    /* Setze Fehler und breche ab */
    catch(boost::exception_detail::clone_impl
            <boost::exception_detail::error_info_injector
            <boost::system::system_error> >& e)
    {
        threadStatus(thread, ST_CONN_ABORTED, e.what());
        std::cerr << thread->strerr << std::endl;
    }

    /* Exklusivzugriff freigeben */
    if( pthread_mutex_unlock(&thread->mutex) != 0 ) {
		threadStatus(thread, ST_MUTEX_UNLOCK);
		std::cerr << thread->strerr << std::endl;
	}
    return r;
}

int tavaron::net::sendCmd(connection_t* thread) {

    int r = -1;

    /* exit and set strerr if buffer is empty */
    if(thread->scmd[0] == 255 || thread->scmd == NULL) {
        threadStatus(thread, ST_NO_SCMD);
        return 0;
    }

    /* lock thread mutex and exit if not possible*/
    if( pthread_mutex_lock(&thread->mutex) != 0 ) {
		threadStatus(thread, ST_MUTEX_LOCK);
        std::cerr	<< thread->strerr << std::endl;
        return -1;
	}

    /* send command */
    try {
        thread->socket->write_some(
            boost::asio::buffer(
                thread->scmd, LENCMD ) );
        r = LENCMD;
    }

    /* save error to strerr and status */
    catch(boost::exception_detail::clone_impl
            <boost::exception_detail::error_info_injector
            <boost::system::system_error> >& e)
    {
        threadStatus(thread, ST_CONN_ABORTED, e.what());
        std::cerr << thread->strerr << std::endl;
    }

    thread->scmd[0] = 255;

    /* unlock thread mutex */
    if( pthread_mutex_unlock(&thread->mutex) != 0 ) {
		threadStatus(thread, ST_MUTEX_UNLOCK);
        std::cerr << thread->strerr << std::endl;
	}

    return r;
}

size_t tavaron::net::recvData(connection_t* thread) {

	/* init variables */
    char buf[LENDATA+1];
    size_t bytes = 0;
    boost::system::error_code error;
    thread->rdata->clear();

    /* lock thread mutex and exit if not possible*/
    if( pthread_mutex_lock(&thread->mutex) != 0 ) {
		threadStatus(thread, ST_MUTEX_LOCK);
        std::cerr	<< thread->strerr << std::endl;
        return -1;
	}

    /* Schleife bis EOF */
    do {
        try {
            /* Lese Daten und diese sie an String an */
            bytes += thread->socket->read_some(
                boost::asio::buffer(buf, LENDATA), error );
            thread->rdata->append(buf);
        }

        /* Setze Fehler */
        catch(boost::exception_detail::clone_impl
                <boost::exception_detail::error_info_injector
                <boost::system::system_error> >& e)
        {
            threadStatus(thread, ST_NO_RDATA, e.what());
            std::cerr << thread->strerr << std::endl;
            bytes = 0;
            break;
        }
    } while(error != boost::asio::error::eof);
	
	
    /* unlock thread mutex */
    if( pthread_mutex_unlock(&thread->mutex) != 0 ) {
		threadStatus(thread, ST_MUTEX_UNLOCK);
        std::cerr	<< thread->strerr << std::endl;
	}

    return bytes;
}

size_t tavaron::net::recvCmd(connection_t* thread) {

    size_t bytes = 0;
    boost::system::error_code error;



    /* Exklusivzugriff sichern*/
    if( pthread_mutex_lock(&thread->mutex) != 0 ) {
		threadStatus(thread, ST_MUTEX_LOCK);
		std::cerr << thread->strerr << std::endl;
	}

    try {
        /* Lese cmd */
        bytes = thread->socket->read_some(
            boost::asio::buffer(thread->rcmd, LENCMD));

    }

    /* Setze Fehler */
    catch(boost::exception_detail::clone_impl
            <boost::exception_detail::error_info_injector
            <boost::system::system_error> >& e)
    {
        threadStatus(thread, ST_NO_RDATA, e.what());
        bytes = 0;
    }

    /* Exklusivzugriff freigeben */
    if( pthread_mutex_unlock(&thread->mutex) != 0 ) {
		threadStatus(thread, ST_MUTEX_UNLOCK);
		std::cerr << thread->strerr << std::endl;
	}
	
    return bytes;
}

void tavaron::net::threadStatus(connection_t* thread, int status) {
	
	thread->status = status;
	thread->strerr->clear();
	
	switch(status) {
		case ST_OK:
			thread->strerr->append("everything fine");
			break;
		case ST_MUTEX_LOCK:
			thread->strerr->append("could not lock mutex");
			break;
		case ST_MUTEX_UNLOCK:
			thread->strerr->append("could not unlock mutex");
			break;
		case ST_CONN_ABORTED:
			thread->strerr->append("connection aborted");
			break;
		case ST_CONN_CLOSED:
			thread->strerr->append("connection closed");
			break;
		case ST_AUTH_OK:
			thread->strerr->append("user successfully authenticated");
			break;
		case ST_NO_SCMD:
			thread->strerr->append("no command to send to ");
			thread->strerr->append(thread->socket->remote_endpoint().address().to_string());
			break;
		case ST_NO_SDATA:
			thread->strerr->append("no data to send to ");
			thread->strerr->append(thread->socket->remote_endpoint().address().to_string());
			break;
		case ST_NO_RCMD:
			thread->strerr->append("no command received from ");
			thread->strerr->append(thread->socket->remote_endpoint().address().to_string());
			break;
		case ST_NO_RDATA:
			thread->strerr->append("no data received from ");
			thread->strerr->append(thread->socket->remote_endpoint().address().to_string());
			break;
		default:
			thread->strerr->append("unknown error");
			break;
	}
	
	thread->strerr->append(" in thread x");
	//thread->strerr->append((int)thread->tid));
	
}

void tavaron::net::threadStatus(connection_t* thread, int status, std::string msg) {
	threadStatus(thread, status);
	thread->strerr->append("\nmessage: ");
	thread->strerr->append(msg);
}
