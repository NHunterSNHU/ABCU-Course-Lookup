#include <iostream>
#include <fstream>
#include <ranges>
#include <utility>
#include <vector>
using namespace std;

struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;

    Course(string courseNumber, string courseTitle, vector<string> prerequisites) {

        if (courseNumber.empty()) {throw invalid_argument("Course number cannot be empty.");}
        this->courseNumber = std::move(courseNumber);

        if (courseTitle.empty()) {throw invalid_argument("Course title cannot be empty.");}
        this->courseTitle = std::move(courseTitle);

        this->prerequisites = std::move(prerequisites);
    }

    //Print only the number and title
    void print() const {
        cout << courseNumber << ", " << courseTitle << endl;
    }

    //Print only the prerequisites
    void printPrerequisites() {
        if (!prerequisites.empty()) {
            cout << "Prerequisites: ";
            for (int i = 0; i < prerequisites.size(); i++) {
                cout << prerequisites[i];
                if (i != prerequisites.size() - 1) {
                    cout << ", ";
                }
            }
            cout << endl;
        } else {
            cout << "No prerequisites." << endl;
        }
    }

};

struct Node {
    Course course;
    Node* left{};
    Node* right{};
};

struct BinarySearchTree {
    Node* root{};

    //Sorting a new item into it's place in the tree
    void addNode(Course &course, Node* node) {
        if (course.courseNumber < node->course.courseNumber) {
            if (node->left == nullptr) {
                node->left = new Node(course);
            } else {
                addNode(course, node->left);
            }
        } else {
            if (node->right == nullptr) {
                node->right = new Node(course);
            } else {
                addNode(course, node->right);
            }
        }
    }

    //Default addNode behavior
    void addNode(Course &course) {
        if (root == nullptr) {
            root = new Node(course);
        } else {
            addNode(course, root);
        }
    }

    //Iterate and print through each node in the tree in order.
    void printInOrder(Node* node) {
        if (node == nullptr)
            return;
        printInOrder(node->left);
        node->course.print();
        printInOrder(node->right);
    }

    //Returns a course if a course.courseNumber in the tree matches the given string
    [[nodiscard]] Course* Search(const string& courseNumber) const {
        Node* node = root;

        while (node != nullptr) {
            if (node->course.courseNumber == courseNumber) {
                return &node->course;
            }

            if (courseNumber < node->course.courseNumber) {
                node = node->left;
            } else {
                node = node->right;
            }
        }

        cout << "Course not found." << endl;
        return nullptr;
    }

};

//Basic CSV parser for parsing the file and filling the binary tree.
BinarySearchTree loadDataStructureFromFile(const string& filename) {
    BinarySearchTree tree;

    auto fs = fstream(filename);
    if (!fs.is_open()) {
        cout << "Could not open file." << endl;
        return tree;
    }

    //Iterate through each line
    string line;
    while (getline(fs, line)) {
        vector<string> courseData;

        //Fill a temporary vector with the line data
        for (auto const &segment : line | views::split(',')) {
            if (!segment.empty()) {
                courseData.emplace_back(segment.begin(), segment.end());
            }
        }

        //Ensure courseNumber is all uppercase
        for (char &character : courseData[0]) {
            character = static_cast<char>(toupper(character));
        }

        //Add it to the tree
        Course course(courseData[0], courseData[1], vector<string>(courseData.begin() + 2, courseData.end()));
        tree.addNode(course);
    }

    return tree;
}

int main() {

    string filename;
    BinarySearchTree tree;

    cout << "Welcome to the course planner." << endl;

    int choice = -1;
    while (choice != 9) {

        cout << endl;
        cout << "   1. Load Data Structure." << endl;
        cout << "   2. Print Course List." << endl;
        cout << "   3. Print Course." << endl;
        cout << "   9. Exit" << endl;
        cout << endl;
        cout << "What would you like to do?: ";
        cin >> choice;


        switch (choice) {

            //Load Data Structure
            case 1:
                cin.ignore(10000, '\n');
                cout << "Enter the filename, or press enter to read the default file: ";
                getline(cin, filename);

                if (filename.empty()) {
                    cout << "No file name given. Reading from default file." << endl;
                    filename = "CS 300 ABCU_Advising_Program_Input.csv";
                }

                cout << endl;
                cout << "Loading data structure." << endl;
                tree = loadDataStructureFromFile(filename);
                break;

            //Print Course List
            case 2:
                cout << endl;
                cout << "Here is a sample schedule:" << endl;
                cout << endl;

                tree.printInOrder(tree.root);
                break;

            //Print Course
            case 3:
            {
                string courseNumber;

                cout << "What course do you want to know about?: ";
                cin >> courseNumber;

                for (char &character : courseNumber) {
                    character = static_cast<char>(toupper(character));
                }

                Course* course = tree.Search(courseNumber);

                if (course != nullptr) {
                    cout << endl;
                    course->print();
                    course->printPrerequisites();
                }

                break;
            }

            //Exit
            case 9:
                cout << "Thank you for using the course planner!";
                return 0;
            default:
                cout << choice << " is not a valid option." << endl;
        }

    }

}