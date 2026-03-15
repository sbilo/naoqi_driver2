#include "posture.hpp"

namespace naoqi
{
namespace subscriber
{

PostureSubscriber::PostureSubscriber( const std::string& name, const std::string& topic, const qi::SessionPtr& session ):
  BaseSubscriber( name, topic, session ),
  p_posture_(session->service("ALRobotPosture").value())
{}

void PostureSubscriber::reset( rclcpp::Node* node )
{
  sub_ = node->create_subscription<std_msgs::msg::String>(
    topic_,
    10,
    std::bind(&PostureSubscriber::callback, this, std::placeholders::_1));

  is_initialized_ = true;
}

void PostureSubscriber::callback( const std_msgs::msg::String::SharedPtr msg )
{
  p_posture_.async<bool>("goToPosture", msg->data, 0.5f);
}

} // subscriber
} // naoqi
