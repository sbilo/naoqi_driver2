#ifndef AUTONOMOUS_LIFE_SUBSCRIBER_HPP
#define AUTONOMOUS_LIFE_SUBSCRIBER_HPP

#include "subscriber_base.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/bool.hpp"

namespace naoqi
{
namespace subscriber
{

class AutonomousLifeSubscriber: public BaseSubscriber<AutonomousLifeSubscriber>
{
public:
  AutonomousLifeSubscriber( const std::string& name, const std::string& topic, const qi::SessionPtr& session );
  ~AutonomousLifeSubscriber(){}

  void reset( rclcpp::Node* node );
  void callback( const std_msgs::msg::Bool::SharedPtr msg );

private:
  qi::AnyObject p_life_;
  rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr sub_;
};

} // subscriber
} // naoqi
#endif
