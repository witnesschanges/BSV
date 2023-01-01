using namespace std;
using namespace cv;

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
void CalibrationCamera(string pic_Path, string cali_Result, int row_corner_num,
	int column_corner_num, double grid_width, double grid_height);

void DrawCorners(Mat imageInput, Size board_size, vector<Point2f> image_points_buf, bool patternWasFound, string banner);

void StartRecoginzeCorner(ifstream& fin, ofstream& fout, int image_count, Size image_size, Size board_size, vector<vector<Point2f>> image_points_seq);

void PrintCornerCoordinates(Size board_size, vector<vector<Point2f>> image_points_seq);

vector<vector<Point3f>> InitialCornerCorodinates(int image_count, Size board_size, Size square_size);

vector<int> InitialCornerCounts(int image_count, Size board_size);
