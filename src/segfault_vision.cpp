#include "std_msgs/String.h"
#include <math.h>
#include "sstream"
#include <iostream>
#include <utility>
#include <ros/ros.h>
#include <pses_basis/SensorData.h>
#include <pses_basis/Command.h>
#include <pses_basis/CarInfo.h>
#include <sensor_msgs/Range.h>
#include <sensor_msgs/Imu.h>
#include <nav_msgs/Odometry.h>
#include <functional>
//Image related
#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>

#include <string>
#include <sstream>

typedef sensor_msgs::Image image_msg;

//Callback functions
void depthCallback(const image_msg::ConstPtr& img, cv::Mat depthImg)
{
    cv_bridge::CvImagePtr cv_ptr;
    try{
        cv_ptr = cv_bridge::toCvCopy(*img, img->encoding);
    }catch(cv_bridge::Exception& e){
        ROS_ERROR("cv_bridge exception: %s", e.what());
    }
    depthImg = cv_ptr->image;
    /*
    double min,max;
    cv::minMaxIdx(im16, &min, &max);
    cv::Mat im8;
    im16.convertTo(im8, CV_8UC1, 255/(max-min), -min);
    QImage image(im8.data, im8.cols, im8.rows, static_cast<int>(im8.step),QImage::Format_Indexed8);
    ui->display_camera->setPixmap(QPixmap::fromImage(image));
    */
}


int main(int argc, char **argv) {
    ros::init(argc, argv, "segfault_vision");
    ros::NodeHandle *nviz;
    ros::Rate loop_rate(40);
    cv::Mat depthImg;

    cv::namedWindow("view");
    cv::startWindowThread();
    ros::Subscriber depthSub = nviz->subscribe<image_msg>("kinect2/sd/image_depth", 10, boost::bind(depthCallback, _1, depthImg));
    cv::imshow("view", depthImg);

    ros::spin();
    cv::destroyWindow("view");
    /*
    while(ros::ok())
    {
        ros::spinOnce();
        loop_rate.sleep();
    }
     */
/*Subscriptions from other ROS Nodes*/
   // ros::spin();

}