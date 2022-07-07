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

using namespace ctre::phoenix;
using namespace ctre::phoenix::platform;
using namespace ctre::phoenix::motorcontrol;
using namespace ctre::phoenix::motorcontrol::can;

bool start_config = true;

/* make some talons for drive train */
TalonFX talFrontLeft(0);
TalonFX talFrontRight(2);
TalonFX talBackLeft(1);
TalonFX talBackRight(3);

void initDrive()
{
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
   double left_targetVelocity=left*6000*2048/600;//left*velocidad que otorga el motor*resolucion encoder/600
   double right_targetVelocity=rght*6000*2048/600;//left*velocidad que otorga el motor*resolucion encoder/600

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

int main(int argc, char **argv) {      	
	/* Configuracion inicial*/
	if (start_config==true){
		std::string interface;
    	interface = "can0";
	   ctre::phoenix::platform::can::SetCANInterface(interface.c_str());
  	   //ctre::phoenix::unmanaged::SetTransmitEnable(true);	
		/*ctre::phoenix::unmanaged::FeedEnable(100);*/
   	initDrive();
   	start_config=false;
   }
	ros::init(argc, argv, "drive_can");
	ros::NodeHandle nh;
	ros::Subscriber sub = nh.subscribe("cmd_vel",1000, &velocityCallback);
	/*sleepApp(20);*/
	ros::spin();

	return 0;
}

