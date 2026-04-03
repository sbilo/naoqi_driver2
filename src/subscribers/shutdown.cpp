#include "shutdown.hpp"

namespace naoqi
{
namespace subscriber
{

ShutdownSubscriber::ShutdownSubscriber( const std::string& name, const std::string& topic, const qi::SessionPtr& session ):
  BaseSubscriber( name, topic, session ),
  p_system_(session->service("ALSystem").value())
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
  p_system_.async<void>("shutdown");
}

} // subscriber
} // naoqi
