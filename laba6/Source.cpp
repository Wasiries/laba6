#include <windows.h>
#include <gl\GL.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <iomanip>
#include <string>
#include <algorithm>

using namespace std;

float const xmax = 10, xmin = -1, ymax = 10, ymin = -1, delta = 0.2, eps = 0.05, EPS = 1e-9;
int width = 800, height = 800, num = 0, len = 0;


double random(double min_, double max_) {
    return min_ + double(rand()) * (max_ - min_) / double(RAND_MAX);
}

struct Point {
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {}
    void show() {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_POINTS);
        glVertex2f(x, y);
        glEnd();
    }
};

Point further = Point(-1000000.0, -1000000.0);

Point convert(int x, int y) {
    return Point(double(xmin) + double(x) * double(xmax - xmin) / double(width),
        double(ymax) - double(y) * double(ymax - ymin) / double(height));
}

struct Edge {
    Point a, b;
    Edge(Point a = Point(), Point b = Point()) : a(a), b(b) {}
    double len() {
        return pow((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y), 0.5);
    }
    double difx() {
        return b.x - a.x;
    }
    double dify() {
        return b.y - a.y;
    }
};


double pseu(Edge first, Edge second) {
    return first.difx() * second.dify() - first.dify() * second.difx();
}
double scal(Edge first, Edge second) {
    return first.difx() * second.difx() + first.dify() * second.dify();
}


//using Polygon = vector<Point>;


vector<Point> hull;
vector<Point> points;

void addPoint(Point point) {
    points.push_back(point);
    num++;
    if (point.x >= further.x) {
        if (point.x > further.x or point.y > further.y) {
            further = point;
        }
    }
}
void clearPoints() {
    points = vector<Point>();
    num = 0;
}
void addRandom() {
    addPoint(Point(random(xmin, xmax), random(ymin, ymax)));
}

//Point previous = further;
//Point current;
//int counter = 0;
//while (true) {
//    Point temp;
//    for (int i = 0; i < num; i++) {
//        current = points[i];
//        bool result = true;
//        for (int j = 0; j < num; j++) {
//            if (pseu(Edge(previous, current), Edge(previous, points[j])) > 0.0) {
//                result = false;
//                continue;
//            }
//        }
//        if (current.x == previous.x and current.y == previous.y) {
//            continue;
//        }
//        if (result == true) {
//            previous = current;
//            hull.push_back(current);
//            len++;
//            counter++;
//            break;
//        }
//    } 
//    if (current.x == further.x and current.y == further.y and counter != 0) {
//        break;
//    }
//}

void makeHull() {
    Point previous = further;
    hull.push_back(further);
    len = 1;
    Point current = further;
    int counter = 0;
    do {
        Point temp = current;
        double ps = pseu(Edge(current, temp), Edge(current, points[0]));
        for (int i = 0; i < num; i++) {
            if (points[i].x == current.x and points[i].y == current.y) {
                continue;
            }
            if (pseu(Edge(current, temp), Edge(current, points[i])) <= ps) {
                ps = pseu(Edge(current, temp), Edge(current, points[i]));
                temp = points[i];
            }
        }
        previous = current;
        current = temp;
        hull.push_back(current);
        len++;
    } while (current.x == further.x and current.y == further.y);
}


int main() {
    addPoint(Point(1, 1));
    addPoint(Point(2, 4));
    addPoint(Point(3, 6));
    addPoint(Point(4, 4));
    addPoint(Point(6, 7));
    addPoint(Point(8, 3));
    addPoint(Point(6, 1));
    addPoint(Point(4, 2));
    addPoint(Point(1, 1));
    addPoint(Point(6, 3));
    addPoint(Point(8, 6));
    makeHull();
    for (int i = 0; i < len; i++) {
        cout << hull[i].x << "\t" << hull[i].y << "\n";
    }
}