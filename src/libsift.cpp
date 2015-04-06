/*
*	File : libsift.cpp
*	Author : Émile Robitaille @ LERobot
*	Creation date : 2015, March 7th
*	Version : 1.0
*
*	Description : Functions relative to sift
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#if CV_VERSION_MAJOR == 2
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#elif CV_VERSION_MAJOR == 3
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#endif

#include "directory.h"
#include "libsift.h"
#include "util.h"

using namespace std;
using namespace cv;
#if CV_VERSION_MAJOR == 3
using namespace xfeatures2d;
#endif


/* CV_LOAD_IMAGE_GRAYSCALE is renamed to IMREAD_GRAYSCALE in OpenCV 3 */
#if CV_VERSION_MAJOR == 3
    #define CV_LOAD_IMAGE_GRAYSCALE IMREAD_GRAYSCALE
#endif



/*
*	Function : sParseArgs
*	Description : Parse the arguments for the sift search program
*
*	argc : argc main argument
*	argv : argv main argument
*/
void sParseArgs(int argc, char *argv[], struct sArgs *args)
{
  char c;

  while ((c = getopt(argc, argv, "vo:")) != -1)
  {
    switch(c)
    {
      case 'v':
        args->verbose = 1;
        break;

      case 'o':
        args->siftPath = optarg;
        break;

      case '?' :
        sUsage(argv[0]);
        break;

      default:
        printf("Error while parsing -%c, read usage below\n", c);
        sUsage(argv[0]);
    }
  }

  if (args->siftPath == NULL)
    args->siftPath = argv[argc-1];
}

/*
*	Function : sUsage
*	Description : Show usage for the sift search program
*
* progName : name of the program
*
*/
void sUsage(char *progName)
{
  printf("This is ulsift (ulavalSFM sift). Use it to find sift points on a dataset.\n");
  printf("Louis-Émile Robitaille @ L3Robot\n");
  printf("usage: mpirun -n [numberOfCores] %s [-v] [-o siftPath] [workingDirectory]\n", progName);
  printf("      -v verbose mode, print a progress bar\n");
  printf("      -o [siftPath] set the sift files repository\n");
  exit(1);
}

/*
*	Function : doSift
*	Description : Find sift points on the image
*
*	path : path of the image
*	container : container for sift keypoints and their descriptor
*/
void doSift(const string &path, struct SFeatures &container)
{
	Mat img, des;
	vector<KeyPoint> keypoints;

	img = imread(path.c_str(), CV_LOAD_IMAGE_GRAYSCALE);

  #if CV_VERSION_MAJOR == 2

	SiftFeatureDetector detector;

  detector.detect(img, keypoints);

  SiftDescriptorExtractor extractor;

  extractor.compute(img, keypoints, des);

  #elif CV_VERSION_MAJOR == 3

  Ptr<SIFT> ptrSIFT = SIFT::create();

  ptrSIFT->detect(img, keypoints);

  ptrSIFT->compute(img, keypoints, des);

  #endif

  container.des = des;
  container.keys = keypoints;
}

/*
*	Function : WriteSiftFile
*	Description : write the keypoints and their descriptor in the sift files (Lowe's binairy format)
*
*	file : path of the .key file
*	container : container for sift keypoints and their descriptor
*/
void writeSiftFile(const string &file, const struct SFeatures &container)
{
	FILE* f = fopen(file.c_str(), "wb");

    fprintf(f, "%d %d \n", container.des.rows, container.des.cols);

    for(int i = 0; i < container.keys.size(); i++)
    {
      	fprintf(f, "%f %f %f %f \n", container.keys.at(i).pt.y, container.keys.at(i).pt.x, container.keys.at(i).size, (container.keys.at(i).angle*M_PI/180.0));
       	for(int j = 0; j < 128; j++)
       	{
       	fprintf(f, "%d ", (int)container.des.at<float>(i,j));
       	if ((j + 1) % 19 == 0) fprintf(f, "\n");
       	}
       	fprintf(f, "\n");
    }

    fclose(f);
}
