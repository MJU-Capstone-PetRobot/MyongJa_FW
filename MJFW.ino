#define DirectionPin  (10u)
#define BaudRate      (1000000ul)
#define ID1            1
#define ID2            2
#define ID3            3
#define ID4            4
#define LEFT_EYE  9
#define RIGHT_EYE 10

#include <AX12A.h>
#include <math.h>
#include <4dof.h>


void setup() {
  //초기 위치 설정
  theta = 0.0;
  phi = 0.0;
  k = 100.0 / sin(60 * deg2rad);
  b1[0] = k;                       // body origin to motor 1 shaft center X position
  b1[1] = 0.0;                     // body origin to motor 1 shaft center Y position
  b1[2] = z0;                      // body origin to motor 1 shaft center Z position
  b2[0] = -k * sin(30 * deg2rad);  // same for motor 2
  b2[1] = k * cos(30 * deg2rad);
  b2[2] = z0;
  b3[0] = -k * sin(30 * deg2rad);  // same for motor 3
  b3[1] = -k * cos(30 * deg2rad);
  b3[2] = z0;
  p1[0] = k;                       // platform origin to effector pivot vector X position
  p1[1] = 0.0;                     // platform origin to effector pivot vector Y position
  p1[2] = 0.0;                     // platform origin to effector pivot vector Z position
  p2[0] = -k * sin(30 * deg2rad);  // same for second effector pivot point
  p2[1] = k * cos(30 * deg2rad);
  p2[2] = 0.0;
  p3[0] = -k * sin(30 * deg2rad);  // same for third effector pivot point
  p3[1] = -k * cos(30 * deg2rad);
  p3[2] = 0.0;
  Serial1.begin(1000000,SERIAL_8N1, 2, 1);  //tx 1

  // put your setup code here, to run once:
  Serial.begin(115200);  // 통신 속도
  ax12a.begin(BaudRate, DirectionPin, &Serial1);
  delay(1000);
}

void loop() {

  // Serial.read()
  if (Serial.available() <= 0) {
    return;
  }
  char ch = Serial.read();
  RCVdata += ch;
  if (ch == '\n') {

    if (RCVdata.length() > 0) {
      int index;
      int tmpcnt = 0;
      String tmpString = RCVdata;
      tmpString.trim();
      Serial.print("command in rx, ry, z, rz  ");
      Serial.println(tmpString);

      while (tmpString.length() > 0) {
        index = tmpString.indexOf(",");
        if (index == -1) {
          CMDdataDEG[tmpcnt] = tmpString;
          CMDdataDEG[tmpcnt].trim();
          tmpcnt++;
          break;
        }

        CMDdataDEG[tmpcnt] = tmpString.substring(0, index);
        tmpString = tmpString.substring(index + 1);
        tmpString.trim();
        CMDdataDEG[tmpcnt].trim();
        tmpcnt++;
      }
    }

    // 파이썬으로 부터 데이터 받음.
    theta = CMDdataDEG[0].toFloat(); //roll
    phi = CMDdataDEG[1].toFloat(); //pitch
    z_set = CMDdataDEG[2].toFloat(); //z-distance
    yaw = CMDdataDEG[3].toFloat(); //z-axis yaw
    yaw_step = map(yaw, -150, 150, 0, 1023);
    // 역기구학 계산
    create_l_vectors();                                                                // create the end-effector vectors
    L1_a = step_transform(sqrt((l1[0] * l1[0]) + (l1[1] * l1[1]) + (l1[2] * l1[2])));  // norm and
    L2_a = step_transform(sqrt((l2[0] * l2[0]) + (l2[1] * l2[1]) + (l2[2] * l2[2])));  // convert to steps
    L3_a = step_transform(sqrt((l3[0] * l3[0]) + (l3[1] * l3[1]) + (l3[2] * l3[2])));



    RCVdata = "";
    
    Serial.printf("%d ,%d, %d, %d", L1_a, L2_a, L3_a, yaw_step);

    ax12a.move(ID1, L1_a);
    ax12a.move(ID2, L2_a);
    ax12a.move(ID3, L3_a);
    ax12a.move(ID4, yaw_step);
  }
}
