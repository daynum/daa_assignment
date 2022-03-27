#include <vector>
#include <cmath>
#include <iostream>
using namespace std;
class Line
{
public:
    string name = "nullname";
    vector<double> upper_end{0, 0};
    vector<double> lower_end{0, 0};
    Line()
    {
        name = "empty_null_line";
        this->upper_end = {};
        this->lower_end = {};
    }
    Line(double upper[2], double lower[2])
    {
        if (upper[1] > lower[1])
        {
            this->upper_end[0] = upper[0];
            this->upper_end[1] = upper[1];
            this->lower_end[0] = lower[0];
            this->lower_end[1] = lower[1];
        }
        else if (upper[1] < lower[1])
        {
            this->upper_end[0] = lower[0];
            this->upper_end[1] = lower[1];
            this->lower_end[0] = upper[0];
            this->lower_end[1] = upper[1];
        }
        else if (upper[0] < lower[0])
        {
            this->upper_end[0] = upper[0];
            this->upper_end[1] = upper[1];
            this->lower_end[0] = lower[0];
            this->lower_end[1] = lower[1];
        }
        else
        {
            this->upper_end[0] = lower[0];
            this->upper_end[1] = lower[1];
            this->lower_end[0] = upper[0];
            this->lower_end[1] = upper[1];
        }
    }
    bool isNull()
    {
        return (this->upper_end.empty() && this->lower_end.empty());
    }

    bool is_equal(const Line &l2)
    {
        return (this->upper_end == l2.upper_end && this->lower_end == l2.lower_end);
    }

    vector<double> intersection(Line l)
    {
        double x1 = this->upper_end[0];
        double y1 = this->upper_end[1];
        double x2 = this->lower_end[0];
        double y2 = this->lower_end[1];
        double x3 = l.upper_end[0];
        double y3 = l.upper_end[1];
        double x4 = l.lower_end[0];
        double y4 = l.lower_end[1];
        if ((x4 - x3) * (y2 - y1) == (x1 - x2) * (y3 - y4))
        {
            return {};
        }
        double xi = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
        double yi = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
        if (xi < max(min(x1, x2), min(x3, x4)) or xi > min(max(x1, x2), max(x3, x4)))
        {
            return {};
        }
        vector<double> ans{xi, yi};
        return ans;
    }

    int pointWRTLine(double x, double y)
    {
        double x1 = this->upper_end[0];
        double y1 = this->upper_end[1];
        double x2 = this->lower_end[0];
        double y2 = this->lower_end[1];

        double res = (x2 - x1) * (y - y1) - (y2 - y1) * (x - x1);
        if (res < 0)
            return -1;
        else if (res > 0)
            return 1;
        else
            return 0;
    }

    double fetch_lower_angle()
    {
        double angle = atan2((this->lower_end[1] - this->upper_end[1]), (this->lower_end[0] - this->upper_end[0]));
        angle = (angle * 180) / 3.14159265359;
        if (angle < 0)
            angle += 360;
        return angle;
    }

    double fetch_upper_angle()
    {
        double angle = atan2((this->upper_end[1] - this->lower_end[1]), (this->upper_end[0] - this->lower_end[0]));
        angle = (angle * 180) / 3.14159265359;
        if (angle < 0)
            angle += 360;
        return angle;
    }

    double fetch_x_from_y(double y)
    {
        if (this->upper_end[1] == this->lower_end[1])
        {
            return this->lower_end[0];
        }
        else
        {
            double x = (y - this->lower_end[1]) / (this->upper_end[1] - this->lower_end[1]) * (this->upper_end[0] - this->lower_end[0]) + this->lower_end[0];
            return x;
        }
    }

    bool is_horizontal()
    {
        return this->lower_end[1] == this->upper_end[1];
    }

    double compare_lower_angle(double x, double y, Line l)
    {
        int point_location = this->pointWRTLine(x, y);
        if (point_location != 0)
        {
            return -point_location;
        }
        else
        {
            return (this->fetch_lower_angle() - l.fetch_lower_angle());
        }
    }

    double compare_upper_angle(double x, double y, Line l)
    {
        double intersection_point_x = 0, intersection_point_y = 0;
        if (l.is_horizontal())
        {
            intersection_point_x = x;
            intersection_point_y = y;
        }
        else
        {
            intersection_point_x = l.fetch_x_from_y(y);
            intersection_point_y = y;
        }

        int point_location = this->pointWRTLine(intersection_point_x, intersection_point_y);
        if (point_location != 0)
        {
            return -point_location;
        }
        else
        {
            return (l.fetch_upper_angle() - this->fetch_upper_angle());
        }
    }

    bool is_equal(Line l)
    {
        return (this->upper_end == l.upper_end && this->lower_end == l.lower_end);
    }
};