
using namespace cv;
using namespace std;


void ConnectToEach(Mat src, QLabel* label)
{
			QImage image = cvMat2QImage(src);
			label->setPixmap(QPixmap::fromImage(image));
			label->resize(src.cols, src.rows);
			label->show();
}

