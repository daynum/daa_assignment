/// \headerfile vector
/// \brief including standard vector library.
#include <vector>
/// \headerfile cmath
/// \brief including standard math library for atan2 function.
#include <cmath>
/// \headerfile iostream
/// \brief including standard i/o library.
#include <iostream>
/// \namespace std
/// \brief using standard namespace.
using namespace std;

/// \class Line Line.cpp
/// \brief Line class for handling line segments.
/// This class defines a line segment, with two end points.\n and provides a few functions to fetch geometric information about a line or pair of lines.
class Line
{
public:
    /// \brief string name stores the name of line segment.
    string name = "nullname";
    /// \brief vector<double> upper_end stores the upper end point of line segment.
    vector<double> upper_end{};
    /// \brief vector<double> lower_end stores the lower end point of line segment.
    vector<double> lower_end{};
    /// \brief Empty line constructor for Line class.
    Line()
    {
        name = "empty_null_line";
        this->upper_end = {};
        this->lower_end = {};
    }
    /// \brief Constructor for Line class with 2 endpoints given.
    Line(double upper[2], double lower[2])
    {
        this->upper_end = {0, 0};
        this->lower_end = {0, 0};
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
    /// \brief member function to check whether the line object is null.
    /// \returns boolean value null or not
    /// \retval true line object is null.
    /// \retval false line object is not null.
    bool isNull()
    {
        return (this->upper_end.empty() && this->lower_end.empty());
    }
    /// \brief finds the intersection point of two lines.
    /// \param [in] Line l second line to find intersection with.
    /// \param [out] ans the intersection point.
    /// \returns ans intersection point.
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
    /// \brief finds the location of a given point with respect to the line.
    /// \param [in] double x x-coordinate of the point.
    /// \param [in] double y y-coordinate of the point.
    /// \returns int relative position of point.
    /// \retval -1 point lies below the line.
    /// \retval 1 point lies above the line.
    /// \retval 0 point lies on the line.
    int pointWRTLine(double x, double y)
    {
        double x1 = this->upper_end[0];
        double y1 = this->upper_end[1];
        double x2 = this->lower_end[0];
        double y2 = this->lower_end[1];

        double res = (x2 - x1) * (y - y1) - (y2 - y1) * (x - x1);
        if (res < -0.0001)
            return -1;
        else if (res > 0.0001)
            return 1;
        else
            return 0;
    }
    /// \brief get the lower angle of the line.
    /// \returns double lower angle in degrees and positive.
    double fetch_lower_angle()
    {
        double angle = atan2((this->lower_end[1] - this->upper_end[1]), (this->lower_end[0] - this->upper_end[0]));
        angle = (angle * 180) / 3.14159265359;
        if (angle <= 0)
            angle += 360;
        return angle;
    }
    /// \brief get the upper angle of the line.
    /// \returns double upper angle in degrees and positive.
    double fetch_upper_angle()
    {
        double angle = atan2((this->upper_end[1] - this->lower_end[1]), (this->upper_end[0] - this->lower_end[0]));
        angle = (angle * 180) / 3.14159265359;
        if (angle < 0)
            angle += 360;
        return angle;
    }
    /// \brief get the x-coordinate of a point on the line, from y-coordinate.
    /// \param [in] double y y-coordinate
    /// \returns double x-coordinate of the point on line.
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
    /// \brief check whether the line is horizontal.
    /// \returns bool line is horizontal or not.
    /// \retval true line is horizontal.
    /// \retval false line is not horizontal.
    bool is_horizontal()
    {
        return this->lower_end[1] == this->upper_end[1];
    }

    /// \brief get lower angle between a point and a line.
    /// \returns double lower angle
    /// \param [in] double x x-coordinate of the point.
    /// \param [in] double y y-coordinate of the point.
    /// \param [in] Line l line to compare the point with
    double compare_lower_angle(double x, double y, Line l)
    {
        int point_location = this->pointWRTLine(x, y);
        if (point_location != 0)
        {
            return -point_location;
        }
        else
        {
            return this->fetch_lower_angle() - l.fetch_lower_angle();
        }
    }
    /// \brief get upper angle between a point and a line.
    /// \returns double upper angle
    /// \param [in] double x x-coordinate of the point.
    /// \param [in] double y y-coordinate of the point.
    /// \param [in] Line l line to compare the point with
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
    /// \brief member function to check whether the line object is equal to other line.
    /// \param [in] Line l2 Second line to compare with.
    /// \returns boolean value equal or not
    /// \retval true both lines are equal.
    /// \retval false lines are not equal.
    bool is_equal(Line l)
    {
        return (this->upper_end == l.upper_end && this->lower_end == l.lower_end);
    }
};