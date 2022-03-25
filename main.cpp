#include <iostream>
#include <vector>
#include <status_structure.cpp>
#include <line.cpp>
#include <event_queue.cpp>

using namespace std;
class FindIntersections
{
public:
    EventQueue Q;
    StatusStructure T;
    vector<vector<double>> intersections;
    FindIntersections()
    {
        EventQueue Q = EventQueue();
        StatusStructure T = StatusStructure();
        vector<vector<double>> intersections = {};
    }
    void get_intersections(vector<Line> lines)
    {
        for (Line l : lines)
        {
            this->Q.insert_line(l);
        }

        while (!(this->Q.is_null()))
        {
            Node next_event = this->Q.pop_next_event();
            this->handle_eventpoint(next_event);
        }
    }

    void handle_eventpoint(Node ep)
    {
        vector<Line> Up = ep.lines;
        vector<vector<Line>> result = this->T.get_segments_containing_point(ep.point);
        vector<Line> Lp = result[0];
        vector<Line> Cp = result[1];
        vector<Line> LC = result[2];

        vector<Line> UC = Up;
        UC.insert(UC.end(), Cp.begin(), Cp.end());
        vector<Line> LUC = LC;
        LUC.insert(LUC.end(), Up.begin(), Up.end());

        if ((int)LUC.size() > 1)
        {
            this->intersections.push_back(ep.point);
        }

        for (Line l : LC)
        {
            this->T.delete_line(ep.point, l);
        }

        this->T.insert_lines(ep.point, UC);
        // this->T.print_name();

        if ((int)UC.size() == 0)
        {
            Line left_line = this->T.get_adjacent_left_line(ep.point);
            Line right_line = this->T.get_adjacent_right_line(ep.point);
            this->search_newevent(left_line, right_line, ep.point);
        }
        else
        {
            Line leftmost_line = this->T.get_leftmost(ep.point);
            Line left_line = this->T.get_adjacent_left_line(ep.point);
            this->search_newevent(leftmost_line, left_line, ep.point);

            Line rightmost_line = this->T.get_rightmost(ep.point);
            Line right_line = this->T.get_adjacent_right_line(ep.point);
            this->search_newevent(rightmost_line, right_line, ep.point);
        }
    }

    void search_newevent(Line left, Line right, vector<double> point)
    {
        if (left.isNull() || right.isNull())
        {
            return;
        }

        vector<double> x = left.intersection(right);

        if (x.empty())
            return;
        double xx = x[0];
        double yx = x[1];

        double xp = point[0];
        double yp = point[1];

        if (yx < yp || (yx == yp and xx > xp))
        {
            this->Q.insert_point(x);
        }
    }
};

int main()
{
    vector<Line> lines = {};
    FindIntersections F = FindIntersections();
    F.get_intersections(lines);

    for (vector<double> point : F.intersections)
    {
        cout << "x-cord: " << point[0] << ", y-cord: " << point[1] << "\n";
    }

    return 0;
}