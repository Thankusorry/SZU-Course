#include<stdio.h>
#include<stdlib.h>
#include<time.h>

//二叉树的一个节点
typedef struct Node {
  int val;//数据域
  struct Node* lchild, * rchild;//左指针和右指针
}Node;

//二叉树的树型结构
typedef struct Tree {
  Node* root;
  int n;
}Tree;

//初始化节点
Node* getNewNode(int val) {
  Node* p = (Node*)malloc(sizeof(Node));
  if (p == NULL)return 0;
  p->val = val;
  p->lchild = p->rchild = NULL;
  return p;
}

//初始化树形结构
Tree* getNewTree() {
  Tree* tree = (Tree*)malloc(sizeof(Tree));
  if (tree == NULL)return 0;
  tree->n = 0;
  tree->root = NULL;
  return tree;

}

//二叉排序树的插入方式
Node* insertNode(Node* root, int val, int *ret) {
  if (root == NULL)
  {
    *ret = 1;
    return getNewNode(val);
  }
  if (root->val == val)return root;
  if (root->val > val)root->lchild = insertNode(root->lchild, val, ret);
  else root->rchild = insertNode(root->rchild, val, ret);
  return root;
}

//排序二叉树：插入节点比当前根节点的值小往左子树插，大则往右插。
void insert(Tree* tree, int val) {
  int flag = 0;
  tree->root = insertNode(tree->root, val, &flag);
  tree->n += flag;
  return;
}

//销毁节点
void clearNode(Node *node) {
  if (node == NULL)return;
  clearNode(node->lchild);//clear 当前的左孩子
  clearNode(node->rchild);//clear 当前的右孩子
  free(node);
  return;
}

//销毁树形结构
void clearTree(Tree *tree) {
  clearNode(tree->root);
  free(tree);
  return;
}

void outputNode(Node *root) {
  if (root == NULL)return;
  printf("%d", root->val);
  if (root->lchild == NULL && root->rchild == NULL)return;
  printf("(");
  outputNode(root->lchild);
  printf(",");
  outputNode(root->rchild);
  printf(")");
  return;
}


//二叉树转广义表
void outputTree(Tree *tree) {
  printf("tree(%d) = ",tree->n);
  outputNode(tree->root);
  printf("\n");
  return;
}

//前序遍历
void preorderNode(Node* node) {
  if (node == NULL)return;
  printf(" %d", node->val);
  preorderNode(node->lchild);
  preorderNode(node->rchild);
  return;
}

void preorder(Tree* tree) {
  printf("preorder:");
  preorderNode(tree->root);
  printf("\n");
  return;
}

//中序遍历
void inorderNode(Node* node) {
  if (node == NULL)return;
  inorderNode(node->lchild);
  printf(" %d", node->val);
  inorderNode(node->rchild);
  return;
}

void inorder(Tree* tree) {
  printf("inorder:");
  inorderNode(tree->root);
  printf("\n");
  return;
}

//后序遍历
void postorderNode(Node* node) {
  if (node == NULL)return;
  postorderNode(node->lchild);
  postorderNode(node->rchild);
  printf(" %d", node->val);
  return;
}

void postorder(Tree* tree) {
  printf("postorder:");
  postorderNode(tree->root);
  printf("\n");
  return;
}

int main() {
  srand((unsigned)time(0));
  Tree* tree = getNewTree();
  for (int i = 0; i < 10; i++)
  {
    int val = rand() % 100;
    insert(tree, val);
    outputTree(tree);
  }
  preorder(tree);      //前序遍历
  inorder(tree);      //中序遍历
  postorder(tree);   //后序遍历
  clearTree(tree);  
  return 0;
}