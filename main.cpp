/// \headerfile chrono
/// \brief including chrono library.
#include <chrono>
/// \headerfile vector
/// \brief including standard vector library.
#include "event_queue.cpp"
/// \headerfile fstream
/// \brief including fstream header.
#include <fstream>

/// \namespace std
/// \brief using standard namespace.
using namespace std;

/// \class FindIntersections
/// \brief FindIntersections defines a status structure and even queue needed to find the intersections.
/// This class defines a status structure, an event queue, and a vector of points intersections.
class FindIntersections
{
public:
    /// \brief EventQueue Q to store the event points we find.
    EventQueue Q;
    /// \brief StatusStructure T to store the state of sweep line (line segments intersecting the sweep line in an ordered manner)
    StatusStructure T;
    /// \brief vector<vector<double>> intersections to store intersection points.
    vector<vector<double>> intersections;
    /// \brief Empty Constructor to create a null EventQueue, StatusStructure, and intersections vector.
    FindIntersections()
    {
        EventQueue Q = EventQueue();
        StatusStructure T = StatusStructure();
        vector<vector<double>> intersections = {};
    }
    /// \brief member function to get intersections between lines.
    /// Inserts lines into event queue, and then process the events one by one by popping them.
    /// \param [in] vector<Line> lines set of lines.
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
    }

    /// \brief member function to handle an event point.
    /// fetches segments containing a point. pushes intersection points, checks for new intersections.
    /// \param [in] Event_Node ep the event point to be handled.
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
            this->intersections.push_back(ep.point);
        }
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
    /// \brief member function to search new event (potential intersection point) and inserts them into event queue.
    /// \param [in] Line left left line.
    /// \param [in] Line right right line
    /// \param [in] vector<double> point the event point.
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

/// \brief MAIN DRIVER FUNCTION
/// Creates a vector of lines (values hardcoded for now). Then it sends the line data to a text file to be processed by python for plotting purpose.
/// Fetches all intersection points.
/// Prints all the intersection points.
/// Sends the intersections data to a text file to be processed by python for plotting purpose.
/// return int return value 0
/// retval 0 success
int main()
{

    vector<Line> lines = {};
    FindIntersections F = FindIntersections();
    vector<pair<double, double>> x = {
        {3, 6},
        {3, 2.90}};
    vector<pair<double, double>> y = {
        {5, 1},
        {5, 4.50}};

    double xc[2] /* = {}*/;
    double yc[2] /* = {}*/;
    ofstream line_cord("plot_lines.txt");
    int total_lines = 2;
    cin >> total_lines;
    for (int i = 0; i < total_lines; i++)
    {
        //xc[0] = x[i].first;
        //xc[1] = x[i].second;
        //yc[0] = y[i].first;
        //yc[1] = y[i].second;
        cin >> xc[0] >> xc[1] >> yc[0] >> yc[1];
        line_cord << xc[0] << " " << xc[1] << " " << yc[0] << " " << yc[1] << "\n";
        Line *temp = new Line(xc, yc);
        lines.push_back(*temp);
        delete temp;
        lines[i].name = (char)(i + (int)'0');
    }
    line_cord.close();
    auto start1 = chrono::steady_clock::now();
    F.get_intersections(lines);
    cout << "Total intersection points found: " << F.intersections.size() << "\n";
    auto stop1 = chrono::steady_clock::now();
    cout << "Time taken:" << chrono::duration<double, milli>(stop1 - start1).count() << " ms" << endl;
    ofstream cross_cord("plot_cross.txt");
    for (vector<double> point : F.intersections)
    {
        cout << "(" << point[0] << ", " << point[1] << ") ";
        cross_cord << point[0] << " " << point[1] << "\n";
    }

    return 0;
}