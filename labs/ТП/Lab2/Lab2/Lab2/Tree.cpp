#include "Tree.h"
#include <iostream>
#include <queue>

using std::cout;
using std::cin;
using std::queue;

Tree::Tree()
{
	root = nullptr;
}

Tree::~Tree()
{
	delete_tree();
}

void Tree::insert()
{
	if (root != nullptr) insert(root);
	else {
		root = new TreeNode;
		cout << "Enter value: ";
		cin >> root->value;
		cout << "Enter size matrix";
		cin >> root->size;
		root->array_d = new double[root->size];
		for (int i = 0; i < root->size; i++) cin >> root->array_d[i];
		root->left = nullptr;
		root->right = nullptr;
	}
}

TreeNode * Tree::search(int key)
{
	return search(key, root);
}

void Tree::delete_tree()
{
	delete_tree(root);
}

void Tree::print()
{
	print(root);
}

void Tree::insert(TreeNode *&leaf)
{
	cout << "Enter value: ";
	int key;
	cin >> key;
	if (key < leaf->value) {
		if (leaf->left != nullptr) insert(leaf->left);
		else {
			leaf->left = new TreeNode;
			leaf->left->value = key;
			cout << "Enter size matrix: ";
			cin >> leaf->left->size;
			leaf->left->array_d = new double[leaf->left->size];
			for (int i = 0; i < leaf->left->size; i++) cin >> leaf->left->array_d[i];
			leaf->left->left = nullptr;
			leaf->left->right = nullptr;
		}
	}
	else if (key >= leaf->value) {
		if (leaf->right != nullptr) insert(leaf->right);
		else {
			leaf->right = new TreeNode;
			leaf->right->value = key;
			cout << "Enter size matrix: ";
			cin >> leaf->right->size;
			leaf->right->array_d = new double[leaf->right->size];
			for (int i = 0; i < leaf->right->size; i++) cin >> leaf->right->array_d[i];
			leaf->right->left = nullptr;
			leaf->right->right = nullptr;
		}
	}
}

TreeNode * Tree::search(int key, TreeNode *leaf)
{
	if (leaf != nullptr) {
		if (key == leaf->value) return leaf;
		if (key < leaf->value)
			return search(key, leaf->left);
		else
			return search(key, leaf->right);
	}
	return nullptr;
}

void Tree::delete_tree(TreeNode *&leaf)
{
	if (leaf != nullptr) {
		delete_tree(leaf->left);
		delete_tree(leaf->right);
		delete[]leaf->array_d;
		delete leaf;
		leaf = nullptr;
	}
}

void Tree::print(TreeNode * leaf)
{
	if (leaf == nullptr) return;

	print(leaf->left);
	cout << leaf->value << " ";
	print(leaf->right);
}
