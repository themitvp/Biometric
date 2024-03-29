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

#include "functions.h"

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
        cout << "The project needs 2 arguments to run: path to training set and path to testing set." << endl;
        exit(1);
    }

    // -------------------------------
	// First let's train LBP with the training set
	// -------------------------------
    // Get the path to the CSV.
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
    // These are the parameters for the LBPH model:
    int radius = 1;
    int neighbors = 8;
    int grid_x = 8;
    int grid_y = 8;
    double threshold = 250.0;

    Ptr<FaceRecognizer> model = createLBPHFaceRecognizer(radius,neighbors,grid_x,grid_y,threshold);
    model->setLabelsInfo(labelsInfo);
    model->train(images, labels);

    // Get the generated LBP histograms
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

	// The same parameters are used here for the testing set.
    Ptr<FaceRecognizer> model2 = createLBPHFaceRecognizer(radius,neighbors,grid_x,grid_y,threshold);
    model2->setLabelsInfo(labelsInfo2);
    model2->train(images2, labels2);

    // Get the LBP histograms from the testing set
    vector<Mat> histograms2 = model2->getMatVector("histograms");

    // Custom method to visualize the histograms
    // Uncomment to show the result
    //visualizeHistogram(model, labels2);

    // Create to list that will be combined
    // to a 2-dimensional list.
    std::map<int, std::map<int, double> > testDistances;
    std::map<int, double> minDistance;

    // Calculate the shortest distances from each probe
    // to each data subject in the training set
    for (int i = 0; i < histograms2.size(); i++) {
    	for (int j = 0; j < histograms.size(); j++) {
    		// Use the Chi-Squared distance as metric when comparing
    		double dist = compareHist(histograms[j], histograms2[i], CV_COMP_CHISQR);
    		int trainLabel = labels.at(j);

    		std::map<int, double>::iterator it = minDistance.find(trainLabel);
            if (it != minDistance.end()) {
				if ((dist < minDistance[trainLabel]) && (dist < threshold)) {
					minDistance[trainLabel] = dist;
				}
            } else {
            	// Keep the distance if it is below the threshold
            	// otherwise discard it (-1)
            	if (dist < threshold) {
            		minDistance[trainLabel] = dist;
            	} else {
            		minDistance[trainLabel] = -1;
            	}
            }
    	}
    	// Done with training set for this probe
    	// store data in the 2-dimensional list
    	int testLabel = labels2.at(i);
    	testDistances[testLabel] = minDistance;
    	// Clear and run again with next probe
    	// on the list
    	minDistance.clear();
    }

    // This is used for printing out the similarity score in the console.
    // Uncomment the following to see the result in the console.
    /*for (auto& x: testDistances) {
        cout << "test subject: " << x.first << " (" << labelsInfo2[x.first] << ")" << endl;
        for (auto& y: x.second) {
        	if (y.second != -1) {
				double sim = 100.0 - 100.0/threshold*y.second;
				cout << "   L__train subject: " << y.first << ": " << sim << "%" << endl;
        	} else {
        		cout << "   L__train subject: " << y.first << ": " << "No match" << endl;
        	}
        }
        cout << endl;
	}*/

    // Save the similarity scores in the csv database file for further investigation
    try {
		ofstream myfile;
		 myfile.open ("output/database.csv");
		 cout << "writing database file" << endl;
		 for (auto& x: labelsInfo) {
			 myfile << "," << x.second;
		 }
		 myfile << endl;

		 for (auto& x: testDistances) {
			 myfile << labelsInfo2[x.first];
			 for (auto& y: x.second) {
				if (y.second != -1) {
					// Calculate the similarity score from the Chi-Squared distance
					double sim = (100.0 - 100.0/threshold*y.second)/100;
					myfile << "," << sim;
				} else {
					myfile << ",0";
				}
			 }
			 myfile << endl;
		}
		 cout << "Done with database file" << endl;
		 myfile.close();
    } catch (cv::Exception& e) {
		cerr << "Error opening file. Reason: " << e.msg << endl;
		// nothing more we can do
		exit(1);
    }

	return 0;
}
