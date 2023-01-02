using namespace std;
using namespace cv;

class CameraCalibration
{
public:
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
	CameraCalibration(string pic_Path, string cali_Result, int row_corner_num, int column_corner_num, double grid_width, double grid_height);

	vector<vector<Point3f>> InitialCornerCorodinates();

	vector<int> InitialCornerCounts();

	void DrawCorners(Mat imageInput, bool patternWasFound, string banner);

	void ShowImageWindow(string banner, Mat imageInput);

	void StartRecoginzeCorner();

	void PrintCornerCoordinates();

	void EvaluateCalibrationResults();

	void SaveCalibrationResults();

	void StoreAndDisplayCalibrationResults();

	void CalibrateCamera();

public:
	string m_pic_Path;
	string m_cali_Result;
	Size m_image_size;
	Size m_board_size;// 标定板上每行、列的角点数
	Size m_square_size;
	vector<Point2f> m_image_points_buf;// 缓存每幅图像上检测到的角点
	vector<vector<Point2f>> m_image_points_seq;// 保存检测到的所有角点
	int m_image_count;//图像数量

	/*内外参数*/
	Mat m_cameraMatrix;// 摄像机内参数矩阵
	Mat m_distCoeffs;// 摄像机的5个畸变系数：k1, k2, p1, p2, k3
	vector<Mat> m_tvecsMat;// 每幅图像的旋转向量
	vector<Mat> m_rvecsMat;// 每幅图像的平移向量

	vector<vector<Point3f>> m_object_points;// 每幅图像中角点的三维坐标
	vector<int> m_point_counts;// 每幅图像中角点的数量

	Mat m_rotation_matrix;// 保存每幅图像的旋转矩阵

	ifstream& m_fin;//存在空白行会报错
	ofstream& m_fout;
};