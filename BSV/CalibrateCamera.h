using namespace std;
using namespace cv;

class CameraCalibration
{
public:
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
	CameraCalibration(string pic_Path, string cali_Result, int row_corner_num, int column_corner_num, double grid_width, double grid_height);

	void CalibrateCamera();

	void DrawCorners(Mat imageInput, bool patternWasFound, string banner);

	void StartRecoginzeCorner();

	void PrintCornerCoordinates();

	vector<vector<Point3f>> InitialCornerCorodinates();

	vector<int> InitialCornerCounts();

	void EvaluateCalibrationResults(vector<int> point_counts, vector<vector<Point3f>> object_points);

	void SaveCalibrationResults();

public:
	string m_pic_Path;
	string m_cali_Result;
	Size m_image_size;
	Size m_board_size;// �궨����ÿ�С��еĽǵ���
	Size m_square_size;
	vector<Point2f> m_image_points_buf;// ����ÿ��ͼ���ϼ�⵽�Ľǵ�
	vector<vector<Point2f>> m_image_points_seq;// �����⵽�����нǵ�
	int m_image_count;//ͼ������

	/*�������*/
	Mat m_cameraMatrix;// ������ڲ�������
	Mat m_distCoeffs;// �������5������ϵ����k1, k2, p1, p2, k3
	vector<Mat> m_tvecsMat;// ÿ��ͼ�����ת����
	vector<Mat> m_rvecsMat;// ÿ��ͼ���ƽ������

	ifstream& m_fin;//���ڿհ��лᱨ��
	ofstream& m_fout;
};