#include "posture.hpp"

namespace naoqi
{
namespace subscriber
{

PostureSubscriber::PostureSubscriber( const std::string& name, const std::string& topic, const qi::SessionPtr& session ):
  BaseSubscriber( name, topic, session ),
  p_posture_(session->service("ALRobotPosture").value()),
  p_motion_(session->service("ALMotion").value())
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
  if (msg->data == "Crouch" || msg->data == "rest") {
    // ALMotion.rest() is more reliable than goToPosture in solitary life mode
    p_motion_.async<void>("rest");
  } else {
    p_posture_.async<bool>("goToPosture", msg->data, 0.5f);
  }
}

} // subscriber
} // naoqi
