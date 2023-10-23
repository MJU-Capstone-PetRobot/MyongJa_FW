float deg2rad = M_PI / 180.0;
float rad2deg = 180.0 / M_PI;
float b1[3], b2[3], b3[3], p1[3], p2[3], p3[3];
float l1[3], l2[3], l3[3];
int L1_a, L2_a, L3_a;
float bRp0[3], bRp1[3], bRp2[3], T[3];
float z_set = 80;
float angle, theta, phi;
float phi0 = 30;
float d = 25.0;
float e = 60.0;
float z0 = 26.0; //모터 좌표
float k; //모터 좌표 반지름
int angle_step;
int yaw = 150;
int yaw_step = 512;

void init_neck_position()
{
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
}

float dot_product(float v[], float u[]) 
{
    float result = 0.0;
    int i;

    for (i = 0; i < 3; i++) 
    {
    result += v[i] * u[i];
    }
    return result;
}

void BRP(float theta, float phi) 
{
    bRp0[0] = cos(theta * deg2rad);
    bRp0[1] = sin(theta * deg2rad) * sin(phi * deg2rad);
    bRp0[2] = cos(phi * deg2rad) * sin(theta * deg2rad);
    bRp1[0] = 0;
    bRp1[1] = cos(phi * deg2rad);
    bRp1[2] = -sin(phi * deg2rad);
    bRp2[0] = -sin(theta * deg2rad);
    bRp2[1] = cos(theta * deg2rad) * sin(phi * deg2rad);
    bRp2[2] = cos(phi * deg2rad) * cos(theta * deg2rad);
}

void create_l_vectors(float _theta, float _phi, float _z_set) 
{
    BRP(_theta, _phi);

    T[0] = 0.0;
    T[1] = 0.0;
    T[2] = _z_set;
    l1[0] = T[0] + (dot_product(bRp0, p1)) - b1[0];
    l1[1] = T[1] + (dot_product(bRp1, p1)) - b1[1];
    l1[2] = T[2] + (dot_product(bRp2, p1)) - b1[2];
    l2[0] = T[0] + (dot_product(bRp0, p2)) - b2[0];
    l2[1] = T[1] + (dot_product(bRp1, p2)) - b2[1];
    l2[2] = T[2] + (dot_product(bRp2, p2)) - b2[2];
    l3[0] = T[0] + (dot_product(bRp0, p3)) - b3[0];
    l3[1] = T[1] + (dot_product(bRp1, p3)) - b3[1];
    l3[2] = T[2] + (dot_product(bRp2, p3)) - b3[2];
}

//length == 높이
int step_transform(float length) 
{
    if (length > 110.0) 
    {
        length = 110.0;  //최대길이
    }
    if (length < 40.0) 
    {
        length = 40.0;  //최소길이
    }
    float intermed = ((length * length) + (d * d) - (e * e)) / (2 * d * length);
    if (intermed >= 1) 
    {
        intermed = 0.99;
    }

    angle = rad2deg * acos(intermed) - 90.0 + phi0;
    angle_step = map(angle, 0, 300, 0, 1023);

    if (angle_step < 0) 
    {
        angle_step = 0;
    }

    return angle_step;
    //각각의 길이를 0~4095로 변환후 반환
}

void move_neck(float _theta, float _phi, float _z_set, float _yaw)
{
    create_l_vectors(_theta, _phi, _z_set); // create the end-effector vectors
    yaw_step = map(_yaw, -150, 150, 0, 1023);
    
    L1_a = step_transform(sqrt((l1[0] * l1[0]) + (l1[1] * l1[1]) + (l1[2] * l1[2])));  // norm and
    L2_a = step_transform(sqrt((l2[0] * l2[0]) + (l2[1] * l2[1]) + (l2[2] * l2[2])));  // convert to steps
    L3_a = step_transform(sqrt((l3[0] * l3[0]) + (l3[1] * l3[1]) + (l3[2] * l3[2])));

    ax12a.move(ID1, L1_a);
    ax12a.move(ID2, L2_a);
    ax12a.move(ID3, L3_a);
    ax12a.move(ID4, yaw_step);
}