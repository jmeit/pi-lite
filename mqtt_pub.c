#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mosquitto.h>

#define MQTT_HOST   "mqtt.bassface.space"
#define MQTT_PORT   8883
#define TARGET_USER "wildboard"
#define TARGET_PASS "wilderness user"
#define CA_PATH     "/etc/ssl/certs"
#define KEEP_ALIVE  60
#define CLEAN_SESS  true
#define TOPIC       "subtest"


void on_publish( struct mosquitto* mosq, void* obj, int mid )
{
//    printf( "Sent msg#%i\n", mid );
    mosquitto_loop_stop( mosq, true );
	mosquitto_disconnect( mosq );
}

int main( int argc, char* argv[] ){

    char* to_send = argv[1];
	int rc;
	struct mosquitto* mosq;
    int id = 2;

	mosquitto_lib_init();

	mosq = mosquitto_new( "client-pub", CLEAN_SESS, &id );
    mosquitto_publish_callback_set( mosq, on_publish );

    mosquitto_username_pw_set( mosq, TARGET_USER, TARGET_PASS );
    mosquitto_tls_insecure_set( mosq, false );
    mosquitto_tls_opts_set( mosq, 1, NULL, NULL );
    mosquitto_tls_set( mosq, NULL, CA_PATH, NULL, NULL, NULL );

	rc = mosquitto_connect( mosq, MQTT_HOST, MQTT_PORT, KEEP_ALIVE );
	if(rc != 0){
		printf("Client could not connect to broker! Error Code: %d\n", rc);
		mosquitto_destroy(mosq);
		return -1;
	}

	mosquitto_publish( mosq, NULL, TOPIC, strlen(to_send), to_send, 0, false );

    mosquitto_loop_forever( mosq, -1, 1 );

	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();

	return 0;
}
