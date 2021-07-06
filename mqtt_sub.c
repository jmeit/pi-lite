#include <stdio.h>
#include <stdlib.h>

#include <mosquitto.h>

#define MQTT_HOST   "mqtt.bassface.space"
#define MQTT_PORT   8883
#define TARGET_USER "wildboard"
#define TARGET_PASS "wilderness user"
#define CA_PATH     "/etc/ssl/certs"
#define KEEP_ALIVE  10
#define CLEAN_SESS  true
#define TOPIC       "subtest"

void on_connect(struct mosquitto *mosq, void *obj, int rc) {
	printf("ID: %d\n", * (int *) obj);
	if(rc) {
		printf("Error with result code: %d\n", rc);
		exit(-1);
	}
	mosquitto_subscribe(mosq, NULL, TOPIC, 0);
}

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg) {
	printf("New message with topic %s: %s\n", msg->topic, (char *) msg->payload);
}

int main() {
	int rc, id=1;

	mosquitto_lib_init();

	struct mosquitto *mosq;

	mosq = mosquitto_new( "client-sub", CLEAN_SESS, NULL );
	mosquitto_connect_callback_set( mosq, on_connect );
	mosquitto_message_callback_set( mosq, on_message );

    mosquitto_username_pw_set( mosq, TARGET_USER, TARGET_PASS );
    mosquitto_tls_insecure_set( mosq, false );
    mosquitto_tls_opts_set( mosq, 1, NULL, NULL );
    mosquitto_tls_set( mosq, NULL, CA_PATH, NULL, NULL, NULL );

	rc = mosquitto_connect( mosq, MQTT_HOST, MQTT_PORT, KEEP_ALIVE );
	if(rc) {
		printf("Could not connect to Broker with return code %d\n", rc);
		return -1;
	}

	mosquitto_loop_start(mosq);
	printf("Press Enter to quit...\n");
	getchar();
	mosquitto_loop_stop(mosq, true);

	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();

	return 0;
}
