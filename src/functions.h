/*
 * histogram.h
 *
 *  Created on: 19/06/2016
 *      Author: Anders B. Christiansen
 */

#ifndef SRC_FUNCTIONS_H_
#define SRC_FUNCTIONS_H_

#include <iostream>
using namespace std;
using namespace cv;

// histogram.cpp
extern void visualizeHistogram(Ptr<FaceRecognizer> model, vector<int> labels);

#endif
