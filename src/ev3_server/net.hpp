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

#ifndef __TAVARON_NET_HPP
#define __TAVARON_NET_HPP

#define LENCMD 20
#define LENDATA 100


#define ST_AUTH_OK 2
#define ST_CONN_CLOSED 1
#define ST_OK 0
#define ST_CONN_ABORTED -1
#define ST_NO_SCMD -2
#define ST_NO_SDATA -3
#define ST_NO_RCMD -4
#define ST_NO_RDATA -5

#define ST_MUTEX_LOCK -8
#define ST_MUTEX_UNLOCK -9

#define ST_CONSTRUCTOR_FAILED -128



namespace tavaron {
	namespace net {
		
		
		int sendData(connection_t* thread);
		int sendCmd(connection_t* thread);
		
		size_t recvData(connection_t* thread);
		size_t recvCmd(connection_t* thread);
		
		void threadStatus(connection_t* thread, int status);
		void threadStatus(connection_t* thread, int status, std::string msg);
		

		typedef struct connectionElement { /* connection_t */

			pthread_t *tid; /* thread id */
			pthread_mutex_t mutex; /* thread mutex */
			boost::asio::ip::tcp::socket *socket; /* tcp socket */
			int status; /* possible values defined as ST_* */
			std::string *strerr;
			unsigned char *scmd, *rcmd;
			std::string *sdata, *rdata;
			struct connectionElement* next;
			class connections* parent;
			int index;
			bool auth;

		} connection_t;

                            /* connections */
        class connections { /* chained list of connections/threads */
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

        #include "net.cpp"

		
	}
}

#endif
