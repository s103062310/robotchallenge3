#include <caffe/caffe.hpp>
#ifdef USE_OPENCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#endif  // USE_OPENCV
#include <algorithm>
#include <iosfwd>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <iostream> //
#include <sstream> //
#include <cstdio> //
#include <ctime> //
#ifdef USE_OPENCV
#define THERESHOLD 50
using namespace caffe;  // NOLINT(build/namespaces)
using std::string;
using namespace cv;
using namespace std;

/* Pair (label, confidence) representing a prediction. */
typedef pair<string, float> Prediction;

/* Return the indices of the top N values of vector v. */
static vector<int> Argmax(const vector<float>& v, int N);
static bool PairCompare(const std::pair<float, int>& lhs,
                        const std::pair<float, int>& rhs);

class Classifier {

public:
	
	Classifier(const string& model_file, 
			   const string& trained_file, 
			   const string& mean_file, 
			   const string& label_file);
	vector<Prediction> Classify(const Mat& img, int N = 1);

private:

	void SetMean(const string& mean_file);
	vector<float> Predict(const Mat& img);
	void WrapInputLayer(vector<Mat>* input_channels);
	void Preprocess(const Mat& img, vector<Mat>* input_channels);

private:

	shared_ptr<Net<float>> net_;
	Size input_geometry_;
	int num_channels_;
	Mat mean_;
	vector<string> labels_;
	
};
