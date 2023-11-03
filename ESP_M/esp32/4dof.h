const float deg2rad = M_PI / 180.0;
const float rad2deg = 180.0 / M_PI;
const float z_set = 80;
const float phi0 = 30;
const float d = 25.0;
const float e = 60.0;
const float z0 = 26.0;  // Motor coordinate
// Global variables to hold the current and target positions
float current_theta = 0.0;
float current_phi = 0.0;
float current_z_set = z_set;

float target_theta = 0.0;
float target_phi = 0.0;
float current_y_set = 0.0;  // Current yaw position
float target_y_set = 0.0;   // Target yaw position
float target_z_set = z_set;

const float increment_value = 0.1;  // Adjust this value to change the smoothing speed



struct Vector3 {
  float x, y, z;

  Vector3()
    : x(0), y(0), z(0) {}
  Vector3(float x, float y, float z)
    : x(x), y(y), z(z) {}

  float magnitude() const {
    return sqrt(x * x + y * y + z * z);
  }
};

Vector3 b1, b2, b3, p1, p2, p3;
Vector3 l1, l2, l3;
Vector3 bRp0, bRp1, bRp2, T;

int L1_a, L2_a, L3_a;
float angle, theta, phi;
float k;  // Motor coordinate radius
int angle_step;
int yaws;


void init_neck_position() {
  theta = phi = 0.0;
  k = 100.0 / sin(60 * deg2rad);

  b1 = { k, 0.0, z0 };
  b2 = { -k * sin(30 * deg2rad), k * cos(30 * deg2rad), z0 };
  b3 = { -k * sin(30 * deg2rad), -k * cos(30 * deg2rad), z0 };

  p1 = { k, 0.0, 0.0 };
  p2 = { -k * sin(30 * deg2rad), k * cos(30 * deg2rad), 0.0 };
  p3 = { -k * sin(30 * deg2rad), -k * cos(30 * deg2rad), 0.0 };
}

float dot_product(const Vector3& v, const Vector3& u) {
  return v.x * u.x + v.y * u.y + v.z * u.z;
}

void BRP(float theta, float phi) {
  bRp0 = { cos(theta * deg2rad), sin(theta * deg2rad) * sin(phi * deg2rad), cos(phi * deg2rad) * sin(theta * deg2rad) };
  bRp1 = { 0, cos(phi * deg2rad), -sin(phi * deg2rad) };
  bRp2 = { -sin(theta * deg2rad), cos(theta * deg2rad) * sin(phi * deg2rad), cos(phi * deg2rad) * cos(theta * deg2rad) };
}

void create_l_vectors(float _theta, float _phi, float _z_set) {
  BRP(_theta, _phi);

  T = { 0.0, 0.0, _z_set };
  l1 = { T.x + dot_product(bRp0, p1) - b1.x, T.y + dot_product(bRp1, p1) - b1.y, T.z + dot_product(bRp2, p1) - b1.z };
  l2 = { T.x + dot_product(bRp0, p2) - b2.x, T.y + dot_product(bRp1, p2) - b2.y, T.z + dot_product(bRp2, p2) - b2.z };
  l3 = { T.x + dot_product(bRp0, p3) - b3.x, T.y + dot_product(bRp1, p3) - b3.y, T.z + dot_product(bRp2, p3) - b3.z };
}

int step_transform(float length) {
  length = constrain(length, 40.0, 110.0);

  float intermed = ((length * length) + (d * d) - (e * e)) / (2 * d * length);
  intermed = constrain(intermed, -0.99, 0.99);

  angle = rad2deg * acos(intermed) - 90.0 + phi0;
  angle_step = map(angle, 0, 300, 0, 1023);

  return max(angle_step, 0);
}

int cal_yawstep(float yaw) {

  int yaw_step = map(yaw+1.0, -3.14, 3.14, 204, 820);
  return yaw_step;
}


void move_neck(float _theta, float _phi, float _y_set, float _z_set) {

  // Directly update the current positions to the targets
  current_theta = _theta;
  current_phi = _phi;
  current_z_set = _z_set;
  current_y_set = _y_set;

  // Now move to the updated position
  create_l_vectors(current_theta, current_phi, current_z_set);

  // Transform the lengths into servo steps
  L1_a = step_transform(l1.magnitude());
  L2_a = step_transform(l2.magnitude());
  L3_a = step_transform(l3.magnitude());
  yaws = cal_yawstep(current_y_set); // Use the updated current_y_set

  // Command the servo motors to move to the calculated positions
  ax12a.moveSpeed(ID4, yaws,100);
  ax12a.moveSpeed(ID1, L1_a,100);
  ax12a.moveSpeed(ID2, L2_a,100);
  ax12a.moveSpeed(ID3, L3_a,100);

}
