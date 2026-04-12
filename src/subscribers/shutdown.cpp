#include "shutdown.hpp"

namespace naoqi
{
namespace subscriber
{

ShutdownSubscriber::ShutdownSubscriber( const std::string& name, const std::string& topic, const qi::SessionPtr& session ):
  BaseSubscriber( name, topic, session ),
  p_system_(session->service("ALSystem").value()),
  session_(session)
{}

void ShutdownSubscriber::reset( rclcpp::Node* node )
{
  sub_ = node->create_subscription<std_msgs::msg::Empty>(
    topic_,
    10,
    std::bind(&ShutdownSubscriber::callback, this, std::placeholders::_1));

  is_initialized_ = true;
}

void ShutdownSubscriber::callback( const std_msgs::msg::Empty::SharedPtr msg )
{
  // Tell NAO to power off, then close our qi session after a short delay.
  // Closing the session releases NAOqi's active-client locks so the robot
  // can complete its shutdown sequence without waiting for us to disconnect.
  p_system_.async<void>("shutdown");
  auto session = session_;
  qi::async([session]() {
    qi::os::msleep(2000);
    session->close();
  });
}

} // subscriber
} // naoqi
