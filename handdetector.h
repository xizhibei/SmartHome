#ifndef HANDDETECTOR_H
#define HANDDETECTOR_H
#include <vector>
using namespace std;

#include <cv.h>
#include <highgui.h>
#include <QDebug>

#define IMG8U(img,x,y) ((uchar*)(img->imageData + img->widthStep * (y)))[(x)]
#define STEP 20
#define FEATURE_LEN STEP * STEP
#define PCA_LEN 100
#define DEBUG 0

union FeatureData
{
      char ch[FEATURE_LEN];
      double db[PCA_LEN];
};

struct Feature
{
    int val;
    FeatureData data;
};

class HandDetector
{
public:
    HandDetector(const char* feature_file_name, CvSize img_size,int min_th);
    virtual ~HandDetector();
    virtual int load_feature(const char* name) = 0;
    virtual int find_nearest_hand(FeatureData* feature) = 0;
    virtual CvRect get_feature(IplImage* img,FeatureData* feature) = 0;
    virtual int detect_hand(IplImage* img,CvRect* area) = 0;
protected:
    vector<Feature> features;
    int MIN_TH;
    CvSize img_size;

    IplImage* pFrame;
    IplImage* skin;
    IplImage* hand;
    IplImage* YCrCb;
#if DEBUG
    IplImage* feature_img;
#endif
    CvScalar upper;
    CvScalar lower;
    CvMemStorage* storage;
};

#endif // HANDDETECTOR_H
