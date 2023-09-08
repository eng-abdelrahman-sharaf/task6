// from datasheet
// https://pdf1.alldatasheet.com/datasheet-pdf/view/1132807/TDK/MPU-6050.html

//macros 
#define mpu_address 0x68
#define GYRO_CONFIG 0x1B
#define PWR_MGMT_1_register 0x6B
#define GYRO_ZOUT_H 0x47
#define samples_num 3000

#include <Wire.h>

// // global variables only used in loop function
// int yaw = -mpu_read2bytes_yaw() //to save 
int yaw_offset;
int yaw = 0;
int prev_yaw = 0;
double time = 0;
double prev_time = 0;
float degree_angle = 0;
double time_passed = 0;

// functions declaration
void mpu_gyroconfig();
void mpu_init();
int16_t mpu_read2bytes_yaw();
int get_yaw_offset();  

////////////////////////////////////////////////////////////////////////

// main program
void setup(){
  Wire.begin();
  mpu_init();
  mpu_gyroconfig();
  yaw_offset = get_yaw_offset();
  Serial.begin(9600);
}

void loop(){
  
  // yaw = the real instantaneous angular velocity about z axis 
  yaw = mpu_read2bytes_yaw() - yaw_offset;
  time = millis() *1.0 / 1000;
  
  // to get the time passed
  time_passed += time;
  
  // this is the area of the trapezoid made by the yaw (y) and the time (dt)   = 0.5 * (y1 + y2) * dt 
  degree_angle = degree_angle + (yaw + prev_yaw) * 0.5 * (time - prev_time);
  
  // to make the degree in the range from [-360 , 360]
  while (degree_angle>=360) 
    degree_angle -= 360;
  while (degree_angle <=-360) 
    degree_angle += 360;

  //printing the output to the user
  Serial.print("the angle : ");
  Serial.print(degree_angle);
  Serial.print(" time passed : ");
  Serial.print(time_passed);
  Serial.print(" dt : ");
  Serial.println(time - prev_time);

  prev_time = time;
  prev_yaw = yaw;
  delay(100);
  
}
////////////////////////////////////////////////////////////////////////

// functions definitions



/**
 * @brief  gives you the angle velocity around the z axis
 * 
 * @return int16_t the yaw value (angular velocity) 
 */
int16_t mpu_read2bytes_yaw(){
  int16_t data = 0;
  double real_data;

  Wire.beginTransmission(mpu_address);
  Wire.write(GYRO_ZOUT_H);
  Wire.endTransmission();
  
  Wire.requestFrom(mpu_address , 2);
  
  while(Wire.available() < 2);

  // write.read() gives you 1 byte 
  // 1byte (e.g. 0x1B)<< 8 | 1byte (e.g. 0x0F) ==> combinig the 2 digits (e.g. 0x1B0F)
  data = Wire.read() << 8 | Wire.read();
  
  // according to the config
  real_data = map (data , -32767 , 32766 , -250 , 250);
  return real_data;
}



/**
 * @brief mpu activation
 * 
 * 
 */
void mpu_init(){
  /*
  Register           | 7            | 6     | 5     |   4   |     3    | 2 | 1 | 0 |
  PWR_MGMT_1 (0x6B)  | DEVICE_RESET | SLEEP | CYCLE |   -   | TEMP_DIS | CLKSEL[2:0]
  */
  Wire.beginTransmission(mpu_address);
  Wire.write(PWR_MGMT_1_register);
  Wire.write(0x00); //0b0000 0000
  Wire.endTransmission();
}





/**
 * @brief setting the full scale range to `+-2000 deg/s`
 * 
 * 
 */
void mpu_gyroconfig(){
  Wire.beginTransmission(mpu_address);
  // deploying gyroscope to be able to read it
  Wire.write(GYRO_CONFIG);
  
  // now we set the full scale range to +-250 deg/s (from data sheet also) 
  /*
  Register           | 7     | 6     | 5     |   4  |   3   | 2 | 1 | 0 |
  GYRO_CONFIG (0x1B) | XG_ST | YG_ST | ZG_ST | FS_SEL [1:0] | - | - | - |
  
  | FS_SEL | Full Scale Range |
  |--------|------------------|
  | 0      | +-250 deg/s      |
  | 1      | +-500 deg/s      |
  | 2      | +-1000 deg/s     |
  | 3      | +-2000 deg/s     |  
  */

  //!!!!any change to this config should appear in the map in finding yaw
  Wire.write(0x00);// 0b0000 0000
  Wire.endTransmission();

}




/**
 * @brief gives you the offset
 * 
 * @return int  
 */
int get_yaw_offset(){
  long long sum = 0;
  
  // get the sum of readings 
  for (int i = 0 ; i< samples_num ; i++){
    sum += mpu_read2bytes_yaw();
 
  }
 
  // return the average of the readings as the offset 
  return sum / samples_num;
  
}


