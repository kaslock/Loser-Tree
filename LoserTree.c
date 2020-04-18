
#include <stdio.h>
#include <stdlib.h>
#define numRow 5  /* power of 2 (2, 4, 8, 16, ...)가 아닌 값으로 설정 */
#define numCol 10

int matrix[numRow][numCol] = {
{ 5,90,5,9,80,80,7,90,7,90 },
{ 100,30,30,51,160,160,160,51,160,59 },
{ 500,100,7000,100,900,600,100,100,650,100 },
{ 1000,300,41,300,41,41,41,900,900,950 },
{ 90,81,81,95,81,83,81,90,81,90 }
};

typedef struct Node* treePointer;
typedef struct Node {	// binary search tree 구성
	int row;			// 행렬의 행을 저장
	int key;			// 정수 값
	int count;			// 같은 정수의 개수
	treePointer left;
	treePointer right;
} Node;

treePointer* root[numRow];	// 각 binary search tree의 root를 가리키는 포인터
treePointer loserArray[numRow * 2];		// loser tree를 배열로 저장
treePointer getNode = NULL;

int maxCount;
int theKey;

treePointer iterSearch(treePointer tree, int k) {

	treePointer parent = NULL;

	while (tree) {		// 같은 정수가 나오면 count 1 증가
		if (k == tree->key) {
			tree->count++;
			return NULL;
		}

		parent = tree;
		if (k < tree->key)
			tree = tree->left;	// 정수 값이 작으면 왼쪽
		else
			tree = tree->right;	// 정수 값이 크면 오른쪽
	}
	return parent;		// 부모노드를 반환
}

void insert(treePointer *node, int k, int row) {	// 저장되지 않은 정수 값이 있으면 노드를 생성하여 저장

	treePointer ptr;
	treePointer temp = iterSearch(*node, k);

	if (temp || !(*node)) {		// 노드 생성
		ptr = (treePointer)(malloc(sizeof(Node)));
		ptr->row = row;
		ptr->key = k;
		ptr->count = 1;
		ptr->left = NULL;
		ptr->right = NULL;

		if (*node) {
			if (k < temp->key) temp->left = ptr;
			else temp->right = ptr;
		}
		else *node = ptr;
	}
}

void traverse(treePointer tree, int row) {	// loser tree에 다음으로 들어갈 노드를 찾음
	
	if (!getNode) {
		maxCount = 0;
		theKey = 0;
	}

	if (tree == NULL) return;
	if ((tree->count > maxCount) || (tree->count == maxCount && tree->key > theKey)) { // tie breaking 규칙
		if (loserArray[row + numRow]) {
			if (tree->count < loserArray[row + numRow]->count || (tree->count == loserArray[row + numRow]->count && tree->key < loserArray[row + numRow]->key)) {
				getNode = tree;
				maxCount = getNode->count;
				theKey = getNode->key;
			}
		}
		else {
			getNode = tree;
			maxCount = getNode->count;
			theKey = getNode->key;
		}
	}
	traverse(tree->left, row);
	traverse(tree->right, row);
}

void loserTree(int x) {		// loser tree 알고리즘
	
	if (!loserArray[x] && x < numRow) {		// non-leaf node가 null일 때
		if (!loserArray[(x/2)*2] && !loserArray[(x / 2) * 2 + 1]) loserTree(x / 2);
		else if (!loserArray[(x / 2) * 2]) loserTree((x / 2) * 2 + 1);
		else if (!loserArray[(x / 2) * 2 + 1]) loserTree((x / 2) * 2);
		return;
	}
	if (!loserArray[x] && x >= numRow) {	// leaf node가 null일 때
		loserTree(x / 2);
		return;
	}

	treePointer p = loserArray[x];
	treePointer tmp = NULL;

	for (int i = x; i > 0; i /= 2) {	// 비어있는 부모 노드가 나올 때까지 반복
		if (loserArray[i / 2]) {
			if (!compare(p, loserArray[i / 2])) {	// 부모 노드보다 작으면 그 위치에 저장 후 부모 노드가 올라감
				tmp = p;
				p = loserArray[i / 2];
				loserArray[i / 2] = tmp;
			}
		}
		else {
			if (i >= numRow && (!loserArray[(i / 2) * 2] || !loserArray[(i / 2) * 2 + 1]))
				continue;
			loserArray[i / 2] = p;
			break;
		}
	}
	if (loserArray[0]) {	// index 0이 채워지면 반환
		if (x < numRow) loserArray[x] = NULL;
		return;
	}
	if (x > numRow) loserTree(x - 1);	// 초기 loser tree를 구성하기 위하여 역순으로 노드 호출
}

int compare(treePointer a, treePointer b) {		// a 노드와 b노드를 비교하여 a 노드가 크면 1, b 노드가 크면 0

	if (a->count == b->count) {	// tie breaking 규칙 적용
		if (a->key == b->key) return a->row > b->row;
		else return a->key > b->key;
	}
	else return a->count > b->count;
}

void main() {

	for (int i = 0; i < numRow; i++)	// 행렬을 Binary tree로 구성
		for (int j = 0; j < numCol; j++)
			insert(root + i, matrix[i][j], i);

	for (int i = 0; i < numRow; i++) {	// 루저트리 초기 리프노드 설정
		traverse(root[i], i);
		loserArray[numRow + i] = getNode;
		getNode = NULL;
	};

	loserTree(numRow * 2 - 1);
	printf("(%d, %d, %d)\n", loserArray[0]->row, loserArray[0]->key, loserArray[0]->count); // 전체 승자

	while (1) {
		int d = loserArray[0]->row;
		if (loserArray[d + numRow])	// Binary tree에 출력되지않은 노드 중 빈도가 가장 높은 정수 값을 가져옴
			traverse(root[d], d);
		loserArray[d + numRow] = getNode;

		getNode = NULL;
		loserArray[0] = NULL;
		loserTree(d + numRow);

		if (!loserArray[1]) break; // 종료조건
		printf("(%d, %d, %d)\n", loserArray[0]->row, loserArray[0]->key, loserArray[0]->count);	// 가장 높은 정수값부터 내림차순으로 정렬하여 출력
	}
}