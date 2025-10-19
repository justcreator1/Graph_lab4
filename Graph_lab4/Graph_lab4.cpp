

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#define PI 3.14159265358979323846 

using namespace std;

struct RGB {
    unsigned char r, g, b;
};
struct Point {
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
};

double Sign(double x)
{
    if (x > 0) {
        return 1.0;
    }
    if (x == 0) {
        return 0;
    }
    return -1.0;
}

class Canvas1 {
public:
    int width, height;
    std::vector<RGB> pixels;
    Canvas1(const int& w, const int& h)
    {
        width = w;
        height = h;
        pixels.assign(width * height, { 255,255,255 });
    }
    void DrawPixel(int x, int y, RGB& color)
    {
        pixels[y * width + x] = color;
    }

    bool Save_Canvas(const string& filepath)
    {
        ofstream stream(filepath, ios::binary);
        if (!stream) {
            cout << "не удалось создать файл" << endl;
            return false;
        }
        stream << "P6\n" << width << " " << height << "\n255\n";
        stream.write(reinterpret_cast<const char*>(pixels.data()), pixels.size() * sizeof(RGB));
        return true;
    }
    void Brezenhem(Point start, Point end, RGB color)
    {
        double dx = (double)(end.x - start.x);
        double dy = (double)(end.y - start.y);
        double sx = Sign(dx);
        double sy = Sign(dy);
        dx = abs(dx);
        dy = abs(dy);
        int flag = 0;
        if (dx < dy) {
            double t = dx;
            dx = dy;
            dy = t;
            flag = 1;
        }
        double f = dy / dx - 0.5;
        double x = start.x;
        double y = start.y;
        for (int i = 0; i < dx; i++)
        {
            DrawPixel(floor(x), floor(y), color);
            if (f >= 0) {
                if (flag == 1) {
                    x = x + sx;
                }
                else {
                    y = y + sy;
                }
                f -= 1;
            }
            if (flag == 1) {
                y = y + sy;
            }
            else {
                x = x + sx;
            }
            f = f + dy / dx;
        };
    }
    void DrawCirclePoints(int xc, int yc, int xs, int ys, int xe, int ye, RGB color) {
        Brezenhem(Point(xc + xs, yc + ys), Point(xc + xe, yc + ye), color);
        Brezenhem(Point(xc - xs, yc + ys), Point(xc - xe, yc + ye), color);
        Brezenhem(Point(xc + xs, yc - ys), Point(xc + xe, yc - ye), color);
        Brezenhem(Point(xc - xs, yc - ys), Point(xc - xe, yc - ye), color);
        Brezenhem(Point(xc + ys, yc + xs), Point(xc + ye, yc + xe), color);
        Brezenhem(Point(xc - ys, yc + xs), Point(xc - ye, yc + xe), color);
        Brezenhem(Point(xc + ys, yc - xs), Point(xc + ye, yc - xe), color);
        Brezenhem(Point(xc - ys, yc - xs), Point(xc - ye, yc - xe), color);
    }

    void Equation(int xc, int yc, int r, RGB color) {
        int x = 0;
        int y = r;

        while (x <= y) {
            int ys = (int)(sqrt(r * r - x * x) + 0.5);
            x++;
            y = (int)(sqrt(r * r - x * x) + 0.5);
            DrawCirclePoints(xc, yc, x-1, ys,x,y, color);
        }
    }
    void Parametric(int xc, int yc, int r, RGB color) {
        int steps = 8 * r; 

        for (int i = 0; i < steps;) {
            double t = 2 * PI * i / steps;
            int xs = (int)(r * cos(t) + 0.5);
            int ys = (int)(r * sin(t) + 0.5);
            i++;
            t = 2 * PI * i / steps;
            int xe = (int)(r * cos(t) + 0.5);
            int ye = (int)(r * sin(t) + 0.5);
            Brezenhem(Point(xc+xs, xc+ys),Point(xc+xe, xc+ye), color);
        }
    }

    void Bresenham(int xc, int yc, int r, RGB color) {
        int x = 0;
        int y = r;
        int d = 3 - 2 * r;
        vector<Point> p;
        p.push_back(Point(x, y));
        while (y >= x) {
            x++;

            if (d > 0) {
                y--;
                d = d + 4 * (x - y) + 10;
            }
            else {
                d = d + 4 * x + 6;
            }
            p.push_back(Point(x, y));
        }
        
        for (int i = 1; i < p.size(); i++) {
            DrawCirclePoints(xc, yc, p[i - 1].x, p[i - 1].y, p[i].x, p[i].y, color);
        }
    }
    
};

int main()
{
    int l;
    cout << "Enter size of line:";
    cin>>l;
    int r1 = l / (2 * sin(PI / 5));
    int r2 = l / (2 * tan(PI / 5));
    Canvas1 eq(2*r1+20, 2 * r1 + 20);
    eq.Equation(10+r1, 10+r1, r1, { 0,0,0 });
    eq.Equation(10 + r1, 10 + r1, r2, { 0,0,0 });
    eq.Save_Canvas("eq.ppm");
    Canvas1 prm(2 * r1 + 20, 2 * r1 + 20);
    prm.Parametric(10 + r1, 10 + r1, r1, { 0,0,0 });
    prm.Parametric(10 + r1, 10 + r1, r2, { 0,0,0 });
    prm.Save_Canvas("prm.ppm");
    Canvas1 bre(2 * r1 + 20, 2 * r1 + 20);
    bre.Bresenham(10 + r1, 10 + r1, r1, { 0,0,0 });
    bre.Bresenham(10 + r1, 10 + r1, r2, { 0,0,0 });
    bre.Save_Canvas("bre.ppm");
}

