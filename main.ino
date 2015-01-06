#define SPEAKER_PIN 2
#define MIC_PIN 2

#define READ_CAP 500

//char sensorReads[READ_CAP];
//unsigned int sensorReadCount = 0;

unsigned int current_read = NULL;
unsigned int running_avg = NULL;
bool find_peak = false;
unsigned long last_ping = 0;
unsigned long response_time = 0;

void click();

void setup() {
    Serial.begin(115200);
    pinMode(SPEAKER_PIN, OUTPUT);
    digitalWrite(SPEAKER_PIN, LOW);
}

void loop() {
	if (running_avg == NULL) {
		click();

		delayMicroseconds(100);
	}

	current_read = analogRead(MIC_PIN);

	if (running_avg == NULL) {
		running_avg = current_read;
	} else {
		running_avg = (int)((running_avg + current_read) / 2);
	}

	if (!find_peak) {
		if (current_read > running_avg && !find_peak) {
			find_peak = true;
		}
	} else {
		if (running_avg > current_read) {
			// response peak found
			unsigned int delay_time = micros() - last_ping;

			Serial.print("Delay: ");
			Serial.println(delay_time);

			running_avg = NULL;
			find_peak = false;

			delay(1000);
		}
	}

	if (micros() - last_ping > 10000) {
		click();
	}
}

void click() {
    digitalWrite(SPEAKER_PIN, HIGH);
    delayMicroseconds(40);
    digitalWrite(SPEAKER_PIN, LOW);

    last_ping = micros();
}
