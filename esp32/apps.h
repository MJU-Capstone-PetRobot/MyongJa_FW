#include <string>

typedef struct
{
    float RPZY[4];
    int emo_code;

    int touch;
    int co_value;
    int distance;
    int bat_percent;
    int bat_hour;

} ESP32_DATA;

ESP32_DATA myoungja;

void init_default_value()
{
    memset(&myoungja, 0, sizeof(myoungja));
}

void send_to_opi()
{
    // pid 1이면 
    // tx 플래그
}

void receive_from_opi()
{
    if (Serial.available() <= 0) return;

    char ch = Serial.read();
    static String rx_str = "";
    static String neck_cmd_str[4];
    static bool sof = false;
    static bool eof = false;
    static bool error = false;

    rx_str += ch;
    rx_str.trim(); // delete '\n'

    if(ch == '[') sof = true;
    if((sof == true) && (ch == ']')) eof = true;

    if(sof && eof) // 전체 패킷 수신 완료
    {
        Serial.print("[OPI->ESP] ");
        Serial.println(rx_str);

        if(rx_str[1] == 'N')
        {
            int index;
            int cnt = 0;
            rx_str.replace("[", ""); // delete '['
            rx_str.replace("N", ""); // delete 'N'
            rx_str.replace("]", ""); // delete ']'

            Serial.print("[NECK] [R,P,Z,Y] ");
            Serial.printf("[%s]", rx_str);

            while(cnt != 4)
            {
                index = rx_str.indexOf(",");
                if(index == -1)
                {
                    if(cnt == 3)
                    {
                        neck_cmd_str[cnt] = rx_str;
                        cnt++;
                        error = false;                    
                    }
                    else 
                    {
                        error = true;
                        break;
                    }
                }
                else
                {
                    neck_cmd_str[cnt] = rx_str.substring(0, index);
                    rx_str = rx_str.substring(index + 1);
                    cnt++;
                }
            }

            if(error)
                Serial.println("[ERROR]");
            else
            {
                myoungja.RPZY[0] = neck_cmd_str[0].toFloat(); //roll
                myoungja.RPZY[1] = neck_cmd_str[1].toFloat(); //pitch
                myoungja.RPZY[2] = neck_cmd_str[2].toFloat(); //z-distance
                myoungja.RPZY[3] = neck_cmd_str[3].toFloat(); //z-axis yaw

                move_neck(myoungja.RPZY[0], myoungja.RPZY[1], myoungja.RPZY[2], myoungja.RPZY[3]);
            }
        }
        else if(rx_str[1] == 'E')
        {
            int emo_code;
            rx_str.replace("[", "");
            rx_str.replace("E", "");
            rx_str.replace("]", "");

            emo_code = rx_str.toInt();
            displayEyes(emo_code);
        }
        else
        {

        }

        rx_str = "";
        sof = false;
        eof = false;
    }
}

void receive_from_esp_s()
{

}
