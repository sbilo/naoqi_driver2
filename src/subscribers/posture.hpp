#ifndef POSTURE_SUBSCRIBER_HPP
#define POSTURE_SUBSCRIBER_HPP

#include "subscriber_base.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

namespace naoqi
{
namespace subscriber
{

class PostureSubscriber: public BaseSubscriber<PostureSubscriber>
{
public:
  PostureSubscriber( const std::string& name, const std::string& topic, const qi::SessionPtr& session );
  ~PostureSubscriber(){}

  void reset( rclcpp::Node* node );
  void callback( const std_msgs::msg::String::SharedPtr msg );

private:
  qi::AnyObject p_posture_;
  qi::AnyObject p_motion_;
  qi::Future<bool> posture_future_;  // kept alive so async call reaches NAO
  qi::Future<void> rest_future_;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_;
};

} // subscriber
} // naoqi
#endif
