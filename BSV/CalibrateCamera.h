using namespace std;
using namespace cv;

/*
*����˵�������������궨
*���������
PicPath:�궨����ͼ���ļ���·��
Cali_Result������궨�����.txt�ļ���
row_corner_num���궨ͼ���ڽǵ㣨���ͱ�Ե�Ӵ��Ľǵ㣩����
column_corner_num���궨ͼ���ڽǵ㣨���ͱ�Ե�Ӵ��Ľǵ㣩����
grid_width:ʵ�ʲ����õ��ı궨����ÿ�����̸�ĳ�
grid_height:ʵ�ʲ����õ��ı궨����ÿ�����̸�Ŀ�
*�������: ��
*�� �� ֵ����
*/
void CalibrationCamera(string pic_Path, string cali_Result, int row_corner_num,
	int column_corner_num, double grid_width, double grid_height);

void DrawCorners(Mat imageInput, Size board_size, vector<Point2f> image_points_buf, bool patternWasFound, string banner);

void StartRecoginzeCorner(ifstream& fin, ofstream& fout, int image_count, Size image_size, Size board_size, vector<vector<Point2f>> image_points_seq);

void PrintCornerCoordinates(Size board_size, vector<vector<Point2f>> image_points_seq);

vector<vector<Point3f>> InitialCornerCorodinates(int image_count, Size board_size, Size square_size);

vector<int> InitialCornerCounts(int image_count, Size board_size);
