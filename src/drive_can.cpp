#define Phoenix_No_WPI // remove WPI dependencies
#include "ctre/Phoenix.h"
#include "ctre/phoenix/platform/Platform.h"
#include "ctre/phoenix/unmanaged/Unmanaged.h"
#include "ctre/phoenix/cci/Unmanaged_CCI.h"
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <unistd.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Float64.h>
#include <signal.h>
#include <unistd.h>

using namespace ctre::phoenix;
using namespace ctre::phoenix::platform;
using namespace ctre::phoenix::motorcontrol;
using namespace ctre::phoenix::motorcontrol::can;

bool start_config = true;

/* make some talons for arm train */
TalonSRX srxArm1(11);
TalonSRX srxArm2(12);
TalonSRX srxArm3(13);
TalonSRX srxArm4(14);
TalonSRX srxArm5(15); //Gripper
TalonSRX srxArm6(16); //Prismatico
TalonSRX srxArmLab(21);//POR PONER

/* make some talons for drive train */
TalonFX talFrontLeft(0);
TalonFX talFrontRight(2);
TalonFX talBackLeft(1);
TalonFX talBackRight(3);


double my_map(double x, double in_min, double in_max, double out_min, double out_max){
        double targetPos = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
        return targetPos;
}


void initDrive()
{
/* Factory default hardware to prevent unexpected behavior */
srxArm1.ConfigFactoryDefault();
srxArm2.ConfigFactoryDefault();
srxArm3.ConfigFactoryDefault();
srxArm4.ConfigFactoryDefault();
srxArm5.ConfigFactoryDefault();
srxArm6.ConfigFactoryDefault();

srxArmLab.ConfigFactoryDefault();


srxArm1.SetInverted(false);
srxArm2.SetInverted(false);
srxArm3.SetInverted(false);
srxArm4.SetInverted(false);

srxArmLab.SetInverted(false);

srxArm1.SetSensorPhase(false);
srxArm2.SetSensorPhase(false);
srxArm3.SetSensorPhase(false);
srxArm4.SetSensorPhase(false);

srxArmLab.SetSensorPhase(false);


/* choose the sensor and sensor direction */
srxArm1.ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute, 0, 10);
srxArm2.ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute, 0, 10);
srxArm3.ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute, 0, 10);
srxArm4.ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute, 0, 10);

srxArmLab.ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute, 0, 10);

//srxArm1.SetSelectedSensorPosition(offset1, 0, 10);


/* Set the peak and nominal outputs */
srxArm1.ConfigNominalOutputForward(0, 10);
srxArm1.ConfigNominalOutputReverse(0, 10);
srxArm1.ConfigPeakOutputForward(1, 10);
srxArm1.ConfigPeakOutputReverse(-1, 10);

srxArm2.ConfigNominalOutputForward(0, 10);
srxArm2.ConfigNominalOutputReverse(0, 10);
srxArm2.ConfigPeakOutputForward(1, 10);
srxArm2.ConfigPeakOutputReverse(-1, 10);

srxArm3.ConfigNominalOutputForward(0, 10);
srxArm3.ConfigNominalOutputReverse(0, 10);
srxArm3.ConfigPeakOutputForward(1, 10);
srxArm3.ConfigPeakOutputReverse(-1, 10);

srxArm4.ConfigNominalOutputForward(0, 10);
srxArm4.ConfigNominalOutputReverse(0, 10);
srxArm4.ConfigPeakOutputForward(1, 10);
srxArm4.ConfigPeakOutputReverse(-1, 10);

srxArm5.ConfigNominalOutputForward(0, 10);
srxArm5.ConfigNominalOutputReverse(0, 10);
srxArm5.ConfigPeakOutputForward(1, 10);
srxArm5.ConfigPeakOutputReverse(-1, 10);

srxArm6.ConfigNominalOutputForward(0, 10);
srxArm6.ConfigNominalOutputReverse(0, 10);
srxArm6.ConfigPeakOutputForward(1, 10);
srxArm6.ConfigPeakOutputReverse(-1, 10);


srxArmLab.ConfigNominalOutputForward(0, 10);
srxArmLab.ConfigNominalOutputReverse(0, 10);
srxArmLab.ConfigPeakOutputForward(1, 10);
srxArmLab.ConfigPeakOutputReverse(-1, 10);

/* set closed loop gains in slot0, editar valor de en medio */ 
/* Set acceleration and vcruise velocity - see documentation */
srxArm1.Config_kF(0, 82.25, 10);
srxArm1.Config_kP(0, 5.52972973, 10); 
srxArm1.Config_kI(0, 0.001, 10);
srxArm1.Config_kD(0, 55.2972973, 10);
srxArm1.ConfigMotionCruiseVelocity(9, 10);
srxArm1.ConfigMotionAcceleration(9, 10);

/* set closed loop gains in slot0, editar valor de en medio */ 
/* Set acceleration and vcruise velocity - see documentation */
srxArm2.Config_kF(0, 102.3, 10);
srxArm2.Config_kP(0, 1.967307692, 10); 
srxArm2.Config_kI(0, 0.005, 10);
srxArm2.Config_kD(0, 19.67307692, 10);
srxArm2.ConfigMotionCruiseVelocity(8, 10);
srxArm2.ConfigMotionAcceleration(7.5, 10);

/* set closed loop gains in slot0, editar valor de en medio */ 
/* Set acceleration and vcruise velocity - see documentation */
srxArm3.Config_kF(0, 46.5, 10);
srxArm3.Config_kP(0, 1.794736842, 10); 
srxArm3.Config_kI(0, 0.001, 10);
srxArm3.Config_kD(0, 17.94736842, 10);
srxArm3.ConfigMotionCruiseVelocity(17, 10);
srxArm3.ConfigMotionAcceleration(16.5, 10);

/* set closed loop gains in slot0, editar valor de en medio */ 
/* Set acceleration and vcruise velocity - see documentation */
srxArm4.Config_kF(0, 34.1, 10);
srxArm4.Config_kP(0, 6.2, 10); 
srxArm4.Config_kI(0, 0.001, 10);
srxArm4.Config_kD(0, 62.0, 10);
srxArm4.ConfigMotionCruiseVelocity(23, 10);
srxArm4.ConfigMotionAcceleration(22.5, 10);


srxArmLab.Config_kF(0, 34.1, 0);
srxArmLab.Config_kP(0, 9.742857143, 0); 
srxArmLab.Config_kI(0, 0.001, 0);
srxArmLab.Config_kD(0, 97.42857143, 0);
srxArmLab.ConfigMotionCruiseVelocity(23, 0);
srxArmLab.ConfigMotionAcceleration(22.5, 0);


/* Factory default hardware to prevent unexpected behavior */
talFrontLeft.ConfigFactoryDefault();
talFrontRight.ConfigFactoryDefault();
talBackLeft.ConfigFactoryDefault();
talBackRight.ConfigFactoryDefault();



/* Configure Sensor Source for Pirmary PID */
talFrontLeft.ConfigSelectedFeedbackSensor(FeedbackDevice::IntegratedSensor, 0, 10);
talFrontRight.ConfigSelectedFeedbackSensor(FeedbackDevice::IntegratedSensor, 0, 10);
talBackLeft.ConfigSelectedFeedbackSensor(FeedbackDevice::IntegratedSensor, 0, 10);
talBackRight.ConfigSelectedFeedbackSensor(FeedbackDevice::IntegratedSensor, 0, 10);

/**
* Configure Talon SRX Output and Sensor direction accordingly
* Invert Motor to have green LEDs when driving Talon Forward / Requesting Postiive Output
* Phase sensor to have positive increment when driving Talon Forward (Green LED)
*/
talFrontLeft.SetSensorPhase(false);
talFrontLeft.SetInverted(false);
talFrontRight.SetSensorPhase(true);
talFrontRight.SetInverted(true);
talBackLeft.SetSensorPhase(false);
talBackLeft.SetInverted(false);
talBackRight.SetSensorPhase(true);
talBackRight.SetInverted(true);

/* Set the peak and nominal outputs */
talFrontLeft.ConfigNominalOutputForward(0, 10);
talFrontLeft.ConfigNominalOutputReverse(0, 10);
talFrontLeft.ConfigPeakOutputForward(1, 10);
talFrontLeft.ConfigPeakOutputReverse(-1, 10);

talFrontRight.ConfigNominalOutputForward(0, 10);
talFrontRight.ConfigNominalOutputReverse(0, 10);
talFrontRight.ConfigPeakOutputForward(1, 10);
talFrontRight.ConfigPeakOutputReverse(-1, 10);

talBackLeft.ConfigNominalOutputForward(0, 10);
talBackLeft.ConfigNominalOutputReverse(0, 10);
talBackLeft.ConfigPeakOutputForward(1, 10);
talBackLeft.ConfigPeakOutputReverse(-1, 10);

talBackRight.ConfigNominalOutputForward(0, 10);
talBackRight.ConfigNominalOutputReverse(0, 10);
talBackRight.ConfigPeakOutputForward(1, 10);
talBackRight.ConfigPeakOutputReverse(-1, 10);

/* Set Motion Magic gains in slot0 - see documentation */
talFrontLeft.SelectProfileSlot(0, 0);
talFrontLeft.Config_kF(0, 0.04721247923, 10);
talFrontLeft.Config_kP(0, 0.16084905660, 10);
talFrontLeft.Config_kI(0, 0.001, 10);
talFrontLeft.Config_kD(0, 1.68, 10);

talFrontRight.SelectProfileSlot(0, 0);
talFrontRight.Config_kF(0, 0.04721247923, 10);
talFrontRight.Config_kP(0, 0.16084905660, 10);
talFrontRight.Config_kI(0, 0.001, 10);
talFrontRight.Config_kD(0, 1.68, 10);

talBackLeft.SelectProfileSlot(0, 0);
talBackLeft.Config_kF(0, 0.04721247923, 10);
talBackLeft.Config_kP(0, 0.16084905660, 10);
talBackLeft.Config_kI(0, 0.001, 10);
talBackLeft.Config_kD(0, 1.68, 10);

talBackRight.SelectProfileSlot(0, 0);
talBackRight.Config_kF(0, 0.04721247923, 10);
talBackRight.Config_kP(0, 0.16084905660, 10);
talBackRight.Config_kI(0, 0.001, 10);
talBackRight.Config_kD(0, 1.68, 10);

}

/** simple wrapper for code cleanup */
void sleepApp(int ms)
{
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void velocityCallback(const geometry_msgs::Twist& msg){
/* Magic velocity */
/* 2048 units/rev * 1 Rotations in either direction */
double rght = msg.linear.x - msg.angular.z;
double left = msg.linear.x + msg.angular.z;
double left_targetVelocity=left*6000*2048/600;//left*velocidad que otorga el motor*resolucion enc$
double right_targetVelocity=rght*6000*2048/600;//left*velocidad que otorga el motor*resolucion en$

ctre::phoenix::unmanaged::FeedEnable(5000);
if (left == 0){
    talFrontLeft.Set (ControlMode::PercentOutput, left);
    talBackLeft.Set  (ControlMode::PercentOutput, left);
}
else{
    talFrontLeft.Set (ControlMode::Velocity, left_targetVelocity);
    talBackLeft.Set  (ControlMode::Velocity, left_targetVelocity);
}
if (rght == 0){
    talFrontRight.Set (ControlMode::PercentOutput, rght);
    talBackRight.Set  (ControlMode::PercentOutput, rght);
}
else{
    talFrontRight.Set (ControlMode::Velocity, right_targetVelocity);
    talBackRight.Set  (ControlMode::Velocity, right_targetVelocity);
}

/* Get current Talon SRX motor output */
std::stringstream sb;
/* Prepare line to print */
std::cout << "\tOutput%:" << talFrontLeft.GetMotorOutputPercent() << "\n";
std::cout << "\tEncoder Velocity:" << talFrontLeft.GetSelectedSensorVelocity(0) << "\n";
std::cout << "\tTeorical Velocity:" << left_targetVelocity << "\n\n";
}


void joint1Callback(const std_msgs::Float64& msg){
    ctre::phoenix::unmanaged::FeedEnable(10000);  
    /* Motion Magic */     
    /*4096 ticks/rev in either direction */
    /* input should be a value from -110 to 110 deg */
    if (msg.data >= -90 && msg.data <= 180){
        //int offset1_deg = 50.2734375 + 180; //Cero at  center
        //double targetPos = (msg.data + offset1_deg) * 4096 / 360;
	double targetPos = my_map(msg.data, -90, 180, 191, 3263);
        srxArm1.Set(ControlMode::MotionMagic, targetPos);
        //srxArm1.Set(ControlMode::PercentOutput, targetPos);

        /* Prepare line to print */
        std::stringstream sb;
        int dif_error = srxArm1.GetSelectedSensorPosition() - targetPos;
        std::cout << "\tAxis 2 Output\n";
        //double sensorPosition = srxArm1.GetSelectedSensorPosition() / 4096 / 24 * 360;
        //double targetPos = msg.data * 4096 * 24 / 360;
        std::cout << "\tTarget Position Deg: " << msg.data << " degrees\n";
        std::cout << "\tEncoder Position Ticks:" << srxArm1.GetSelectedSensorPosition() << "ticks\n";
        std::cout << "\tEncoder Error:" << dif_error << "\n";
        std::cout << "\tEncoder Velocity:" << srxArm1.GetSelectedSensorVelocity(0) << "\n\n";
    }
    else{
        std::stringstream sb;
        std::cout << "\tMust be a value from -110 to 110 degrees\n";
    }
}

void joint2Callback(const std_msgs::Float64& msg){
    ctre::phoenix::unmanaged::FeedEnable(30000);
    if (msg.data >= 20 && msg.data <= 161){
    //    int offset2_deg = 33.3984375; //grados, cero horizontal
      //  double targetPos = (msg.data + offset2_deg) * 4096 / 360;
      // double targetPos = my_map(msg.data, 10, 161, 510.77, 2228.8);
	double targetPos = my_map(msg.data,20,161, 1585.55, 3189.62);  
 	 srxArm2.Set(ControlMode::MotionMagic, targetPos);
        //srxArm1.Set(ControlMode::PercentOutput, targetPos);

        /* Prepare line to print */
        std::stringstream sb;
        int dif_error = srxArm2.GetSelectedSensorPosition() - targetPos;
        std::cout << "\tAxis 2 Output\n";
        //double sensorPosition = srxArm1.GetSelectedSensorPosition() / 4096 / 24 * 360;
        //double targetPos = msg.data * 4096 * 24 / 360;
        std::cout << "\tTarget Position Deg: " << msg.data << " degrees\n";
        std::cout << "\tEncoder Position Ticks:" << srxArm2.GetSelectedSensorPosition() << "ticks\n";
        std::cout << "\tEncoder Error:" << dif_error << "\n";
        std::cout << "\tEncoder Velocity:" << srxArm2.GetSelectedSensorVelocity(0) << "\n\n";
    }
    else{
        std::stringstream sb;
        std::cout << "\tMust be a value from 0 to 157 degrees\n";
    }

}
void joint3Callback(const std_msgs::Float64& msg){
ctre::phoenix::unmanaged::FeedEnable(10000);
    if (msg.data >= -165.5 && msg.data <= 0){
        double x = msg.data;
        //int in_min = 0;
        //int in_max = -180;
        //int out_min = 3563;
        //int out_max = 1515;
        //double targetPos = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	double targetPos = my_map(msg.data,-165.5 , 0, 658 , 2541);
        srxArm3.Set(ControlMode::MotionMagic, targetPos);
        //srxArm1.Set(ControlMode::PercentOutput, targetPos);

        /* Prepare line to print */
        std::stringstream sb;
        int dif_error = srxArm3.GetSelectedSensorPosition() - targetPos;
        std::cout << "\tAxis 3 Output\n";
        //double sensorPosition = srxArm1.GetSelectedSensorPosition() / 4096 / 24 * 360;
        //double targetPos = msg.data * 4096 * 24 / 360;
        std::cout << "\tTarget Position Deg: " << msg.data << " degrees\n";
        std::cout << "\tEncoder Position Ticks:" << srxArm3.GetSelectedSensorPosition() << "ticks\n";
        std::cout << "\tEncoder Error:" << dif_error << "\n";
        std::cout << "\tEncoder Velocity:" << srxArm3.GetSelectedSensorVelocity(0) << "\n\n";
    }
    else{
        std::stringstream sb;
        std::cout << "\tMust be a value from 0 to -165 degrees\n";
    }
}
void joint4Callback(const std_msgs::Float64& msg){
    ctre::phoenix::unmanaged::FeedEnable(10000); 
     if (msg.data >= -135 && msg.data <= 90){
        double x = msg.data;
        //int in_min = -135;
        //int in_max = 90;
        //int out_min = 457;
        //int out_max = 3017;
        //double targetPos = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
        double targetPos =my_map(msg.data,-135,90,1191,3751);
        srxArm4.Set(ControlMode::MotionMagic, targetPos);
        //srxArm4.Set(ControlMode::PercentOutput, targetPos);

        /* Prepare line to print */
        std::stringstream sb;
        int dif_error = srxArm4.GetSelectedSensorPosition() - targetPos;
        std::cout << "\tAxis 4 Output\n";
        //double sensorPosition = srxArm1.GetSelectedSensorPosition() / 4096 / 24 * 360;
        //double targetPos = msg.data * 4096 * 24 / 360;
        std::cout << "\tTarget Position Deg: " << msg.data << " degrees\n";
        std::cout << "\tEncoder Position Ticks:" << srxArm4.GetSelectedSensorPosition() << "ticks\n";
        std::cout << "\tEncoder Error:" << dif_error << "\n";
        std::cout << "\tEncoder Velocity:" << srxArm4.GetSelectedSensorVelocity(0) << "\n\n";
    }
    else{
        std::stringstream sb;
        std::cout << "\tMust be a value from -135 to 90 degrees\n";
    }
}

void centrifugeCallback(const std_msgs::Float64& msg){
    ctre::phoenix::unmanaged::FeedEnable(10000); 
    srxArm5.Set(ControlMode::PercentOutput, msg.data);
}


void joint6Callback(const std_msgs::Float64& msg){
    ctre::phoenix::unmanaged::FeedEnable(10000); 
    srxArm6.Set(ControlMode::PercentOutput, msg.data);
}

void jointLabCallback(const std_msgs::Float64& msg){
    ctre::phoenix::unmanaged::FeedEnable(10000);  
    if (msg.data >= -30 && msg.data <= 180){
        double x = msg.data;
        int in_min = -30;
        int in_max = 180;
        int out_min = 1026;
        int out_max = 3413;
        double targetPos = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
        srxArmLab.Set(ControlMode::MotionMagic, targetPos);
        //srxArmLab.Set(ControlMode::PercentOutput, msg.data);
        /* Prepare line to print */
        std::stringstream sb;
        int dif_error = srxArmLab.GetSelectedSensorPosition() - targetPos;
        std::cout << "\tAxis 4 Output\n";
        //double sensorPosition = srxArm1.GetSelectedSensorPosition() / 4096 / 24 * 360;
        //double targetPos = msg.data * 4096 * 24 / 360;
        std::cout << "\tTarget Position Deg: " << msg.data << " degrees\n";
        std::cout << "\tEncoder Position Ticks:" << srxArmLab.GetSelectedSensorPosition() << "ticks\n";
        std::cout << "\tEncoder Error:" << dif_error << "\n";
        std::cout << "\tEncoder Velocity:" << srxArmLab.GetSelectedSensorVelocity(0) << "\n\n";
    }
    else{
        std::stringstream sb;
        std::cout << "\tMust be a value from 0 to 90 degrees\n";
    }
}

int main(int argc, char **argv) {       
    /* Configuracion inicial*/
    if (start_config==true){
        std::string interface;
        interface = "can0";
        ctre::phoenix::platform::can::SetCANInterface(interface.c_str());
        initDrive();
        start_config=false;
}
    ros::init(argc, argv, "drive_can");
    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe("cmd_vel",1000, &velocityCallback);
    ros::Subscriber sub1 = nh.subscribe("arm_teleop/joint1",  1000, &joint1Callback);
    ros::Subscriber sub2 = nh.subscribe("arm_teleop/joint2",  1000, &joint2Callback);
    ros::Subscriber sub3 = nh.subscribe("arm_teleop/joint3",  1000, &joint3Callback);
    ros::Subscriber sub4 = nh.subscribe("arm_teleop/joint4",  1000, &joint4Callback);
    ros::Subscriber sub5 = nh.subscribe("centrifuge", 1000, &centrifugeCallback);
    ros::Subscriber sub6 = nh.subscribe("arm_teleop/prism",   1000, &joint6Callback);
    ros::Subscriber subLab = nh.subscribe("arm_teleop/joint2_lab",   1000, &jointLabCallback);
    
    ros::spin();
    return 0;
}


