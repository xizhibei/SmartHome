#include "svmhanddetector.h"

SVMHandDetector::SVMHandDetector(const char* feature_file_name, CvSize img_size, int min_th):
    HandDetector(feature_file_name,img_size, min_th)
{
    if(load_feature(feature_file_name) == -1)
        printf("Error!\n");
    feature_mat = cvCreateMat(1, PCA_LEN, CV_32FC1);
    before_project = cvCreateMat(1, FEATURE_LEN, CV_32FC1);
}

SVMHandDetector::~SVMHandDetector()
{
    cvReleaseMat(&before_project);
    cvReleaseMat(&feature_mat);
    cvReleaseMat(&avg);
    cvReleaseMat(&vecs);
}

int SVMHandDetector::load_feature(const char *name)
{
    svm.load(name);
    avg = cvCreateMat(1,FEATURE_LEN,CV_32FC1);
    vecs = cvCreateMat(FEATURE_LEN,FEATURE_LEN,CV_32FC1);

    FILE* fp = fopen("pcadata","r+");
    fread(avg->data.fl,sizeof(float),FEATURE_LEN,fp);
    fread(vecs->data.fl,sizeof(float),FEATURE_LEN * FEATURE_LEN,fp);
    fclose(fp);

    qDebug() << "Load feature success!";

    return 0;
}

int SVMHandDetector::find_nearest_hand(FeatureData* feature)
{
    float* data = before_project->data.fl;
    for(int i = 0;i < FEATURE_LEN;i++,data++)
        *data = feature->ch[i] - '0';
    cvProjectPCA( before_project, avg, vecs, feature_mat );
    float ret = svm.predict(feature_mat);
    qDebug() << "Get predict val: " << ret;
    return ret;
}

CvRect SVMHandDetector::get_feature(IplImage *img, FeatureData *feature)
{
    IplImage* pFrame = img;
    char* f = feature->ch;
#if DEBUG
    cvZero(feature_img);
#endif
    cvCvtColor(pFrame, YCrCb, CV_BGR2YCrCb);
    cvInRangeS(YCrCb, lower, upper, skin);
    //cvErode(skin,skin, 0, 1);   //形态学滤波，除去噪声
    //cvDilate(skin,skin, 0, 3);
    cvSmooth(skin,skin);

    //cvCopy(skin,hand);

    cvClearMemStorage(storage);
    CvSeq * contour = 0;

    cvFindContours(skin, storage, &contour, sizeof (CvContour), CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));
    if(contour == NULL)
        return cvRect(-1,-1,-1,-1);

    float max = fabs(cvContourArea(contour, CV_WHOLE_SEQ));
    for (CvSeq* p = contour; p != NULL; p = p->h_next)
    {
        float now = fabs(cvContourArea(p, CV_WHOLE_SEQ));
        if (now > max)
        {
            max = now;
            contour = p;
        }
    }

    CvRect rect = cvBoundingRect(contour, 0);
    if (rect.width > STEP && rect.height > STEP)
    {
        //cvRectangle(hand, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height), cvScalar(255, 255, 255), 3);

        cvZero(hand);
        cvDrawContours(hand, contour, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), 0, CV_FILLED,8, cvPoint(0, 0));
        int w_s = rect.width / STEP;
        int h_s = rect.height / STEP;
        int w_off = rect.x;
        int h_off = rect.y;
#if DEBUG
        for(int s = 0;s <= STEP;s++)
        {
            cvLine(hand,cvPoint(rect.x,h_off),cvPoint(rect.x + rect.width,h_off),cvScalar(255),1);
            h_off += h_s;
            cvLine(hand,cvPoint(w_off,rect.y),cvPoint(w_off,rect.y + rect.height),cvScalar(255),1);
            w_off += w_s;
        }
#endif
        w_s = rect.width / STEP;
        h_s = rect.height / STEP;
        int half = w_s * h_s;
        for(int p = 0;p < STEP;p++)
        {
            for(int q = 0;q < STEP;q++)
            {
                int count = 0;
                w_off = rect.x + q * w_s;
                h_off = rect.y + p * h_s;
                for(int y = 0;y < h_s;y++)
                {
                    for(int x = 0;x < w_s;x++)
                    {
                        if(IMG8U(hand,w_off + x,h_off + y) == 255)
                            count++;
                    }
                }
                if((double)count / half > 0.5)
                {
                    f[p * STEP + q] = '1';
#if DEBUG
                    cvSetImageROI(feature,cvRect(q * 100,p * 100,100,100));
                    cvSet(feature,cvScalar(255));
#endif
                }else
                    f[p * STEP + q] = '0';
            }
        }
#if DEBUG
        cvShowImage("hand",hand);
        cvResetImageROI(feature_img);
        cvShowImage("feature",feature_img);
        cvWaitKey(10);
#endif
    }else
        rect = cvRect(-1,-1,-1,-1);

    return rect;
    /*char ch;
    if((ch = cvWaitKey(10)) > 0)
    {
        if(is_train)
        {
            if(ch >= '0' && ch <= '9')
            {
                printf("%c:",ch);
                for(int p = 0;p < step;p++)
                {
                    for(int q = 0;q < step;q++)
                        printf("%c",f[p * step + q]);
                    printf("#\n");
                }
            }
            else if(ch == 13 || ch == 27)
                break;
        }
        else
        {
            if(ch == 13 || ch == 27)
                break;
            else
                findHand(f);
        }
    }*/
}

int SVMHandDetector::detect_hand(IplImage *img,CvRect* area)
{
    FeatureData f;
    CvRect rect = get_feature(img,&f);
    if(rect.x == -1)
        return -1;
    *area = rect;
    return find_nearest_hand(&f);
}








