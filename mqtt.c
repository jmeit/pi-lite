#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>
#include "macaddr.h"

//#define UNUSED(...) (void)(__VA_ARGS__)
#define UNUSED(x) (void)(x)

#define MQTT_HOST   "mqtt.bassface.space"
#define MQTT_PORT   8883
#define TARGET_USER "wildboard"
#define TARGET_PASS "wilderness user"
#define CA_PATH     "/etc/ssl/certs"
#define KEEP_ALIVE  10
#define CLEAN_SESS  true
#define TOPIC       "subtest"


struct mosquitto* mosq;


void on_connect(struct mosquitto *mosq, void *obj, int rc)
{
	printf("ID: %d\n", * (int *) obj);
	if(rc) {
		printf("Error with result code: %d\n", rc);
		exit(-1);
	}
	mosquitto_subscribe(mosq, NULL, TOPIC, 0);
}


void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
    UNUSED( mosq );
    UNUSED( obj );
	printf("New message with topic %s: %s\n", msg->topic, (char *) msg->payload);
}

void on_publish( struct mosquitto* mosq, void* obj, int mid )
{
    UNUSED( mosq );
    UNUSED( obj );
    UNUSED( mid );
    //mosquitto_loop_stop( mosq, true );
	//mosquitto_disconnect( mosq );
}

int mqtt_init()
{
	int rc;

	mosquitto_lib_init();

    char mac_addr[17];
    load_hwmac( mac_addr );

	mosq = mosquitto_new( mac_addr, CLEAN_SESS, NULL );

	mosquitto_connect_callback_set( mosq, on_connect );
	mosquitto_message_callback_set( mosq, on_message );
    mosquitto_publish_callback_set( mosq, on_publish );

    mosquitto_username_pw_set( mosq, TARGET_USER, TARGET_PASS );
    mosquitto_tls_insecure_set( mosq, false );
    mosquitto_tls_opts_set( mosq, 1, NULL, NULL );
    mosquitto_tls_set( mosq, NULL, CA_PATH, NULL, NULL, NULL );

	rc = mosquitto_connect( mosq, MQTT_HOST, MQTT_PORT, KEEP_ALIVE );
	if(rc) {
		printf("Could not connect to Broker with return code %d\n", rc);
		return -1;
	}

	mosquitto_loop_start( mosq );

	return 0;
}

int mqtt_destroy()
{
	mosquitto_loop_stop( mosq, true );
	mosquitto_disconnect( mosq );
	mosquitto_destroy( mosq );
	mosquitto_lib_cleanup();
    return 0;
}

int mqtt_publish( char* message ){

	int rc;
	rc = mosquitto_publish( mosq, NULL, TOPIC, strlen( message ), message, 0, false );

	return rc;
}
