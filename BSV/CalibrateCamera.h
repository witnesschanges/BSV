using namespace std;
using namespace cv;

class CameraCalibration
{
public:
	CameraCalibration();

	/*
	*函数说明：完成左相机标定
	*输入参数：
	PicPath:标定所用图像文件的路径
	Cali_Result：保存标定结果的.txt文件名
	row_corner_num：标定图像内角点（不和边缘接触的角点）行数
	column_corner_num：标定图像内角点（不和边缘接触的角点）列数
	grid_width:实际测量得到的标定板上每个棋盘格的长
	grid_height:实际测量得到的标定板上每个棋盘格的宽
	*输出参数: 无
	*返 回 值：无
	*/
	void CalibrateCamera(string pic_Path, string cali_Result, int row_corner_num,
		int column_corner_num, double grid_width, double grid_height);

	void DrawCorners(Mat imageInput, bool patternWasFound, string banner);

	void StartRecoginzeCorner(ifstream& fin, ofstream& fout);

	void PrintCornerCoordinates();

	vector<vector<Point3f>> InitialCornerCorodinates(Size square_size);

	vector<int> InitialCornerCounts();

	void EvaluateCalibrationResults(vector<int> point_counts, vector<vector<Point3f>> object_points, vector<Mat> tvecsMat,
		vector<Mat> rvecsMat, Mat cameraMatrix, Mat distCoeffs, ofstream& fout);

	void SaveCalibrationResults();

public:
	string m_pic_Path;
	Size m_image_size;
	Size m_board_size;// 标定板上每行、列的角点数
	vector<Point2f> m_image_points_buf;// 缓存每幅图像上检测到的角点
	vector<vector<Point2f>> m_image_points_seq;// 保存检测到的所有角点
	int m_image_count;//图像数量

};