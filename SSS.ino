#include<SoftwareSerial.h>
SoftwareSerial GCS(5, 6);
unsigned long prevMill = 0;
double latN,
      lonN,
      latB,
      lonB,
      jarak,
      head,
      bearing,
      error,
      sSpeed;
int rSpeed, Start, Failsafe;
float WP[20], tuning[4];
const byte dataByte = 255;
char dataRec[dataByte];
char tempChar[dataByte];
char dataGCS[dataByte] = {0};
bool newData = false;
int parsing;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  GCS.begin(57600);
}

void loop() {
  // put your main code here, to run repeatedly:
  recData();
  if(Start==0)rSpeed=1000;
  if (millis() - prevMill >= 1000) {
    prevMill = millis();
        latB = latN;
        lonB = lonN;
    sendData();
  }
}

void recData() {
  rec();
  if (newData == true) {
    strcpy(tempChar, dataRec);
    if (parsing == 1) {
      recWaypoint();
    }
    else if (parsing == 2) {
      recSpeed();
    }
    else if (parsing == 3) {
      recStart();
    }
    else if (parsing == 4) {
      recFailsafe();
    }
    else if (parsing == 5) {
      recTuning();
    }
    newData = false;
  }
}

void rec() {
  static bool recData = false;
  static byte ndx;
  char endline = '>';
  char c;

  while (GCS.available() > 0 && newData == false) {
    c = GCS.read();

    if (recData == true) {
      if (c != endline) {
        dataRec[ndx] = c;
        ndx++;
        if (ndx >= dataByte) {
          ndx = dataByte - 1;
        }
      }
      else {
        dataRec[ndx] = '\0';
        recData = false;
        ndx = 0;
        newData = true;
      }
    }
    else if (c == '!') {
      recData = true;
      parsing = 1;
      //Parse Waypoint
    }
    else if (c == '@') {
      recData = true;
      parsing = 2;
      //Parse Speed
    }
    else if (c == '#') {
      recData = true;
      parsing = 3;
      //Parse Start
    }
    else if (c == '$') {
      recData = true;
      parsing = 4;
      //Parse Failsafe
    }
    else if (c == '^') {
      recData = true;
      parsing = 5;
      //Parse Tuning
    }
  }
}

void recWaypoint() {
  char * strtokIndx;

  strtokIndx = strtok(tempChar, "|");
  strcpy(dataGCS, strtokIndx);

  strtokIndx = strtok(NULL, "|");
  WP[0] = atof(strtokIndx);

  strtokIndx = strtok(NULL, "|");
  WP[1] = atof(strtokIndx);

  strtokIndx = strtok(NULL, "|");
  WP[2] = atof(strtokIndx);

  strtokIndx = strtok(NULL, "|");
  WP[3] = atof(strtokIndx);

  strtokIndx = strtok(NULL, "|");
  WP[4] = atof(strtokIndx);

  strtokIndx = strtok(NULL, "|");
  WP[5] = atof(strtokIndx);

  strtokIndx = strtok(NULL, "|");
  WP[6] = atof(strtokIndx);

  strtokIndx = strtok(NULL, "|");
  WP[7] = atof(strtokIndx);

  strtokIndx = strtok(NULL, "|");
  WP[8] = atof(strtokIndx);

  strtokIndx = strtok(NULL, "|");
  WP[9] = atof(strtokIndx);

  strtokIndx = strtok(NULL, "|");
  WP[10] = atof(strtokIndx);

  strtokIndx = strtok(NULL, "|");
  WP[11] = atof(strtokIndx);

  strtokIndx = strtok(NULL, "|");
  WP[12] = atof(strtokIndx);

  strtokIndx = strtok(NULL, "|");
  WP[13] = atof(strtokIndx);
}

void recSpeed() {
  char * strtokIndx;
  strtokIndx = strtok(tempChar, "|");
  strcpy(dataGCS, strtokIndx);
  strtokIndx = strtok(NULL, "|");
  rSpeed = atof(strtokIndx);
}

void recStart() {
  char * strtokIndx;
  strtokIndx = strtok(tempChar, "|");
  strcpy(dataGCS, strtokIndx);
  strtokIndx = strtok(NULL, "|");
  Start = atof(strtokIndx);
}

void recFailsafe() {
  char * strtokIndx;
  strtokIndx = strtok(tempChar, "|");
  strcpy(dataGCS, strtokIndx);
  strtokIndx = strtok(NULL, "|");
  Failsafe = atof(strtokIndx);
}

void recTuning() {
  char * strtokIndx;

  strtokIndx = strtok(tempChar, "|");
  strcpy(dataGCS, strtokIndx);

  strtokIndx = strtok(NULL, "|");
  tuning[0] = atof(strtokIndx);//kp value

  strtokIndx = strtok(NULL, "|");
  tuning[1] = atof(strtokIndx);//ki value

  strtokIndx = strtok(NULL, "|");
  tuning[2] = atof(strtokIndx);//kd value

  strtokIndx = strtok(NULL, "|");
  tuning[3] = atof(strtokIndx);//sp value

}

///////////////////////////////////////////////////////////////////////////////////////
void sendData() {
  GCS.print("#DAT");
  GCS.print(",");
  GCS.print(latN, 8);
  GCS.print(",");
  GCS.print(lonN, 8);
  GCS.print(",");
  GCS.print(latB, 8);
  GCS.print(",");
  GCS.print(lonB, 8);
  GCS.print(",");
  GCS.print(head);
  GCS.print(",");
  GCS.print(bearing);
  GCS.print(",");
  GCS.print(error);
  GCS.print(",");
  GCS.print(jarak);
  GCS.print(",");
  GCS.print(rSpeed);
  GCS.print(",");
  GCS.print(Start);
  GCS.print(",");
  GCS.print(tuning[0]);
  GCS.print(",");
  GCS.print(tuning[1]);
  GCS.print(",");
  GCS.print(tuning[2]);
  GCS.print(",");
  GCS.print(tuning[3]);
  GCS.print(",");
  GCS.print(Failsafe);
  GCS.print(",");
  GCS.print("!");
  GCS.println();
}
