/*
 * Copyright (c) 2011. Philipp Wagner <bytefish[at]gmx[dot]de>.
 * Released to public domain under terms of the BSD Simplified license.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the organization nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 *   See <http://www.opensource.org/licenses/bsd-license>
 */

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace std;

static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, std::map<int, string>& labelsInfo, char separator = ';') {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    string line, path, classlabel, info;
    while (getline(file, line)) {
        stringstream liness(line);
        path.clear(); classlabel.clear(); info.clear();
        getline(liness, path, separator);
        getline(liness, classlabel, separator);
        getline(liness, info);
        if(!path.empty() && !classlabel.empty()) {
            images.push_back(imread(path, CV_LOAD_IMAGE_GRAYSCALE));
            int label = atoi(classlabel.c_str());
            labels.push_back(label);
            labelsInfo.insert(std::make_pair(label, info));
        }
    }
}

int main(int argc, const char *argv[]) {
    // Check for valid command line arguments, print usage
    // if no arguments were given.
    if (argc != 3) {
        cout << "usage: " << argv[0] << " <csv.ext>" << endl;
        exit(1);
    }
    // Get the path to your CSV.
    string fn_csv = string(argv[1]);

    // These vectors hold the images and corresponding labels.
    vector<Mat> images;
    vector<int> labels;
    std::map<int, string> labelsInfo;
    // Read in the data. This can fail if no valid
    // input filename is given.
    try {
        read_csv(fn_csv, images, labels, labelsInfo);
    } catch (cv::Exception& e) {
        cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
        // nothing more we can do
        exit(1);
    }
    // Quit if there are not enough images for this demo.
    if(images.size() <= 1) {
        string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
        CV_Error(CV_StsError, error_message);
    }

    // The following lines create an LBPH model for
    // face recognition and train it with the images and
    // labels read from the given CSV file.
    Ptr<FaceRecognizer> model = createLBPHFaceRecognizer(1,8,8,8,250.0);
    model->setLabelsInfo(labelsInfo);
    model->train(images, labels);

    vector<Mat> histograms = model->getMatVector("histograms");

    // -------------------------------
    // Next let's train the testing set
    // -------------------------------
    // Get the path to your CSV.
	string fn_csv2 = string(argv[2]);

	// These vectors hold the images and corresponding labels.
	vector<Mat> images2;
	vector<int> labels2;
	std::map<int, string> labelsInfo2;
	// Read in the data. This can fail if no valid
	// input filename is given.
	try {
		read_csv(fn_csv2, images2, labels2, labelsInfo2);
	} catch (cv::Exception& e) {
		cerr << "Error opening file \"" << fn_csv2 << "\". Reason: " << e.msg << endl;
		// nothing more we can do
		exit(1);
	}
	// Quit if there are not enough images for this demo.
	if(images2.size() <= 1) {
		string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
		CV_Error(CV_StsError, error_message);
	}

    Ptr<FaceRecognizer> model2 = createLBPHFaceRecognizer(1,8,8,8,250.0);
    model2->setLabelsInfo(labelsInfo2);
    model2->train(images2, labels2);

    vector<Mat> histograms2 = model2->getMatVector("histograms");

    std::map<int, std::map<int, double> > testDistances;

    std::map<int, double> maxDistance;


    for (int i = 0; i < histograms2.size(); i++) {
    	for (int j = 0; j < histograms.size(); j++) {
    		double dist = compareHist(histograms[j], histograms2[i], CV_COMP_CHISQR);
    		int trainLabel = labels.at(j);
    		std::map<int, std::map<int, double> >::iterator it = testDistances.find(trainLabel);
    		if (it != testDistances.end()) {
				if (maxDistance[trainLabel] > dist) {
					maxDistance[trainLabel] = dist;
				}
    		} else {
    			maxDistance[trainLabel] = dist;
    		}
    	}
    	// Done with train set
    	int testLabel = labels2.at(i);
    	testDistances[testLabel] = maxDistance;
    	maxDistance.clear();
    }
    for (auto& x: testDistances) {
        cout << "test subject: " << x.first << " (" << labelsInfo2[x.first] << ")" << endl;
        for (auto& y: x.second) {
        	double sim = 100.0 - 100.0/250.0*y.second;
        	cout << "   L__train subject: " << y.first << ": " << y.second << "%" << endl;
        }
        cout << endl;
	}


	return 0;
}