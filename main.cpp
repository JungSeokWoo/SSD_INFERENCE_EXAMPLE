#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "c_detector.h"

using namespace std;

int main()
{
    // Load img
    cv::Mat ori_img = cv::imread("../SSD_INFERENCE_EXAMPLE/test.jpg");

    // Create Detector Object
    C_DETECTOR c_detector_obj;

    // Get $HOME folder
    string home_env = getenv("HOME");

    // Setting Trained model ,weight and mean file
    string model_file = home_env + "/caffe/models/VGGNet/VOC_example/SSD_300x300/deploy.prototxt";
    string weights_file = home_env + "/caffe/models/VGGNet/VOC_example/SSD_300x300/VGG_VOC0712_SSD_300x300_iter_120000.caffemodel";
    string mean_value = "104,117,123";

    // Deep Learning Network Initialize
    c_detector_obj.Net_initialize(model_file,weights_file,mean_value);

    // Bounding Box Image
    cv::Mat bb_img;
    ori_img.copyTo(bb_img);

    // Set Confidence threshold
    float cf_thres = 0.29;

    // Get Detection result
    std::vector<vector<float> > detections_result = c_detector_obj.Detect(ori_img);

    /* Print the detection results. */
    for (unsigned int i = 0; i < detections_result.size(); ++i) {
        const vector<float>& d = detections_result[i];
        // Detection format: [image_id, label, score, xmin, ymin, xmax, ymax].
        CHECK_EQ(d.size(), 7);
        const float score = d[2];
        if (score >= cf_thres) {
            cv::Point2d tl,br;
            tl.x = d[3] * bb_img.cols;
            tl.y = d[4] * bb_img.rows;
            br.x = d[5] * bb_img.cols;
            br.y = d[6] * bb_img.rows;

            vector<cv::Point> points;
            points.push_back(tl);
            points.push_back(br);
            cv::Rect boundRect = cv::boundingRect(points);
            string labelstring;
            cv::Scalar box_color;
            switch(int(d[1]))
            {
            case 7:
            {
                labelstring = "Car";
                box_color = cv::Scalar(120,120,216);
                cv::rectangle(bb_img,boundRect,box_color,4);
                cv::putText(bb_img,labelstring,tl,cv::FONT_HERSHEY_SIMPLEX,0.7,CV_RGB(255,255,255),2);
                break;
            }
            case 15:
            {
                labelstring = "Person";
                box_color = cv::Scalar(120,120,216);
                cv::rectangle(bb_img,boundRect,box_color,4);
                cv::putText(bb_img,labelstring,tl,cv::FONT_HERSHEY_SIMPLEX,0.7,CV_RGB(255,255,255),2);
                break;
            }
            default:
                break;
            }
        }
    }

    // Display Detection result
    cv::imshow("test",bb_img);
    cv::waitKey();
    return 0;
}


