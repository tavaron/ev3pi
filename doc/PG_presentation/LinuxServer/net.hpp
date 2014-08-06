//      ----------------------------------------------------------------
//      
//      Datei: net.hpp
//      
//      Name:		Marius Christ
//      Matr.-Nr.:	954402
//      Datum:		17.01.2012 v0.2
//      geaendert:	17.01.2012
//      
//      Beschreibung: Struktur, verkette Liste, Netzwerkfunktionen
//      
//      ----------------------------------------------------------------

#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <string.h>

#define LENCMD 20
#define LENDATA 100

namespace tavaron {
	namespace net {
	
	
		typedef struct connectionElement { /* connection_t */
		
			pthread_t *tid; /* Thread-ID */
			pthread_mutex_t mutex; /* Zugriffskontrolle */
			boost::asio::ip::tcp::socket *socket; /* TCP-Socket */
			int status; /* Moeglicher Status
						 * 2	Benutzer authentifiziert
						 * 1	Verbindung beendet
						 * 0 	Keine Fehler
						 * -1	Verbindung abgebrochen
						 * -2	Kein Befehl zum Senden vorhanden
						 * -3	Keine Daten zum Senden vorhanden
						 * -4	Befehl nicht verstanden
						 * -5	Befehl wurde nicht verstanden
						 */
			std::string *strerr;
			unsigned char *scmd, *rcmd;
			std::string *sdata, *rdata;
			struct connectionElement* next;
			class connections* parent;
			int index;
			bool auth;
			
		} connection_t;
		
		/* connections */ //{
			class connections { /* Verkette Liste mit Verbindungen/Threads */
				private:
					int openConnections, status, index;
					connection_t *head, *tail, *last;
				public:
					connections()
					:openConnections(0), status(0), index(0)
					{
						head = (connection_t*) malloc(sizeof(connection_t));
						tail = (connection_t*) malloc(sizeof(connection_t));
						if(head == NULL || tail == NULL)
							status = -1;
						head->next = tail->next = tail;
					}
					
					//Prototypen
					~connections();
					int add(pthread_t *tid);
					int add(connection_t* conn);
					int remove(pthread_t *tid);
					int removeDead();
					connection_t* getConnection(pthread_t *tid);
					connection_t* getLast();
					int getOpenConnections();
					int getStatus();
					int getIndex();
			};
			
			/* Methoden zu connections */
			int connections::add(pthread_t *tid)	{
				
				connection_t *conn, *tmp;
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
			
			int connections::add(connection_t* conn)	{
				
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
					std::cerr	<< "pthread_mutex_lock() in connections::add()\n";
				
				/* Element initialisieren */
				conn->index = index++;
				conn->sdata = new std::string("");
				conn->rdata = new std::string("");
				conn->scmd = new unsigned char[LENCMD + 1];
				conn->rcmd = new unsigned char[20];
				conn->strerr = new std::string("");
				
				/* Exklusivzugriff freigeben */
				if( pthread_mutex_unlock(&conn->mutex) != 0 );
					std::cerr	<< "pthread_mutex_unlock() in Thread "
								<< conn->tid << std::endl;
				
				return 0;
			}
			
			int connections::remove(pthread_t *tid) {
				
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
			
			int connections::getOpenConnections() {
				return openConnections;
			}

			int connections::getStatus() {
				return status;
			}
			
			int connections::getIndex() {
				return index;
			}
			
			connection_t* connections::getConnection(pthread_t *tid) {
				
				connection_t *conn;
				conn = head;
				
				while(conn->next != conn->next->next) {
					if(pthread_equal(*conn->next->tid, *tid))
						return conn->next;
					conn = conn->next;
				}
				return NULL;
			}
			
			connection_t* connections::getLast() {
				return last;
			}
			
			
			/* Funktionen zur Netzwerkkommunikation */
			int sendData(connection_t* thread) {
				
				int r = -1;
				
				/* Beende bei leerem Puffer */
				if(thread->sdata->empty()) { 
					thread->strerr->clear();
					thread->strerr->append("no data to send to ");
					thread->strerr->append(thread->socket->remote_endpoint()
											.address().to_string());
					thread->status = -2;
					return 0;
				}
				
				/* Exklusivzugriff sichern*/
				if( pthread_mutex_lock(&thread->mutex) != 0 )
					std::cerr	<< "pthread_mutex_lock()\n";
				
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
					thread->status = -1;
					thread->strerr->clear();
					thread->strerr->append(e.what());
				}
				
				/* Exklusivzugriff freigeben */
				if( pthread_mutex_unlock(&thread->mutex) != 0 )
					std::cerr	<< "pthread_mutex_unlock()\n";
					
				return r;
			}
			
			int sendCmd(connection_t* thread) {
				
				int r = -1;
				
				/* Beende bei leerem Puffer */
				if(thread->scmd[0] == 255) { 
					thread->strerr->append("no cmd to send to ");
					thread->strerr->append(thread->socket->remote_endpoint()
											.address().to_string());
					return 0;
				}
				
				/* Exklusivzugriff sichern*/
				if( pthread_mutex_lock(&thread->mutex) != 0 )
					std::cerr	<< "pthread_mutex_lock()\n";
				
				/* Sende Daten */
				try {
					thread->socket->write_some(
						boost::asio::buffer(
							thread->scmd, LENCMD ) );
					r = LENCMD;
				}
				
				/* Setze Fehler */
				catch(boost::exception_detail::clone_impl
						<boost::exception_detail::error_info_injector
						<boost::system::system_error> >& e)
				{
					thread->status = -1;
					thread->strerr->clear();
					thread->strerr->append(e.what());
				}
				
				thread->scmd[0] = 255;
				
				/* Exklusivzugriff freigeben */
				if( pthread_mutex_unlock(&thread->mutex) != 0 )
					std::cerr	<< "pthread_mutex_unlock()\n";
				
				return r;
			}
			
			size_t recvData(connection_t* thread) {
				
				char buf[LENDATA+1];
				size_t bytes = 0;
				boost::system::error_code error;
				thread->rdata->clear();
				
				/* Exklusivzugriff sichern*/
				if( pthread_mutex_lock(&thread->mutex) != 0 )
					std::cerr	<< "pthread_mutex_lock()\n";
								
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
						thread->status = -1;
						thread->strerr->clear();
						thread->strerr->append(e.what());
						bytes = 0;
					}
				} while(error != boost::asio::error::eof);
				
				
				/* Exklusivzugriff freigeben */
				if( pthread_mutex_unlock(&thread->mutex) != 0 )
					std::cerr	<< "pthread_mutex_unlock()\n";
				
				return bytes;
			}
			
			size_t recvCmd(connection_t* thread) {
				
				size_t bytes = 0;
				boost::system::error_code error;
				
				
				
				/* Exklusivzugriff sichern*/
				if( pthread_mutex_lock(&thread->mutex) != 0 )
					std::cerr	<< "pthread_mutex_lock()\n";
				
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
					thread->status = -1;
					thread->strerr->clear();
					thread->strerr->append(e.what());
					bytes = 0;
				}
				
				/* Exklusivzugriff freigeben */
				if( pthread_mutex_unlock(&thread->mutex) != 0 )
					std::cerr	<< "pthread_mutex_unlock()\n";
				
				return bytes;
			}
		//}

	}
}
