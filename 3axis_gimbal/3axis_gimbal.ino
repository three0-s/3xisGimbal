//2022-1 Semester Yonsei-RoboIN 3 Axis Gymbal Project. 
//@author: ga06033@yonsei.ac.kr
//@date: 2022-06-23
// test dohyeon

#include "mpu6050_wrapper.h"
#include "dcmotor_control.h"

#define LOOP_SIZE 10

float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

extern double setPoint[3];
extern double cumError[3];
extern double rateError[3];
extern volatile bool mpuInterrupt;
extern uint8_t devStatus;
int loopcount = 0;
extern MPU6050 mpu;
double pwm_delay_coordi_buffer[30][3] = {0, };


void setup() {
    // baud rate = 115200
    init_mpu();
    roll_motor_attach();
    get_ypr(ypr);
    for(int i=0; i < 3; i++){
        cumError[i] = 0;
        rateError[i] = 0;
        setPoint[i] = 0;
    }
    run_roll_motor(0);
    Serial.println("Setup Ready!");
}

// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop() {
    // get_min_delay(MOTOR_CLASS::ROLL, pwm_delay_coordi_buffer);
    // for(int i=0; i < 17; i++){
    //     Serial.print(pwm_delay_coordi_buffer[i][0]); Serial.print(", "); Serial.print(pwm_delay_coordi_buffer[i][1]);
    //     Serial.print("| DT_ANGLE "); Serial.println(pwm_delay_coordi_buffer[i][2]);
    // }
    // while(1);
    get_ypr(ypr);
    if(loopcount++ > LOOP_SIZE){
        loopcount=0;
        for(int i=0; i < 3; i++){
            cumError[i]=0;
        }
    }

    Serial.print("yaw | "); Serial.println(ypr[0]);
    Serial.print("pitch | "); Serial.println(ypr[1]);
    Serial.print("roll | "); Serial.println(ypr[2]); 
    // pitch control
    int pwm = computePID(ypr[2], setPoint[2]);
    Serial.print("pwm | "); Serial.println(pwm); Serial.println("");
    // run_roll_motor(pwm);
    delay(20);
    mpu.resetFIFO();
    // Serial.print(devStatus); Serial.println("\n");
}
// original