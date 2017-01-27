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
#include <sensor_msgs/LaserScan.h>
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

typedef sensor_msgs::Image      image_msg;
typedef sensor_msgs::LaserScan  laserscan_msg;
cv::Mat depthImg;
laserscan_msg laserscan;
//Callback functions
void depthCallback(const image_msg::ConstPtr& img, cv::Mat *depthImg)
{
    cv_bridge::CvImagePtr cv_ptr;
    cv::Mat im16;
    try{
        cv_ptr = cv_bridge::toCvCopy(*img, img->encoding);
        im16 = cv_ptr->image;

    }catch(cv_bridge::Exception& e){
        ROS_ERROR("cv_bridge exception: %s", e.what());
    }

    double min,max;
    cv::minMaxIdx(im16, &min, &max);
    im16.convertTo(*depthImg, CV_8UC1, 255/(max-min), -min);
    //QImage image(im8.data, im8.cols, im8.rows, static_cast<int>(im8.step),QImage::Format_Indexed8);
    //ui->display_camera->setPixmap(QPixmap::fromImage(image));
    ROS_INFO("CB: IMG ROWSIZE: [%d]    CB:IMG COLSIZE: [%d]", depthImg->rows, depthImg->cols);
    //cv::imshow("view", *depthImg);
    //cv::waitKey(30);

}

void laserscanCallback(const laserscan_msg::ConstPtr& msg, laserscan_msg* laserscan){
    *laserscan = *msg;
}


int main(int argc, char **argv) {
    ros::init(argc, argv, "segfault_vision");
    ros::NodeHandle nviz;
    ros::Rate loop_rate(30);

    cv::Mat testImg;
    cv::namedWindow("view");
    cv::startWindowThread();
    ros::Subscriber depthSub = nviz.subscribe<image_msg>("kinect2/sd/image_depth", 1, boost::bind(depthCallback, _1, &depthImg));
    ros::Subscriber laserSub = nviz.subscribe<laserscan_msg>("scan", 1, boost::bind(laserscanCallback, _1, &laserscan));



    while(ros::ok())
    {
        ROS_INFO("MAIN: IMG ROWSIZE: [%d]    IMG COLSIZE: [%d]", depthImg.rows, depthImg.cols);
        //testImg = depthImg;
        if(depthImg.cols != 0){
            cv::imshow("view", depthImg);
        }

        ros::spinOnce();
        loop_rate.sleep();
    }
    //Subscriptions from other ROS Nodes
    ros::spin();
    cv::destroyWindow("view");
}