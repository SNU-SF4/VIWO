#include <string>
#include <gtest/gtest.h>
#include "ros/ros.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"

#include <boost/thread.hpp>

using namespace ros;


int g_argc;
char** g_argv;

typedef boost::shared_ptr<geometry_msgs::PoseWithCovarianceStamped const> EkfConstPtr;

class TestEKF : public testing::Test
{
public:
  NodeHandle node_;
  ros::Subscriber ekf_sub_;
  double ekf_counter_;


  void EKFCallback(const EkfConstPtr& ekf)
  {
    // count number of callbacks
    ekf_counter_++;
  }


protected:
  /// constructor
  TestEKF()
  {
    ekf_counter_ = 0;

  }


  /// Destructor
  ~TestEKF()
  {
  }
};




TEST_F(TestEKF, test)
{
  ROS_INFO("Subscribing to robot_pose_ekf/odom_combined");
  ekf_sub_ = node_.subscribe("/robot_pose_ekf/odom_combined", 10, &TestEKF::EKFCallback, (TestEKF*)this);

  // wait for 20 seconds
  ROS_INFO("Waiting for 20 seconds while bag is playing");
  ros::Duration(20).sleep();
  ROS_INFO("End time reached");

  EXPECT_EQ(ekf_counter_, 0);

  SUCCEED();
}




int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  g_argc = argc;
  g_argv = argv;

  init(g_argc, g_argv, "testEKF");

  boost::thread spinner(boost::bind(&ros::spin));

  int res = RUN_ALL_TESTS();
  spinner.interrupt();
  spinner.join();

  return res;
}
