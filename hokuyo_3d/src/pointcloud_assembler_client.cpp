#include <ros/ros.h>
#include <laser_assembler/AssembleScans2.h>
#include <sensor_msgs/PointCloud2.h>

using namespace laser_assembler;

int main(int argc, char **argv)
{
  // initialize ROS and create a node handle
  ros::init(argc, argv, "pcl_assembler_client");
  ros::NodeHandle node;

  // create a publisher
  ros::Publisher pub_cloud;

  // advertise the assembled_cloud message
  pub_cloud = node.advertise<sensor_msgs::PointCloud2>("assembled_cloud",1);

  //Call the assemble_scans service
  ros::service::waitForService("assemble_scans2");
  node.setParam("fixed_frame", "base_link");
  node.setParam("max_clouds", 400);
  ros::ServiceClient client = node.serviceClient<AssembleScans2>("assemble_scans2");
  AssembleScans2 srv;
  srv.request.begin = ros::Time(0,0);
  srv.request.end = ros::Time::now();

  if (client.call(srv))
  {
    //publish the resultant cloud
    pub_cloud.publish(srv.response.cloud);
  }
}
