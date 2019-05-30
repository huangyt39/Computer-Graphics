#pragma once

#include <vector>
#include <cmath>

using namespace std;
#define PRECISION 2000

struct Point {
	float x;
	float y;
	Point(float _x, float _y) {
		x = _x;
		y = _y;
	}
};

class BezierCurve {
public:
	static void Bezier(const vector<Point> &ctrlPoints, vector<Point> &curvePoints) {
		float t = 0;
		int n = ctrlPoints.size() - 1;

		for (int i = 0; i <= PRECISION; i++) {
			t = (float)(i) / (float)(PRECISION);
			Point p = getCurvePoints(t, ctrlPoints, n);
			curvePoints.push_back(p);
		}
	}

	static Point getCurvePoints(float t, const vector<Point> &ctrlPoints, int n) {
		vector<Point> temp;
		Point p(0, 0);
		for (int i = 0; i < n; i++) {
			p.x = ctrlPoints[i].x * (1 - t) + ctrlPoints[i + 1].x * t;
			p.y = ctrlPoints[i].y * (1 - t) + ctrlPoints[i + 1].y * t;
			temp.push_back(p);
		}
		while (temp.size() != 1) {
			vector<Point> next;
			for (int i = 0; i < temp.size() - 1; i++) {
				p.x = temp[i].x * (1 - t) + temp[i + 1].x * t;
				p.y = temp[i].y * (1 - t) + temp[i + 1].y * t;
				next.push_back(p);
			}
			temp = next;
		}
		return temp[0];
	}

	static void getActionPoints(int step, const vector<Point> &ctrlPoints, vector<Point> &actionPoints) {
		float t = (float)step / (float)PRECISION;
		int n = ctrlPoints.size() - 1;

		vector<Point> temp;
		Point p(0, 0);
		for (int i = 0; i < n; i++) {
			p.x = ctrlPoints[i].x * (1 - t) + ctrlPoints[i + 1].x * t;
			p.y = ctrlPoints[i].y * (1 - t) + ctrlPoints[i + 1].y * t;
			actionPoints.push_back(p);
			temp.push_back(p);
		}
		while (temp.size() != 1) {
			vector<Point> next;
			for (int i = 0; i < temp.size() - 1; i++) {
				p.x = temp[i].x * (1 - t) + temp[i + 1].x * t;
				p.y = temp[i].y * (1 - t) + temp[i + 1].y * t;
				actionPoints.push_back(p);
				next.push_back(p);
			}
			temp = next;
		}
		actionPoints.push_back(temp[0]);
	}
};