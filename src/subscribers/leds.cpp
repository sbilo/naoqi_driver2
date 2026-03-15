#include "leds.hpp"

namespace naoqi
{
namespace subscriber
{

LedsSubscriber::LedsSubscriber( const std::string& name, const std::string& topic, const qi::SessionPtr& session ):
  BaseSubscriber( name, topic, session ),
  p_leds_(session->service("ALLeds").value())
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
  // Pack r/g/b (0.0-1.0) into a single 0x00RRGGBB int as ALLeds.fadeRGB expects
  const int color =
    (static_cast<int>(msg->r * 255) << 16) |
    (static_cast<int>(msg->g * 255) <<  8) |
    (static_cast<int>(msg->b * 255));
  p_leds_.async<void>("fadeRGB", "FaceLeds", color, 0.1f);
}

} // subscriber
} // naoqi
