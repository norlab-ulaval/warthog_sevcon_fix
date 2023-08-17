/*
 *  Warthog Sevcon Node - fixes current values that
 *  are given by the ROS topics on Clearpath Warthog
 *  Copyright (C) 2023  Damien LaRocque

 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.

 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"
#include "warthog_sevcon_fix/msg/float64_stamped.hpp"

using std::placeholders::_1;

class WarthogSevconFixNode : public rclcpp::Node {
public:
  WarthogSevconFixNode() : Node("warthog_sevcon_fix_node") {
    left_current_sub_ = this->create_subscription<std_msgs::msg::Float64>(
        "left_drive/status/battery_current", 10,
        std::bind(&WarthogSevconFixNode::left_current_callback, this, _1));
    right_current_sub_ = this->create_subscription<std_msgs::msg::Float64>(
        "right_drive/status/battery_current", 10,
        std::bind(&WarthogSevconFixNode::right_current_callback, this, _1));
    left_current_pub_ =
        this->create_publisher<warthog_sevcon_fix::msg::Float64Stamped>(
            "sevcon/battery_current/left", 10);
    right_current_pub_ =
        this->create_publisher<warthog_sevcon_fix::msg::Float64Stamped>(
            "sevcon/battery_current/right", 10);
  }

private:
  void left_current_callback(const std_msgs::msg::Float64 &msg) const {
    auto message = warthog_sevcon_fix::msg::Float64Stamped();
    int16_t current = (int16_t)(16 * msg.data);
    message.data.data = (double)current / 16;
    message.header.stamp = this->now();
    left_current_pub_->publish(message);
  }
  void right_current_callback(const std_msgs::msg::Float64 &msg) const {
    auto message = warthog_sevcon_fix::msg::Float64Stamped();
    int16_t current = (int16_t)(16 * msg.data);
    message.data.data = (double)current / 16;
    message.header.stamp = this->now();
    right_current_pub_->publish(message);
  }
  rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr left_current_sub_;
  rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr right_current_sub_;
  rclcpp::Publisher<warthog_sevcon_fix::msg::Float64Stamped>::SharedPtr
      left_current_pub_;
  rclcpp::Publisher<warthog_sevcon_fix::msg::Float64Stamped>::SharedPtr
      right_current_pub_;
};

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<WarthogSevconFixNode>());
  rclcpp::shutdown();
  return 0;
}
