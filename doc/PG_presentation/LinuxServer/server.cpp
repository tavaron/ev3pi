// Compile with g++ -lncurses -lusb -lcryptopp -lSDL -lboost_system -lpthread -I~/prg/c++/include -std=c++0x -o server server.cpp

#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include "ev3_usbcom.hpp"
/*
#include <crypto++/sha.h>
#include <crypto++/base64.h>
*/

#define PORT 4242
#define LENCMD 20
#define LENDATA 100

using boost::asio::ip::tcp;

using namespace std;

bool passwd(tcp::socket *sock);
string recvData(tcp::socket *sock);
string getConfig(string str);
string SHA256HashString(std::string aString);


int main(int argc, char** argv)
{
	cerr << "Starte Server auf Port " << PORT << endl;
	
	boost::asio::io_service ioService;
	tcp::acceptor acceptor(ioService, tcp::endpoint(tcp::v4(), PORT));
	tcp::socket *sock;
	while(true) {
		sock = new tcp::socket(ioService);
		acceptor.accept(*sock);
		bool auth = false;
		ev3_USBCom *ev3 = new ev3_USBCom(0x02,0x04);
		while(true) {
			drive_t drv;
			char recv[LENCMD];
			char send[LENCMD+1] = "";
			//char data[LENDATA] = "\0";
			try {
				sock->read_some(boost::asio::buffer(recv, LENCMD));
				//cout << " recv: " << recv << endl;
				printf("recv:");
				for( int i = 0; i<LENCMD; i++ )
					printf("  %2x", recv[i]);
				printf("\ndec: ");
				for( int i = 0; i<LENCMD; i++ )
					printf(" %3d", recv[i]);
				printf("\n\n");
				
				drv.speed = recv[0];
				drv.turn = recv [4];
				drv.brakes = recv[8];
				drv.brake_mode = recv[12];
				drv.acc_steering = recv[16];
				
				ev3->drive(drv);
			}
			catch(boost::exception_detail::clone_impl<boost::exception_detail::error_info_injector<boost::system::system_error> >& e)
			{
				cerr << e.what();
				break;
			}
			
			/*
			//CMD senden
			try {
				cout << " send: " << send << endl;
				sock->write_some(boost::asio::buffer(send, LENCMD));
				if(strcmp(send,"BYE!")==0) {
					cout << "Verbindung mit " << sock->remote_endpoint() << " beendet.\n";
					break;
				}
			}
			catch(boost::exception_detail::clone_impl<boost::exception_detail::error_info_injector<boost::system::system_error> >& e)
			{
				cerr << e.what();
				break;
			}*/
		}
	}
	
	return 0;
}
/*
bool passwd(tcp::socket *sock) {
	bool r = false;
	string pwHash = getConfig("password");
	char data[LENDATA] = "\0";
	string pw("");
	if(pwHash.empty())
		r = true;
	else{
		try {
			sock->write_some(boost::asio::buffer("PWGO", LENCMD));
			cout << " send: PWGO" << endl;
			sock->read_some(boost::asio::buffer(data,LENDATA));
			pw = SHA256HashString(data);
			
			if(pwHash.compare(pw)==0)
				r = true;
		}
		catch(boost::exception_detail::clone_impl<boost::exception_detail::error_info_injector<boost::system::system_error> >& e)
		{
			cerr << e.what();
			r = false;
		}
	}
	return r;
}*/

bool setPasswd(tcp::socket *sock) {
	
	bool r = false;
	string pwHash = getConfig("password");
	
	return r;
}

string getConfig(string str) {
	FILE *f = fopen("server.conf", "r");
	if(f == NULL)
	{ return ""; }
	
	string value("");
	char c='a';
	while(c!=EOF)
	{
		string tmp("");
		
		while((c = fgetc(f))!=10&&c!=EOF)
			tmp+=c;
		if(tmp.empty())
			continue;
		
		size_t pos = tmp.find_first_of("#");
		if(pos!=string::npos)
		{ tmp = tmp.substr(0, pos); }
		if(tmp.empty())
			continue;
		
		pos = tmp.find_first_of("=");
		if( pos!=string::npos && str.compare(tmp.substr(0,pos))==0 ) {
			value = tmp.substr(pos+1);
			break;
		}
	}
	fclose(f);
	return value;
}

/*
string SHA256HashString(std::string aString) {
    std::string digest;
    CryptoPP::SHA256 hash;

    CryptoPP::StringSource foo(aString, true,
    new CryptoPP::HashFilter(hash,
      new CryptoPP::Base64Encoder (
         new CryptoPP::StringSink(digest))));

    return digest.substr(0,digest.length() -1);
}
*/
