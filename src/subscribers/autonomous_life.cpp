#include "autonomous_life.hpp"
#include <iostream>

namespace naoqi
{
namespace subscriber
{

AutonomousLifeSubscriber::AutonomousLifeSubscriber( const std::string& name, const std::string& topic, const qi::SessionPtr& session ):
  BaseSubscriber( name, topic, session ),
  p_life_(session->service("ALAutonomousLife").value())
{
  // ALBackgroundMovement is not available on all NAO versions — make it optional
  try {
    p_background_movement_ = session->service("ALBackgroundMovement").value();
    has_background_movement_ = true;
  } catch (...) {
    has_background_movement_ = false;
    std::cout << "ALBackgroundMovement not available, skipping" << std::endl;
  }

  // Set solitary mode at startup so life behaviors don't override LED/motion control
  p_life_.async<void>("setState", "solitary");
  if (has_background_movement_) {
    p_background_movement_.async<void>("setEnabled", false);
  }
}

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
  // Always stay in "solitary" — "safeguard" and "disabled" both break the mic pipeline.
  p_life_.async<void>("setState", "solitary");
  if (has_background_movement_) {
    p_background_movement_.async<void>("setEnabled", false);
  }
}

} // subscriber
} // naoqi
