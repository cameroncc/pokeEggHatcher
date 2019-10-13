/* Output pins for the solenoid */
#define OUTPUT_1 2
#define OUTPUT_2 3

/* Input pins to set the delay time */
// We will always set bit 1 to LOW in order to make another pin available
#define IN_BIT_2 5
#define IN_BIT_3 6
#define IN_BIT_4 7
#define IN_BIT_5 8
#define IN_BIT_6 9
#define IN_BIT_7 10
#define IN_BIT_8 11
#define IN_BIT_9 12

/* Input to determine whether to read new delay time */
#define READ 13

/* Input to determine whether to run the bouncer */
#define ENABLE 4

/* Set up max runtime */
#define MAX_RUNTIME_MIN 90

/* Time control vars */
unsigned int delayTime;
unsigned int ms;
unsigned int seconds;
unsigned int minutes;

void calcRuntime();
unsigned int readInput();

void setup()
{
  delayTime = 228; // This seems good for bouncing an iPhone 6s-8
  ms = 0;
  seconds = 0;
  minutes = 0;
  
  pinMode(OUTPUT_1, OUTPUT);
  pinMode(OUTPUT_2, OUTPUT);
  pinMode(ENABLE, INPUT);
  pinMode(IN_BIT_2, INPUT);
  pinMode(IN_BIT_3, INPUT);
  pinMode(IN_BIT_4, INPUT);
  pinMode(IN_BIT_5, INPUT);
  pinMode(IN_BIT_6, INPUT);
  pinMode(IN_BIT_7, INPUT);
  pinMode(IN_BIT_8, INPUT);
  pinMode(IN_BIT_9, INPUT);
  pinMode(READ, INPUT);
}

void loop()
{
  bool enabled = digitalRead(ENABLE);
  
  // Setting this pin HIGH allows infinite runtime
  if(digitalRead(READ) == HIGH) {
    ms = 0;
    seconds = 0;
    minutes = 0;
    delayTime = readInput();
  }
  if(enabled && minutes < MAX_RUNTIME_MIN) {
    digitalWrite(OUTPUT_1, HIGH);
    digitalWrite(OUTPUT_2, LOW);
    delay(delayTime);
    digitalWrite(OUTPUT_1, LOW);
    digitalWrite(OUTPUT_2, HIGH);
    delay(delayTime);

    calcRuntime();
  }
  else if(!enabled && minutes >= MAX_RUNTIME_MIN) {
    ms = 0;
    seconds = 0;
    minutes = 0;
  }
}

void calcRuntime()
{
  ms += (2 * delayTime);
  if(ms >= 1000) {
    ms = 0;
    ++seconds;
  }
  if(seconds >= 60) {
    seconds = 0;
    ++minutes;
  }
}

unsigned int readInput()
{
  byte input[] = { digitalRead(IN_BIT_9),
                   digitalRead(IN_BIT_8),
                   digitalRead(IN_BIT_7),
                   digitalRead(IN_BIT_6),
                   digitalRead(IN_BIT_5),
                   digitalRead(IN_BIT_4),
                   digitalRead(IN_BIT_3),
                   digitalRead(IN_BIT_2),
                   LOW }; // LSB is permanantly set LOW to free up a pin
  unsigned int num = 0;
  byte size = sizeof(input) / sizeof(byte);
  for (byte i = 0; i < size; ++i) {
        unsigned int temp = (input[i] << (size - i - 1));
        num += temp;
  }
  return num;
}
