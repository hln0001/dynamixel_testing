#include <stdlib.h>
#include <stdio.h>

#include "dynamixel_sdk.h"

#include <ros/ros.h>
#include <std_msgs/Empty.h>
#include <std_msgs/Float64.h>
#include <std_msgs/UInt16.h>

//Spins rotates the dynamixel back and forth 180 deg and publishes the raw position value

// Control table addresses
#define ADDR_MX_TORQUE_ENABLE           64                  // Control table address is different in Dynamixel model
#define ADDR_MX_DRIVE_MODE              11
#define ADDR_MX_GOAL_VELOCITY           104
#define ADDR_MX_GOAL_POSITION           116
#define ADDR_MX_PRESENT_POSITION        132

// Protocol version
#define PROTOCOL_VERSION                2.0                 // See which protocol version is used in the Dynamixel

// Default settings
#define DXL_ID                          1                   // Dynamixel ID: 1
#define BAUDRATE                        57600
#define DEVICENAME                      "/dev/ttyUSB0"      // Check which port is being used on your controller

// Values to send to change settings
#define TORQUE_ENABLE                   1                   // Value for enabling the torque
#define TORQUE_DISABLE                  0                   // Value for disabling the torque
#define VELOCITY_CTRL_MODE              1                   // Value to set drive mode to wheel mode
#define POSITION_CTRL_MODE              3                   // Value to set drive mode to JOINT mode


class Dynamixel {
private:
  ros::Publisher pub_pos; //position
public:
  Dynamixel();
  void positionPub(uint16_t dxl_present_position);
  ros::NodeHandle node;
};

//Dynamixel class constructor creates publishers
Dynamixel::Dynamixel() {
	//create publisher for motor commands
  pub_pos = node.advertise<std_msgs::UInt16>("/dxl_pos", 10);
};


//Publishes raw position value to ROS
void Dynamixel::positionPub(uint16_t dxl_present_position) {
  std_msgs::UInt16 msg;
	msg.data = dxl_present_position;
	pub_pos.publish(msg);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "spin_test");
  Dynamixel motor;
  // Initialize PortHandler instance
  // Set the port path
  // Get methods and members of PortHandlerLinux or PortHandlerWindows
  dynamixel::PortHandler *portHandler = dynamixel::PortHandler::getPortHandler(DEVICENAME);

  // Initialize PacketHandler instance
  // Set the protocol version
  // Get methods and members of Protocol1PacketHandler or Protocol2PacketHandler
  dynamixel::PacketHandler *packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);

  int index = 0;
  int dxl_comm_result = COMM_TX_FAIL;             // Communication result
  int dxl_goal_velocity;
  motor.node.param("goal_speed", dxl_goal_velocity, 50);


  uint8_t dxl_error = 0;                          // Dynamixel error
  uint16_t dxl_present_position = 0;              // Present position

  // Open port
  if (portHandler->openPort())
  {
    // printf("Succeeded to open the port!\n");
  }

  // Set port baudrate
  if (portHandler->setBaudRate(BAUDRATE))
  {
    // printf("Succeeded to change the baudrate!\n");
  }

  // Disable Dynamixel Torque
  dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL_ID, ADDR_MX_TORQUE_ENABLE, TORQUE_DISABLE, &dxl_error);
  if (dxl_comm_result != COMM_SUCCESS)
  {
    // printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
  }
  else if (dxl_error != 0)
  {
    // printf("%s\n", packetHandler->getRxPacketError(dxl_error));
  }
  else
  {
    // printf("Dynamixel has been successfully connected \n");
  }


  // Change Operating Mode to Joint
  dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL_ID, ADDR_MX_DRIVE_MODE, POSITION_CTRL_MODE, &dxl_error);
  if (dxl_comm_result != COMM_SUCCESS)
  {
    // printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
  }
  else if (dxl_error != 0)
  {
    // printf("%s\n", packetHandler->getRxPacketError(dxl_error));
  }
  else
  {
    // printf("In position control mode! \n");
  }

  // Enable Dynamixel Torque
  dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL_ID, ADDR_MX_TORQUE_ENABLE, TORQUE_ENABLE, &dxl_error);
  if (dxl_comm_result != COMM_SUCCESS)
  {
    // printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
  }
  else if (dxl_error != 0)
  {
    // printf("%s\n", packetHandler->getRxPacketError(dxl_error));
  }
  else
  {
    // printf("Dynamixel has been successfully connected \n");
  }

  //Set position to 0
  // Write goal position
  dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, DXL_ID, ADDR_MX_GOAL_POSITION, 0, &dxl_error);
  if (dxl_comm_result != COMM_SUCCESS)
  {
    // printf("%s\n", packetHandler->getRxPacketError(dxl_error));
  }
  ros::Duration(1).sleep();

  // Disable Dynamixel Torque
  dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL_ID, ADDR_MX_TORQUE_ENABLE, TORQUE_DISABLE, &dxl_error);
  if (dxl_comm_result != COMM_SUCCESS)
  {
    // printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
  }
  else if (dxl_error != 0)
  {
    // printf("%s\n", packetHandler->getRxPacketError(dxl_error));
  }
  else
  {
    // printf("Dynamixel has been successfully connected \n");
  }

  // Change Operating Mode to Wheel
  dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL_ID, ADDR_MX_DRIVE_MODE, VELOCITY_CTRL_MODE, &dxl_error);
  if (dxl_comm_result != COMM_SUCCESS)
  {
    // printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
  }
  else if (dxl_error != 0)
  {
    // printf("%s\n", packetHandler->getRxPacketError(dxl_error));
  }
  else
  {
    // printf("In velocity control mode! \n");
  }

  // Enable Dynamixel Torque
  dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL_ID, ADDR_MX_TORQUE_ENABLE, TORQUE_ENABLE, &dxl_error);
  if (dxl_comm_result != COMM_SUCCESS)
  {
    // printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
  }
  else if (dxl_error != 0)
  {
    // printf("%s\n", packetHandler->getRxPacketError(dxl_error));
  }
  else
  {
    // printf("Dynamixel has been successfully connected \n");
  }

  while(ros::ok())
  {
    // Write goal speed
    dxl_comm_result = packetHandler->write4ByteTxRx(portHandler, DXL_ID, ADDR_MX_GOAL_VELOCITY, dxl_goal_velocity, &dxl_error);
    while(dxl_error == 0){
      // Read present position
      dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, DXL_ID, ADDR_MX_PRESENT_POSITION, &dxl_present_position, &dxl_error);
      if (dxl_comm_result != COMM_SUCCESS)
      {
        // printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
      }
      else if (dxl_error != 0)
      {
        // printf("%s\n", packetHandler->getRxPacketError(dxl_error));
      }

      if(dxl_present_position > 4096 && dxl_present_position <5000 && dxl_goal_velocity > 0)
      {
        dxl_goal_velocity = -dxl_goal_velocity;
        dxl_comm_result = packetHandler->write4ByteTxRx(portHandler, DXL_ID, ADDR_MX_GOAL_VELOCITY, dxl_goal_velocity, &dxl_error);
      }
      else if(dxl_present_position > 50000 && dxl_goal_velocity < 0)
      {
        dxl_goal_velocity = -dxl_goal_velocity;
        dxl_comm_result = packetHandler->write4ByteTxRx(portHandler, DXL_ID, ADDR_MX_GOAL_VELOCITY, dxl_goal_velocity, &dxl_error);
      }

      // printf("[ID:%03d] PresPos:%03d\n", DXL_ID, dxl_present_position);
      motor.positionPub(dxl_present_position);
    }
  }
}