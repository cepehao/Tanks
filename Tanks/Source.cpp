#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

class GameObject {
private:
	int id;

public:
	GameObject(int newId) {
		id = newId;
	}

	//get
	virtual int Id() {
		return id;
	}
	//set
	virtual void SetId(int newId) {
		id = newId;
	}

	virtual void Print() {
		cout << "id: " << id << endl;
	}

	virtual ~GameObject() {

	}
};

class PhysicObject : public virtual GameObject {
private:
	int weight;

public:
	PhysicObject(int newId, int newWeight) : GameObject(newId) {
		weight = newWeight;
	}

protected:
	virtual void Print() {
		cout << "Масса: " << weight << endl;
	}
};

class GraphicObject : public virtual GameObject {
private:
	int texture;

public:
	GraphicObject(int newId, int newTexture) : GameObject(newId) {
		texture = newTexture;
	}

protected:
	virtual void Print() {
		cout << "Текстура: " << texture << endl;
	}
};

class Projectile : public virtual PhysicObject {
private:
	int caliber;

public:
	Projectile(int newId, int newWeight, int newCaliber) : PhysicObject(newId, newWeight), GameObject(newId) {
		caliber = newCaliber;
	}

protected:
	virtual void Print() {
		GameObject::Print();
		PhysicObject::Print();
		cout << "Калибр: " << caliber << endl;
	}
};

class Vehicle : public virtual PhysicObject {
private:
	int enginePower;

public:
	Vehicle(int newId, int newWeight, int newEnginePower) : PhysicObject(newId, newWeight), GameObject(newId) {
		enginePower = newEnginePower;
	}

protected:
	virtual void Print() {
		GameObject::Print();
		PhysicObject::Print();
		cout << "Мощность двигателя: " << enginePower << endl;
	}
};

class Tank : public virtual Vehicle, public virtual GraphicObject {
private:
	int armor;

public:
	Tank(int newId, int newWeight, int newEnginePower, int newTexture, int newArmor) :
		Vehicle(newId, newWeight, newEnginePower), PhysicObject(newId, newWeight), GraphicObject(newId, newTexture), GameObject(newId) {
		armor = newArmor;
	}

protected:
	virtual void Print() {
		Vehicle::Print();
		GraphicObject::Print();
		cout << "Толщина брони: " << armor << endl;
	}
};


class Airplane : public virtual Vehicle, public virtual GraphicObject {
private:
	int capacity;

public:
	Airplane(int newId, int newWeight, int newEnginePower, int newTexture, int newCapacity) :
		Vehicle(newId, newWeight, newEnginePower), PhysicObject(newId, newWeight), GraphicObject(newId, newTexture), GameObject(newId) {
		capacity = newCapacity;
	}

protected:
	virtual void Print() {
		Vehicle::Print();
		GraphicObject::Print();
		cout << "Грузоподъемность: " << capacity << endl;
	}
};

struct TNode
{
	GameObject* Data;
	TNode* Left;
	TNode* Right;

};
typedef TNode* PNode;

void Insert(PNode& root, int id, int weight, int caliber) {
	if (!root) {
		PNode newElem = new TNode;
		newElem->Data = new Projectile(id, weight, caliber);
		newElem->Left = nullptr;
		newElem->Right = nullptr;
		root = newElem;
		return;
	}

	if (id < root->Data->Id())
		Insert(root->Left, id, weight, caliber);
	else if (id > root->Data->Id())
		Insert(root->Right, id, weight, caliber);
}

void Insert(PNode& root, int choice, int id, int weight, int texture, int enginePower, int TransportCharacter) {
	if (!root) {
		PNode newElem = new TNode;
		if (choice == 1) newElem->Data = new Tank(id, weight, texture, enginePower, TransportCharacter);
		else newElem->Data = new Airplane(id, weight, texture, enginePower, TransportCharacter);
		newElem->Left = nullptr;
		newElem->Right = nullptr;
		root = newElem;
		return;
	}

	if (id < root->Data->Id())
		Insert(root->Left, choice, id, weight, texture, enginePower, TransportCharacter);
	else if (id > root->Data->Id())
		Insert(root->Right, choice, id, weight, texture, enginePower, TransportCharacter);
}

bool Find(PNode root, int x)
{
	if (!root) return false;
	if (root->Data->Id() == x) return true;
	if (root->Data->Id() > x)
		return Find(root->Left, x);
	else
		return Find(root->Right, x);
}

void Pryam(PNode root, int h = 0) {
	if (!root) return;
	root->Data->Print();
	Pryam(root->Left, h + 1);
	Pryam(root->Right, h + 1);
}

void Obrat(PNode root, int h = 0) {
	if (!root) return;
	Obrat(root->Left, h + 1);
	root->Data->Print();
	Obrat(root->Right, h + 1);
}

void Conc(PNode root, int h = 0) {
	if (!root) return;
	Conc(root->Left, h + 1);
	Conc(root->Right, h + 1);
	root->Data->Print();
}

void Del(PNode& root, PNode& q)
{
	if (root->Left)
		Del(root->Left, q);
	else {
		q = root;
		root = root->Left;
	}
}
void DeleteObject(PNode& root, int x)
{
	if (!root) return;
	if (x < root->Data->Id()) DeleteObject(root->Left, x);
	else if (x > root->Data->Id()) DeleteObject(root->Right, x);
	else
	{
		PNode q = root;
		if (!root->Left) root = root->Right;
		else if (!root->Right) root = root->Left;
		else
		{
			Del(root->Right, q);
			root->Data->SetId(q->Data->Id());
		}
		delete q;
	}
}

void DestroyTree(PNode& root)
{
	if (!root) return;
	if (root->Left)
		DestroyTree(root->Left);
	if (root->Right)
		DestroyTree(root->Right);
	delete root->Data;
	delete root;
}

void CreateObject(PNode& root, int choice) {
	cout << "1 - Снаряд" << endl;
	cout << "2 - Танк" << endl;
	cout << "3 - Самолет" << endl;
	cout << "0 - Назад" << endl;
	int c, id, weight, armor, texture, enginePower, caliber, capacity;
	cin >> c;
	if (c == 0) return;
	cout << "id: ";
	cin >> id;

	if (Find(root, id)) {
		cout << "Объект с идентификатором " << id << " уже создан" << endl;
		return;
	}
	cout << "Вес: ";
	cin >> weight;
	switch (c) {
	case 1:
		cout << "Калибр: ";
		cin >> caliber;
		Insert(root, id, weight, caliber);
		break;

	case 2:
	case 3:
		cout << "Текстура: ";
		cin >> texture;
		cout << "Мощность двигателя: ";
		cin >> enginePower;
		if (c == 2) {
			cout << "Толщина брони: ";
			cin >> armor;
			Insert(root, c, id, weight, texture, enginePower, armor);
		}
		else {
			cout << "Грузоподъемность: ";
			cin >> capacity;
			Insert(root, c, id, weight, texture, enginePower, capacity);
		}

		break;
	}
}

void PrintTree(PNode root) {
	cout << "1 - Прямой обход" << endl;
	cout << "2 - Обратный обход" << endl;
	cout << "3 - Концевой обход" << endl;
	int choice;
	cin >> choice;
	cout << "---------------------" << endl;
	switch (choice) {
	case 1:
		Pryam(root);
		break;

	case 2:
		Obrat(root);
		break;

	case 3:
		Conc(root);
		break;
	}
	cout << "---------------------" << endl;
}

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	PNode root = nullptr;

	while (true) {
		cout << "1 - Добавить объект" << endl;
		cout << "2 - Удалить объект" << endl;
		cout << "3 - Печать дерева" << endl;
		cout << "0 - Завершение программы" << endl;
		int choice;
		cin >> choice;
		switch (choice) {
		case 1:
			CreateObject(root, choice);
			break;

		case 2:
			cout << "id объекта для удаления : ";
			int id;
			cin >> id;
			DeleteObject(root, id);
			break;

		case 3:
			PrintTree(root);
			break;

		case 0:
			DestroyTree(root);
			exit(0);
			break;
		}
	}
}




