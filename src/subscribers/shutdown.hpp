#ifndef SHUTDOWN_SUBSCRIBER_HPP
#define SHUTDOWN_SUBSCRIBER_HPP

#include "subscriber_base.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/empty.hpp"

namespace naoqi
{
namespace subscriber
{

class ShutdownSubscriber: public BaseSubscriber<ShutdownSubscriber>
{
public:
  ShutdownSubscriber( const std::string& name, const std::string& topic, const qi::SessionPtr& session );
  ~ShutdownSubscriber(){}

  void reset( rclcpp::Node* node );
  void callback( const std_msgs::msg::Empty::SharedPtr msg );

private:
  qi::AnyObject p_system_;
  qi::SessionPtr session_;
  rclcpp::Subscription<std_msgs::msg::Empty>::SharedPtr sub_;
};

} // subscriber
} // naoqi
#endif
