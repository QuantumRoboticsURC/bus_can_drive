#define Phoenix_No_WPI // remove WPI dependencies
#include "ctre/Phoenix.h"
#include "ctre/phoenix/platform/Platform.h"
#include "ctre/phoenix/unmanaged/Unmanaged.h"
#include "ctre/phoenix/cci/Unmanaged_CCI.h"
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <SDL2/SDL.h>
#include <unistd.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include <geometry_msgs/Twist.h>

using namespace ctre::phoenix;
using namespace ctre::phoenix::platform;
using namespace ctre::phoenix::motorcontrol;
using namespace ctre::phoenix::motorcontrol::can;


/* make some talons for drive train */
VictorSPX talLeft(1);
VictorSPX talRght(0);

void initDrive()
{
	/* both talons should blink green when driving forward */
	//printf("Init Drive started");
	talLeft.SetInverted(true);
}

void drive(double fwd, double turn)
{
	double left = fwd - turn;
	double rght = fwd + turn; /* positive turn means turn robot LEFT */
	//printf("Left wheel: %lf \n", left);
	//printf("Right wheel: %lf \n", rght);
	talLeft.Set(ControlMode::PercentOutput, left);
	talRght.Set(ControlMode::PercentOutput, rght);
}
/** simple wrapper for code cleanup */
void sleepApp(int ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

float linx, angZ;

void filterVelocityCallback(const geometry_msgs::Twist& msg){
   //Using the callback function just for subscribing  
   //Subscribing the message and storing it in 'linx' and 'angZ'
   linx = msg.linear.x;      angZ = msg.angular.z;
}

int main(int argc, char **argv) {
      	
	/* don't bother prompting, just use can0 */

	std::string interface;
	interface = "can0";
	ctre::phoenix::platform::can::SetCANInterface(interface.c_str());

	// Comment out the call if you would rather use the automatically running diag-server, note this requires uninstalling diagnostics from Tuner. 
	//c_SetPhoenixDiagnosticsStartTime(-1); // disable diag server, instead we will use the diag server stand alone application that Tuner installs

	/* setup drive */
	initDrive();
	drive(0, 0);
	while (true) {
		ros::init(argc, argv, "drive_can");
		ros::NodeHandle nh;
		ros::Subscriber sub = nh.subscribe("cmd_vel",1000,&filterVelocityCallback);


		while (ros::ok()){
			/* grab some stick values */
			double y = linx;
			double turn = angZ;
			//printf("The tip that you should leave is %lf \n", y);
			drive(y, turn);	
			ctre::phoenix::unmanaged::FeedEnable(100);	
			/* loop yield for a bit */
			sleepApp(20);

     			ros::spinOnce();      //Notice this
		}
		
		
	}

	SDL_Quit();
	return 0;
}
