#include "autonomous_life.hpp"

namespace naoqi
{
namespace subscriber
{

AutonomousLifeSubscriber::AutonomousLifeSubscriber( const std::string& name, const std::string& topic, const qi::SessionPtr& session ):
  BaseSubscriber( name, topic, session ),
  p_life_(session->service("ALAutonomousLife").value())
{}

void AutonomousLifeSubscriber::reset( rclcpp::Node* node )
{
  sub_ = node->create_subscription<std_msgs::msg::Bool>(
    topic_,
    10,
    std::bind(&AutonomousLifeSubscriber::callback, this, std::placeholders::_1));

  is_initialized_ = true;
}

void AutonomousLifeSubscriber::callback( const std_msgs::msg::Bool::SharedPtr msg )
{
  const std::string state = msg->data ? "solitary" : "disabled";
  p_life_.async<void>("setState", state);
}

} // subscriber
} // naoqi
