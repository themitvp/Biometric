/*
 * facerec_hist.cpp
 *
 *  Created on: 17/06/2016
 *      Author: Anders B. Christiansen
 */

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace cv;

void show_histogram(string const& name, Mat const& hist) {
    // Set histogram bins count
    int bins = 256;

    // create matrix for histogram visualization
    int const hist_height = 256;
    Mat3b hist_image = Mat3b::zeros(hist_height, bins);

    double max_val=0;
    minMaxLoc(hist, 0, &max_val);

    // visualize each bin
    for(int b = 0; b < bins; b++) {
        float const binVal = hist.at<float>(b);
        int   const height = cvRound(binVal*hist_height/max_val);
        line (hist_image, Point(b, hist_height-height), Point(b, hist_height), Scalar(0,255,0));
    }
    imshow(name, hist_image);
}

void visualizeHistogram (Ptr<FaceRecognizer> model, vector<int> labels) {

	vector<Mat> histograms = model->getMatVector("histograms");

	//cout << result_message << endl;

	for (int i = 0; i < histograms.size(); ++i) {
		Mat const hist = histograms[i];

		//const std::string image = "s" + string(1,labels[i]) + " - " + string(1,i);
		string result_message = format("subject %d - image %d", labels[i], i);

		imshow(result_message, hist);
		show_histogram(result_message, hist);
	}

    waitKey();
}
