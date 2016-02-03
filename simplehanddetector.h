#ifndef SIMPLEHANDDETECTOR_H
#define SIMPLEHANDDETECTOR_H

#include "handdetector.h"

class SimpleHandDetector : public HandDetector
{
public:
    SimpleHandDetector(const char* feature_file_name, CvSize img_size,int min_th = 60);
    int load_feature(const char* name);
    int find_nearest_hand(FeatureData* feature);
    CvRect get_feature(IplImage* img,FeatureData* feature);
    int detect_hand(IplImage* img,CvRect* area);
};

#endif // SIMPLEHANDDETECTOR_H
