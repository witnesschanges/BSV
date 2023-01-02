using namespace std;
using namespace cv;

class CameraCalibration
{
public:
	CameraCalibration();

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
	Size m_board_size;// �궨����ÿ�С��еĽǵ���
	vector<Point2f> m_image_points_buf;// ����ÿ��ͼ���ϼ�⵽�Ľǵ�
	vector<vector<Point2f>> m_image_points_seq;// �����⵽�����нǵ�
	int m_image_count;//ͼ������

};