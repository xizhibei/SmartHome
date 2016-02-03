#ifndef SVMHANDDETECTOR_H
#define SVMHANDDETECTOR_H

#include "handdetector.h"

#include <ml.h>

class SVMHandDetector:public HandDetector
{
public:
    SVMHandDetector(const char* feature_file_name, CvSize img_size,int min_th = 60);
    virtual ~SVMHandDetector();
    int load_feature(const char* name);
    int find_nearest_hand(FeatureData* feature);
    CvRect get_feature(IplImage* img,FeatureData* feature);
    int detect_hand(IplImage* img,CvRect* area);

    void train_and_save();
private:
    CvSVM svm;
    CvMat* avg;
    CvMat* vecs;
    //for tmp
    CvMat* feature_mat;
    CvMat* before_project;
};



#endif // SVMHANDDETECTOR_H
