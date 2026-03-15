#ifndef LEDS_SUBSCRIBER_HPP
#define LEDS_SUBSCRIBER_HPP

#include "subscriber_base.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/color_rgba.hpp"

namespace naoqi
{
namespace subscriber
{

class LedsSubscriber: public BaseSubscriber<LedsSubscriber>
{
public:
  LedsSubscriber( const std::string& name, const std::string& topic, const std::string& led_group, const qi::SessionPtr& session );
  ~LedsSubscriber(){}

  void reset( rclcpp::Node* node );
  void callback( const std_msgs::msg::ColorRGBA::SharedPtr msg );

private:
  qi::AnyObject p_leds_;
  std::string led_group_;
  rclcpp::Subscription<std_msgs::msg::ColorRGBA>::SharedPtr sub_;
};

} // subscriber
} // naoqi
#endif
