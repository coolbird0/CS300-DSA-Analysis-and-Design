//=========================================================================== =
// Name        : ProjectTwo.cpp
// Author      : Owen Walker Jr 4/20, 2026
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <Windows.h>
#include <vector>

using namespace std;

const int GLOBAL_SLEEP_TIME = 2000; //default time for sleep

struct Course {

	string courseId;
	string courseName;
	vector<string> prelist;

};

struct Node {
	Course course;
	Node* left;
	Node* right;
	//default Constructor for Node
	Node() {
		left = nullptr;
		right = nullptr;
	}

	Node(Course aCourse) {

		course = aCourse;
		left = nullptr;
		right = nullptr;
	}

};

class BinarySearchTree {
private:

	void inOrder(Node* node);
	Node* root;
	Node* removeNode(Node* node, string courseId);
	int size = 0;
public:
	BinarySearchTree();
	void addNode(Node* node, Course course);
	void InOrder();
	void Insert(Course course);
	Course Search(string courseId);
	void Remove(string courseId);
	int Size();
};

BinarySearchTree::BinarySearchTree() {
	root = nullptr;
}

void BinarySearchTree::InOrder() {
	inOrder(root);
}

void BinarySearchTree::Insert(Course course) {
	if (root == nullptr) {
		root = new Node(course);
	}
	else {
		addNode(root, course);
	}
	size++;
}
void BinarySearchTree::addNode(Node* node, Course course) {
	//check if node's courseId is less than courseId
	if (node->course.courseId.compare(course.courseId) > 0) {
		//check whether node's left pointer is null if it is assign with a new node with course as argument
		if (node->left == nullptr) {
			//this node becomes left
			node->left = new Node(course);
		}
		else {
			//otherwise if node's left is filled recurse down nodes left pointer
			addNode(node->left, course);
		}
	}
	//otherwise courseId is larger
	else {
		//check if node's right pointer is empty/null
		if (node->right == nullptr) {
			//this node becomes right
			node->right = new Node(course);
		}
		else {
			//otherwise if node's right is filled recurse down nodes right pointer
			addNode(node->right, course);
		}
	}
}

void caseConverter(string& toConvert) {
	for (unsigned int i = 0; i < toConvert.length(); i++) {
		if (isalpha(toConvert.at(i))) {
			toConvert.at(i) = toupper(toConvert.at(i));
		}
	}
}

void BinarySearchTree::Remove(string courseId) {
	//call removeNode passing root and courseId as argument
	root = removeNode(root, courseId);
}
int BinarySearchTree::Size() {
	return size;
}
Node* BinarySearchTree::removeNode(Node* node, string courseId) {
	//check if the node to be removed is the root node if it is return node
	if (node == nullptr) {
		return node;
	}
	//(otherwise recurse down left subtree and check for match, if so remove left node using recursive call
	if (courseId.compare(node->course.courseId) < 0) {
		node->left = removeNode(node->left, courseId);
		return node;
	}
	//Otherwise recurse down right subtree and check for match, if so remove rightg node using recursive call
	else if (courseId.compare(node->course.courseId) > 0) {
		node->right = removeNode(node->right, courseId);
		return node;
	}
	else {
		// if left node = nullptr && right node = nullptr delete node 
		if (node->left == nullptr && node->right == nullptr) {
			delete node;
			node = nullptr;
		}
		// (otherwise check one child to the left)
		// if left node != nullptr && right node = nullptr delete node 
		else if (node->left != nullptr && node->right == nullptr) {
			Node* temp = node;
			node = node->left;
			delete temp;
		}
		// (otherwise check one child to the right)
		// if left node = nullptr && right node != nullptr delete node
		else if (node->left == nullptr && node->right != nullptr) {
			Node* temp = node;
			node = node->right;
			delete temp;
		}
		// (otherwise more than one child so find the minimum)
		else if (node->left != nullptr && node->right != nullptr) {
			// create temp node to right
			Node* temp = node->right;
			// while left node is not nullptr keep moving temp left
			while (temp->left != nullptr) {
				temp = temp->left;
			}
			// make node bid (right) equal to temp bid (left)
			node->course = temp->course;
			// remove right node using recursive call
			node->right = removeNode(node->right, temp->course.courseId);
		}
	}
	// return node
	return node;
}
void BinarySearchTree::inOrder(Node* node) {
	//check if node is not null
	if (node != nullptr) {
		//inOrder left
		inOrder(node->left);
		cout << node->course.courseId << ", " << node->course.courseName << endl;
		//Inorder right
		inOrder(node->right);
	}
}

Course BinarySearchTree::Search(string courseId) {
	//Create node pointer currNode to help search through tree, assign with root
	Node* currNode = root;
	//loop until matching courseId is found or until bottom reached
	while (currNode != nullptr) {
		//if match found,return current course
		if (currNode->course.courseId.compare(courseId) == 0) {
			return currNode->course;
		}
		//if the course is smaller than current Node traverse left
		if (courseId.compare(currNode->course.courseId) < 0) {
			currNode = currNode->left;
		}
		//otherwise traverse right of current Node
		else {
			currNode = currNode->right;
		}
	}
	Course course;
	return course;
}

//Logic for reading course data
void loadCourses(string csvPath, BinarySearchTree* bst) {
	//variables to read csv file
	ifstream inFS;
	string line;
	//opening csvfile
	inFS.open(csvPath);
	//check if file csvPath opened successfully
	if (!inFS.is_open()) {
		cout << "File could not open\n";
		return;
	}
	//Looping while not end of file
	while (getline(inFS, line)) {
		//variables to store course data
		Course aCourse;
		stringstream stringS(line);
		//check if line is empty
		if (line.empty()) {
			continue;
		}
		//variables to read course data
		string tempCourseId, tempCourseName, preReq = "";
		//reading the csv line until comma is reached for both
		if (getline(stringS, tempCourseId, ',') && getline(stringS,	tempCourseName,',')) {
			//assigning course's data with the data that read
			aCourse.courseId = tempCourseId;
			aCourse.courseName = tempCourseName;
			//loop to add prerequisite data to course's prerequisite list
			while (getline(stringS, preReq, ',')) {
				if (!preReq.empty()) {
					aCourse.prelist.push_back(preReq);
				}
			}
			//inserting the course into the binary search tree
			bst->Insert(aCourse);
		}
		


	}
	//closing the filestream
	inFS.close();
}

void displayCourse(Course aCourse) {
	cout << aCourse.courseId << ", " << aCourse.courseName << endl;
	cout << "Prerequisites: ";
	for (unsigned int i = 0; i < aCourse.prelist.size(); i++) {
		cout << aCourse.prelist.at(i);
		if (aCourse.prelist.size() > 1 && i < aCourse.prelist.size() - 1) {
			cout << ", ";
		}
	}
	cout << endl;
}
int main(int argc, char* argv[]) {
	//command line arguments
	string csvPath, aCourseKey;
	switch (argc) {
	case 2:
		csvPath = argv[1];
		break;
	case 3:
		csvPath = argv[1];
		aCourseKey = argv[2];
		break;
	default:
		csvPath = "CS 300 ABCU_Advising_Program_Input.csv";
	}

	//Create binary search tree to hold all courses
	BinarySearchTree* courseList;
	courseList = new BinarySearchTree();

	Course course;
	bool valid;
	//integer variable to hold user's choice input
	int choice = 0;
	cout << "Welcome to the course planner.\n";
	//looping while choice isnt 9
	while (choice != 9) {
		cout << "1. Load Data Structure.\n2. Print Course List.\n3. Print Course.\n9. Exit.\n";
		cout << "\nWhat would you like to do? ";
		//clearing aCourseKey,choice, and key
		aCourseKey = "";
		string key = " ";
		choice = 0;
		//error handling
		try {
			cin >> choice;
			//limit the choices to being 1-3 or 9
			if (choice > 0 && choice < 4 || (choice == 9)) {
				valid = true;
			}
			else {
				valid = false;
			}
			switch (choice) {
				case 1:
					loadCourses(csvPath, courseList);
					Sleep(GLOBAL_SLEEP_TIME);
					if (courseList->Size() > 1) {
						cout << courseList->Size() << " Courses Loaded\n";
					}
					else {
						cout << courseList->Size() << " Course Loaded\n";
					}
					break;
				case 2:
					cout << "Here is a sample schedule:\n " << endl;
					courseList->InOrder();
					cout << endl;
					break;
				case 3:
					cout << "\nWhat course do you want to know about? \n";
					cin >> aCourseKey;

					caseConverter(aCourseKey);
					course = courseList->Search(aCourseKey);
					if (!course.courseId.empty()) {
						cout << endl;
						displayCourse(course);
					}
					else {
						cout << "\nCourse ID " << aCourseKey << " not found.\n";
					}

					Sleep(GLOBAL_SLEEP_TIME);
					break;
				case 9:
					cout << "Thank you for using the course planner!\n";
					break;
				default:
					throw runtime_error(to_string(choice) + " is not a valid option\n");
			}
		}
		catch (const runtime_error& e) {
			cout << e.what() << endl;
			cin.clear();
			cin.ignore(1000,'\n');
		}
	}
}

