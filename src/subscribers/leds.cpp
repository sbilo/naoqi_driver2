#include "leds.hpp"

namespace naoqi
{
namespace subscriber
{

LedsSubscriber::LedsSubscriber( const std::string& name, const std::string& topic, const std::string& led_group, const qi::SessionPtr& session ):
  BaseSubscriber( name, topic, session ),
  p_leds_(session->service("ALLeds").value()),
  led_group_(led_group)
{}

void LedsSubscriber::reset( rclcpp::Node* node )
{
  sub_ = node->create_subscription<std_msgs::msg::ColorRGBA>(
    topic_,
    10,
    std::bind(&LedsSubscriber::callback, this, std::placeholders::_1));

  is_initialized_ = true;
}

void LedsSubscriber::callback( const std_msgs::msg::ColorRGBA::SharedPtr msg )
{
  const int color =
    (static_cast<int>(msg->r * 255) << 16) |
    (static_cast<int>(msg->g * 255) <<  8) |
    (static_cast<int>(msg->b * 255));
  p_leds_.async<void>("fadeRGB", led_group_, color, 0.1f);
}

} // subscriber
} // naoqi
