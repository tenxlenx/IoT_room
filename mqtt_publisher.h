// mqtt publisher
#include <mosquittopp.h>
#include <cstdlib>
#include <iostream>

// for convenience
using json = nlohmann::json;

class Mqtt_publisher : public mosqpp::mosquittopp {

private:
	const char *host;
	const char *id;
	const char *topic;
	int port;
	int keepalive;

	void on_connect(int rc);
	void on_disconnect(int rc);
	void on_publish(int mid);
public:
	Mqtt_publisher(const char *id, const char * _topic, const char *host, int port);
	~Mqtt_publisher();
 	bool send_message(const char * _message);
};

Mqtt_publisher::Mqtt_publisher(const char *_id,const char *_topic, const char *_host, int _port) : mosquittopp(_id)
{
	mosqpp::lib_init();        // Mandatory initialization for mosquitto library
	this->keepalive = 60;    // Basic configuration setup for myMosq class
	this->id = _id;
	this->port = _port;
	this->host = _host;
	this->topic = _topic;
	username_pw_set(id);
	connect(host,     // non blocking connection to broker request
	port,
	keepalive);
	loop_start();            // Start thread managing connection / publish / subscribe
};

Mqtt_publisher::~Mqtt_publisher() 
{
	loop_stop();            // Kill the thread
	mosqpp::lib_cleanup();    // Mosquitto library cleanup
}

bool Mqtt_publisher::send_message(const  char * _message)
{
	// Send message - depending on QoS, mosquitto lib managed re-submission this the thread
	//
	// * NULL : Message Id (int *) this allow to latter get status of each message
	// * topic : topic to be used
	// * lenght of the message
	// * message
	// * qos (0,1,2)
	// * retain (boolean) - indicates if message is retained on broker or not
	// Should return MOSQ_ERR_SUCCESS
	int ret = publish(NULL,this->topic,strlen(_message),_message,1,false);
	std::cout << "mqtt code " << ret << std::endl; 
	return ( ret == MOSQ_ERR_SUCCESS );
}

void Mqtt_publisher::on_disconnect(int rc)
{
	std::cout << ">> Mqtt_publisher - disconnection(" << rc << ")" << std::endl;
}

void Mqtt_publisher::on_connect(int rc)
{
	if ( rc == 0 ) {
		std::cout << ">> Mqtt_publisher - connected with server" << std::endl;
	} else {
		std::cout << ">> Mqtt_publisher - Impossible to connect with server(" << rc << ")" << std::endl;
	}
}

void Mqtt_publisher::on_publish(int mid)
{
	std::cout << ">> Mqtt_publisher - Message (" << mid << ") succeed to be published " << std::endl;
}
