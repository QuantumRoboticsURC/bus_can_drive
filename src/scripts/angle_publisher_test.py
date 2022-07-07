#!/usr/bin/env python
# license removed for brevity
import rospy
from std_msgs.msg import Float64

def angle_publisher():
    pub_2 = rospy.Publisher('arm_teleop/joint2', Float64, queue_size=10)
    pub_3 = rospy.Publisher('arm_teleop/joint3', Float64, queue_size=10)

    rospy.init_node('angle_publisher', anonymous=True)
    rate = rospy.Rate(0.15) # 10hz

    angle_2 = Float64()
    angle_3 = Float64()
    rate.sleep()

    angle_3.data = -90
    pub_3.publish(angle_3)
    print("moviendo angulo 3")
    rate.sleep()

    angle_2.data = 90
    pub_2.publish(angle_2)
    rate.sleep()
    rate.sleep()


    angle_2.data = 161
    pub_2.publish(angle_2)
    rate.sleep()

    angle_3.data = -165.4
    pub_3.publish(angle_3)
    rate.sleep()




if __name__ == '__main__':
    try:
        angle_publisher()
    except rospy.ROSInterruptException:
        pass
