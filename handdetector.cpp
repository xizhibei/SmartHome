#include "handdetector.h"

HandDetector::HandDetector(const char* feature_file_name, CvSize img_size,int min_th)
{
    this->MIN_TH = min_th;
    this->img_size = img_size;

    //IplImage* pFrame = cvQueryFrame(pcapture);
    skin = cvCreateImage(img_size, IPL_DEPTH_8U, 1);
    hand = cvCreateImage(img_size, IPL_DEPTH_8U, 1);
    YCrCb = cvCreateImage(img_size, IPL_DEPTH_8U, 3);
    upper = cvScalar(255, 166, 129);
    lower = cvScalar(69, 133, 79);
    storage = cvCreateMemStorage(0);
#if DEBUG
    feature = cvCreateImage(cvSize(100 * STEP,100 * STEP), IPL_DEPTH_8U, 1);
    cvNamedWindow("hand", 0);
    cvNamedWindow("feature", 0);
#endif
}

HandDetector::~HandDetector()
{
    cvReleaseImage(&skin);
    cvReleaseImage(&hand);
    cvReleaseImage(&YCrCb);
    cvReleaseMemStorage(&storage);
#if DEBUG
    cvReleaseImage(&feature);
    cvDestroyAllWindows();
#endif
}
