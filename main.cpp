#include <iostream>
#include <vector>
#include "event_queue.cpp"

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
            Event_Node next_event = this->Q.pop_next_event();
            this->handle_eventpoint(next_event);
        }
        cout << "returning from get intersections\n";
    }

    void handle_eventpoint(Event_Node ep)
    {

        vector<Line> Up{};
        for (Line l : ep.lines)
        {
            if (!l.isNull())
                Up.push_back(l);
        }
        vector<vector<Line>> result = this->T.get_segments_containing_point(ep.point);
        vector<Line> Lp{};
        for (Line l : result[0])
        {
            if (!l.isNull())
                Lp.push_back(l);
        }
        vector<Line> Cp{};
        for (Line l : result[1])
        {
            if (!l.isNull())
                Cp.push_back(l);
        }
        vector<Line> LC{};
        for (Line l : result[2])
        {
            if (!l.isNull())
                LC.push_back(l);
        }

        vector<Line> UC = Up;
        UC.insert(UC.end(), Cp.begin(), Cp.end());
        vector<Line> LUC = LC;
        LUC.insert(LUC.end(), Up.begin(), Up.end());

        if ((int)LUC.size() > 1)
        {
            cout << "\nInserting point: " << ep.point[0] << ", " << ep.point[1] << "\n";
            cout << "\n";
            for (Line l : LUC)
            {
                cout << "[line " << l.name << "], ";
            }
            this->intersections.push_back(ep.point);
        }
        cout << "sizes: " << LC.size() << " " << LUC.size() << " " << UC.size();
        for (Line l : LC)
        {
            this->T.delete_line(ep.point, l);
        }
        this->T.insert_lines(ep.point, UC);
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

    vector<pair<double, double>> x = {
        {0, 0},
        {1, 2},
        {2, 3},
        {3, 2},
        {2, 1},
        {2, 1},
        {1, 2},
        {1, 1},
        {1, 1.5000},
        {1.5000, 1},
        {0, 0}};
    vector<pair<double, double>> y = {
        {1, 2},
        {2, 3},
        {3, 2},
        {2, 1},
        {0, 0},
        {2, 3},
        {4, 2},
        {1, 1.5},
        {1.5, 1},
        {1, 1},
        {3, 3}};
    // vector<pair<double, double>> x = {

    //     {2, 1},
    //     {1, 2}};
    // vector<pair<double, double>> y = {

    //     {2, 3},
    //     {4, 2}};
    double xc[2] = {};
    double yc[2] = {};

    for (int i = 0; i < (int)x.size(); i++)
    {
        xc[0] = x[i].first;
        xc[1] = x[i].second;
        yc[0] = y[i].first;
        yc[1] = y[i].second;
        Line *temp = new Line(xc, yc);
        lines.push_back(*temp);
        delete temp;
        lines[i].name = (char)(i + (int)'0');
    }
    cout << endl;
    F.get_intersections(lines);
    cout << "\nhere2";
    cout << "f: " << F.intersections.empty() << endl;
    for (vector<double> point : F.intersections)
    {
        cout << "(" << point[0] << ", " << point[1] << ")\n";
    }

    return 0;
}