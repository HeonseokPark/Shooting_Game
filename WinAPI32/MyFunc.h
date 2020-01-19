#pragma once
#define PI 3.141592654f
#define TORADIAN 0.017435f  // PI / 180.0f
//float �� ����ü
typedef struct tagFRECT
{
	float left;
	float top;
	float right;
	float bottom;
} FRECT, *PFRECT;

typedef struct tagFPOINT
{
	float  x;
	float  y;
} FPOINT, *PFPOINT;

typedef struct tagFCIRCLE
{
	float  x;
	float  y;
	float  r;
} FCIRCLE, *PFCIRCLE;

//�Ÿ� ���ϱ�
static float GetDistance(float x1, float y1, float x2, float y2)
{
	float x = x1 - x2;
	float y = y2 - y1;

	return sqrtf(x * x + y * y);
}
//���� ���ϱ�
static float GetRadian(float x1, float y1, float x2, float y2)
{
	float x = x2 - x1;//a
	float distance = GetDistance(x1, y1, x2, y2);//c
	float angle = acosf(x / distance);
	if (y2 > y1)
	{
		angle = PI + (PI - angle);
	}

	return angle;
}
static float GetDegree(float x1, float y1, float x2, float y2)
{
	float angle = GetRadian(x1, y1, x2, y2);
	angle /= TORADIAN;
	return angle;
}
//���� ������ �Լ�
static void ReverseAngle(float &angle, bool isRadian, bool AxisX, bool AxisY)
{
	float angle180;
	//���׿�����
	(isRadian) ? angle180 = PI : angle180 = 180.0f;
	if (AxisX) angle = angle180 - angle;
	if (AxisY) angle = angle180 + angle180 - angle;
}


//�׸� �����
static FRECT RectMake(const float x, const float y, const float width, const float height)
{
	FRECT rc = { x , y , x + width , y + height };
	return rc;
}
//�����߽� �׸� �����
static FRECT RectMakeCenter(const float x, const float y, const float width, const float height)
{
	float HalfWidth = width / 2.0f;
	float HalfHeight = height / 2.0f;
	FRECT rc = { x - HalfWidth ,y - HalfHeight ,x + HalfWidth ,y + HalfHeight };
	return rc;
}
//POINT �����
static FPOINT PointMake(const float x, const float y)
{
	FPOINT pt = { x,y };
	return pt;
}
//�� �����
static FCIRCLE CircleMake(const float x, const float y, const float r)
{
	FCIRCLE cc = { x,y,r };
	return cc;
}


//�浹 �Լ�

//�� �浹. rc1 = ĳ���� ��Ʈ, rc2 = �� ��Ʈ
static bool isMapCollision(const FRECT rc1, const FRECT rc2)
{
	if (rc1.left <= rc2.left ||  rc1.top <= rc2.top || rc1.right >= rc2.right || rc1.bottom >= rc2.bottom)
	{
		return true;
	}
	return false;
}


//�׸�,�׸�
static bool IntersectRect(const FRECT rc1, const FRECT rc2)
{
	if (rc1.right >= rc2.left &&
		rc1.left <= rc2.right &&
		rc1.top <= rc2.bottom &&
		rc1.bottom >= rc2.top)
	{
		return true;
	}
	return false;
}
//�׸�,��
static bool PtInRect(const FRECT Rect, const FPOINT InPt)
{
	if (Rect.left <= InPt.x && InPt.x <= Rect.right &&
		Rect.top <= InPt.y && InPt.y <= Rect.bottom)
	{
		return true;
	}
	return false;
}
//��,��
static bool PtInCircle(const FCIRCLE cc, const FPOINT InPt)
{
	if (cc.r >= GetDistance(cc.x, cc.y, InPt.x, InPt.y))
	{
		return true;
	}
	return false;
}

//��,��
static bool IntersectCircle(const FCIRCLE cc1, const FCIRCLE cc2)
{
	if (cc1.r+cc2.r >= GetDistance(cc1.x, cc1.y, cc2.x, cc2.y))
	{
		return true;
	}
	return false;
}


//�׸� ��
static bool IntersectRectCircle(const FRECT rc, const FCIRCLE cc)
{
	//�𼭸� ���� �׸� ���浹
	FRECT temp1{ rc.left,rc.top - cc.r , rc.right,rc.bottom + cc.r };
	FRECT temp2{ rc.left - cc.r,rc.top, rc.right + cc.r,rc.bottom };
	if (PtInRect(temp1, PointMake(cc.x, cc.y)))return true;
	if (PtInRect(temp2, PointMake(cc.x, cc.y)))return true;

	//�𼭸� �浹
	if (PtInCircle(cc, FPOINT{ rc.left, rc.top }))return true;
	if(PtInCircle(cc, PointMake(rc.left, rc.bottom)))return true;
	if(PtInCircle(cc, PointMake(rc.right, rc.top)))return true;
	if(PtInCircle(cc, PointMake(rc.right, rc.bottom)))return true;

	return false;
}

//�׸��� �Լ�
//��ġ������ �׸���
//�׸� �׸���
static void RectangleMake(HDC hdc, int x, int y, int width, int height)
{
	Rectangle(hdc, x, y, x + width, y + height);
}
//���߱�
static void LineMake(HDC hdc, int x1, int y1, int x2, int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}
//Ÿ���׸���
static void EllipseMake(HDC hdc, int x, int y, int width, int height)
{
	Ellipse(hdc, x, y, x + width, y + height);
}
//����ü �־ �׸���
//���׸���
static void DrawfCircle(const HDC hdc, const FCIRCLE cc)
{
	Ellipse(hdc, static_cast<int>(round(cc.x - cc.r)), static_cast<int>(round(cc.y - cc.r)),
		static_cast<int>(round(cc.x + cc.r)), static_cast<int>(round(cc.y + cc.r)));
}
//���׸���
static void DrawfLine(const HDC hdc, const FPOINT pt1, const FPOINT pt2)
{
	MoveToEx(hdc, static_cast<int>(round(pt1.x)), static_cast<int>(round(pt1.y)), NULL);
	LineTo(hdc, static_cast<int>(round(pt2.x)), static_cast<int>(round(pt2.y)));
}
//�׸�׸���
static void DrawfRect(const HDC hdc, const FRECT rc)
{
	Rectangle(hdc, static_cast<int>(round(rc.left)), static_cast<int>(round(rc.top)),
		static_cast<int>(round(rc.right)), static_cast<int>(round(rc.bottom)));
}


//����
template <typename T>
static void CustomSwap(T &a, T &b)
{
	T temp;
	temp = a;
	a = b;
	b = temp;

}
//����
template <typename T>
static void CustomShuffle(T a[], unsigned int time, unsigned int range)
{
	for (UINT i = 0; i < time; i++)
	{
		unsigned int dest = rand() % range;
		unsigned int sour = rand() % range;
		T temp = a[dest];
		a[dest] = a[sour];
		a[sour] = temp;
	}
}

//���� �Լ�

//	0 ���� ���� int �� ��������
static int GetInt(int num)
{
	return rand() % num;
}
//	������ ���� int �� ��������
static int GetFromIntTo(int fromNum, int toNum)
{
	return rand() % (toNum - fromNum + 1) + fromNum;
}
//	0 ���� ���� float �� ��������
static float GetFloat(float num)
{
	return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * num;
}
//	������ ���� float �� ��������
static float GetFromFloatTo(float fromNum, float toNum)
{
	float rnd = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	return (rnd * (toNum - fromNum) + fromNum);
}