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
bool showable = false;


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


vector<Point> hull;
vector<Point> points;

void show_points() {
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POINTS);
    for (int i = 0; i < num; i++) {
        glVertex2f(points[i].x, points[i].y);
    }
    glEnd();
}

void show_hull() {
    if (showable) {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINES);
        for (int i = 0; i < len; i++) {
            glVertex2f(hull[i].x, hull[i].y);
            glVertex2f(hull[(i + 1) % len].x, hull[(i + 1) % len].y);
        }
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    show_hull();
    show_points();
    glFlush();
}

void add_point(Point point) {
    showable = false;
    points.push_back(point);
    num++;
    if (point.x >= further.x) {
        if (point.x > further.x or point.y > further.y) {
            further = point;
        }
    }
}
void clear_points() {
    points = vector<Point>();
    num = 0;
}
void clear_hull() {
    len = 0;
    hull = vector<Point>();
    showable = false;
}
void clear_all() {
    clear_points();
    clear_hull();
}
void add_random() {
    add_point(Point(random(xmin, xmax), random(ymin, ymax)));
}

void make_hull() {
    Point previous = further;
    Point current;
    int counter = 0;
    while (true) {

        for (int i = 0; i < num; i++) {
            current = points[i];
            bool result = true;
            for (int j = 0; j < num; j++) {
                if (pseu(Edge(previous, current), Edge(previous, points[j])) > 0.0) {
                    result = false;
                    break;
                }
            }
            if (current.x == previous.x and current.y == previous.y) {
                continue;
            }
            if (result == true) {
                previous = current;
                hull.push_back(current);
                len++;
                counter++;
                break;
            }
        }
        if (current.x == further.x and current.y == further.y and counter != 0) {
            break;
        }
    }
}


void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        if (button == GLUT_LEFT_BUTTON) {
            add_point(convert(x, y));
            clear_hull();
        }
        if (button == GLUT_RIGHT_BUTTON) {
            clear_all();
        }
    }
    display();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'f') {
        make_hull();
        showable = true;
    }
    display();
}


void init(void) {
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(50, 10);
    glutCreateWindow("Okna 11");
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_POINT_SMOOTH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(xmin, xmax, ymin, ymax);
    glPointSize(8);
    glLineWidth(3);
    glClearColor(0.2, 0.3, 0.9, 1.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}