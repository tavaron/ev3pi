#include <iostream>
#include <boost/asio.hpp>
#include <cstdlib>
#include <string>

#define PORT 4242
#define LENCMD 20
#define LENDATA 100

using boost::asio::ip::tcp;
using namespace std;

string recvData(tcp::socket *sock);
string getConfig(string str);

int main(int argc, char** argv)
{

	if(argc!=2)
	{
		string fehler("Gebrauch: ");
		fehler.append(argv[0]);
		fehler.append(" <ip>\n");
		cout << fehler;
		return 1;
	}
	
	bool auth = false;
	
	boost::asio::io_service ioService;
	boost::asio::ip::tcp::endpoint 
		server(boost::asio::ip::address::from_string(argv[1]), PORT);
	boost::asio::ip::tcp::socket socket(ioService);
	
	cout << "Verbinde mit " << argv[1] << ":" << PORT << endl;
	try {
		socket.connect(server);
	}
	catch(boost::exception_detail::clone_impl<boost::exception_detail::error_info_injector<boost::system::system_error> >& e)
	{ cout << "Verbindungsaufbau mit " << argv[1] << ":" << PORT << " gescheitert\n"; return 2; }
	
	socket.write_some(boost::asio::buffer("HELO", LENCMD));
	printf(" send: HELO\n");
	
	while(true) {
		char send[LENCMD] = "NONE";
		char recv[LENCMD] = "WAIT";
		
		// Empfange Befehl
		try {
			socket.read_some(boost::asio::buffer(recv, LENCMD));
			cout << " recv: " << recv << endl;
		}
		catch(boost::exception_detail::clone_impl<boost::exception_detail::error_info_injector<boost::system::system_error> >& e) {
			cerr << e.what();
			break;
		}
		
		if(strcmp(recv, "HELO")==0) {
			cout << "Verbindung zu " << socket.remote_endpoint() << " hergestellt.\n";
			strcpy(send, "WAIT");
		}
		else if(strcmp(recv, "PWOK")==0) {
			cout << "Zugang gewährt.\n";
			auth = true;
			strcpy(send, "WAIT");
		}
		else if(strcmp(recv, "PWNO")==0) {
			cout << "Zugang verweigert.\n";
			auth = false;
			strcpy(send, "WAIT");
		}
		else if(strcmp(recv, "PWGO")==0) {
			string passwd("");
			cout << "Passwort: ";
			cin >> passwd;
			socket.write_some(boost::asio::buffer(passwd, LENDATA));
		}
		else if(strcmp(recv, "BYE!")==0) {
			cout << "Verbindung mit " << socket.remote_endpoint() << " beendet.\n";
			break;
		}
		else if(strcmp(recv, "WAIT")==0) {
			if(auth)
				cout << "# ";
			else
				cout << "$ ";
			cin >> send;
		}
		else if(strcmp(recv, "UKWN")==0) {
			cout << "Befehl wurde nicht verstanden.\n";
			strcpy(send, "WAIT");
		}
		else {
			strcpy(send, "UKWN");
			cout << "Server sendet falsche Antwort!\n"; }
		
		try {
			socket.write_some(boost::asio::buffer(send, LENCMD));
			cout << " send: " << send << endl;
		}
		catch(boost::exception_detail::clone_impl<boost::exception_detail::error_info_injector<boost::system::system_error> >& e)
		{
			cerr << e.what() << endl;
			break;
		}
		
	}
	
	
	
	return 0;
}

string recvData(tcp::socket *sock)
{
	string r("");
	char recv[LENDATA] = "\0";
	
	try {
		sock->read_some(boost::asio::buffer(recv, LENDATA));
		r.append(recv);
	}
	catch(boost::exception_detail::clone_impl<boost::exception_detail::error_info_injector<boost::system::system_error> >& e)
	{
		cerr << e.what();
	}
	return r;
}

string getConfig(string str)
{
	FILE *f = fopen("client.conf", "r");
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
